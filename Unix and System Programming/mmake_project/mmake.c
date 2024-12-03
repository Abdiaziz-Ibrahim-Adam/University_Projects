/*
 * Programmering i C
 * Autumn 24
 * 
 * File         : mmake.c
 * Course       : Systemnära programmering (C programming and Unix)
 * 
 * Description  : This program simulates a minimal version of the make utility,
 *                handling simple makefiles to automate the build process. This
 *                program relies on the external modules 'parser.h' and 'parser.c'
 *                to handle makefile parsing. 
 * 
 * Author       : Abdiaziz Ibrahim Adam
 * CS usernames : dv23aam
 * Date         : 1 Novermeber 2024
 * 
 * Input        : Command line arguments to specify makefile path, whether to force build, and silent mode operation.
 * Output       : Executes build commands as specified by the makefile.
 * 
 *
 * Note         : This code uses external modules 'parser.h' and 'parser.c' handle makefile parsing and are part of
 *                the course materials, not authored by the me.
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include <bits/getopt_core.h>
#include <sys/wait.h>
#include <string.h>
#include "parser.h"

/* ------------------ Function Prototypes -------------------------------------------------------------------- */
void parse_cmds(int argc, char *argv[], char **makefile_ptr, bool *force_build, bool *silent_mode);
void build_targets(const char *target, makefile *m, bool force_build, bool silent_mode);
void handle_no_rule_found(const char *target);
void build_dependencies(rule *target_rule, makefile *m, bool force_build, bool silent_mode);
bool needs_build(const char *target, rule *r);
void execute_cmds(char **cmd);
void handle_error(const char *msg);

int main(int argc, char *argv[]) 
{
    char *makefile_ptr;
    bool force_build = false, silent_mode = false;

    // Parse command-line arguments to configure the program's operation
    parse_cmds(argc, argv, &makefile_ptr, &force_build, &silent_mode);

    // Attempt to open the specified makefile
    FILE *f_ptr = fopen(makefile_ptr, "r");
    if (!f_ptr) {
        fprintf(stderr, "%s: No such file or directory\n", makefile_ptr);
        exit(EXIT_FAILURE);
    }

    // Parse the makefile using the external parser utility
    makefile *m = parse_makefile(f_ptr);
    fclose(f_ptr);
    if (!m) {
        fprintf(stderr, "%s: Could not parse makefile\n", makefile_ptr);
        exit(EXIT_FAILURE);
    }

    // Determine if specific targets were specified, otherwise build the default target
    if (optind < argc) {
        for (int i = optind; i < argc; i++) {
            build_targets(argv[i], m, force_build, silent_mode);
        }
    } else {
        const char *default_target = makefile_default_target(m);
        if (default_target == NULL) {
            fprintf(stderr, "No default target specified.\n");
            makefile_del(m);
            return EXIT_FAILURE;
        }
        build_targets(default_target, m, force_build, silent_mode);
    }

    // Cleanup allocated makefile structure
    makefile_del(m);
    return EXIT_SUCCESS;
}

/* ------------------ Function Definitions -------------------------------------------------------------------- */

/**
 * Parses command line arguments to configure the program behavior.
 *
 * @param argc The number of command line arguments.
 * @param argv Array containing the command line arguments.
 * @param makefile_ptr Pointer to store the path to the makefile. 
 *                     This should point to an existing string or memory allocated 
 *                     sufficient to hold the path.
 * @param force_build Pointer to a boolean that will be set if the build should be forced.
 * @param silent_mode Pointer to a boolean that will be set for silent operation.
 */
void parse_cmds(int argc, char *argv[], char **makefile_ptr, bool *force_build, bool *silent_mode) 
{
    *makefile_ptr = "mmakefile"; // Default makefile 
    int opt;
    while ((opt = getopt(argc, argv, "f:Bs")) != -1) {
        switch (opt) {
            case 'f':
                *makefile_ptr = optarg; // Set makefile path from -f option
                break;
            case 'B':
                *force_build = true;   // Set force build flag from -B option
                break;
            case 's':
                *silent_mode = true;   // Set silent mode flag from -s option
                break;
            default:
                fprintf(stderr, "Usage: %s [-f makefile] [-B] [-s] [target...]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }
}

/**
 * Builds the specified target according to the rules defined in the makefile.
 * Assumes makefile_rule and rule_cmd from parser.h allocate and manage memory internally.
 *
 * @param target The name of the target to build.
 * @param m Pointer to the parsed makefile structure, which should be managed
 *          according to the specifications provided by parser.h.
 * @param force_build Boolean indicating whether to force the build.
 * @param silent_mode Boolean indicating whether to suppress command output.
 */
void build_targets(const char *target, makefile *m, bool force_build, bool silent_mode) 
{
    rule *target_rule = makefile_rule(m, target);  
    if (target_rule == NULL) {
        handle_no_rule_found(target);
        return;
    }
    build_dependencies(target_rule, m, force_build, silent_mode);
 
    if (force_build || needs_build(target, target_rule)) {
        char **cmd = rule_cmd(target_rule);
        if (!silent_mode) {
            for (int i = 0; cmd[i] != NULL; i++) {
                printf("%s", cmd[i]);
                if (cmd[i + 1])
                {
                    printf(" ");
                }  
            }
            printf("\n");
        }
        execute_cmds(cmd);
    }
}

/**
 * Handles the case when no build rule is found for the specified target.
 * Exits the program if no file exists for the given target and no rules are defined to build it.
 *
 * @param target The target for which no build rule was found.
 */
void handle_no_rule_found(const char *target) 
{
    struct stat buffer;
    if (stat(target, &buffer) != 0) {
        fprintf(stderr, "mmake: No rule to make target '%s'\n", target);
        exit(EXIT_FAILURE);
    }
}

/**
 * Recursively builds dependencies for a target. Assumes dependencies are managed
 * by the makefile structure.
 *
 * @param target_rule The build rule for the target.
 * @param m Pointer to the parsed makefile structure.
 * @param force_build Boolean indicating whether to force the build.
 * @param silent_mode Boolean indicating whether to suppress command output.
 */
void build_dependencies(rule *target_rule, makefile *m, bool force_build, bool silent_mode) 
{
    const char **dependencies = rule_prereq(target_rule);
    if (dependencies == NULL) {
        fprintf(stderr, "Error: No dependencies found or could not retrieve dependencies.\n");
        exit(EXIT_FAILURE);  
    }

    for (int i = 0; dependencies[i] != NULL; i++) {
        build_targets(dependencies[i], m, force_build, silent_mode);
    }
}

/**
 * Determines whether a target needs to be rebuilt based on the modification times of its prerequisites.
 *
 * @param target The target to check.
 * @param r The rule associated with the target. Assumes that this structure is managed
 *          according to the specifications provided by parser.h.
 * @return true if the target needs to be rebuilt, false otherwise.
 */
bool needs_build(const char *target, rule *r) 
{
    struct stat target_stat;
    if (stat(target, &target_stat) != 0) {
        return true;
    }

    const char **prereqs = rule_prereq(r);
     if (prereqs == NULL) {
        fprintf(stderr, "Error: No prerequisites found.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; prereqs[i] != NULL; i++) {
        struct stat prereq_stat;
        if (stat(prereqs[i], &prereq_stat) != 0 || prereq_stat.st_mtime > target_stat.st_mtime) {
            return true;
        }
    }
    return false;
}

/**
 * Executes a command in a new process and waits for it to finish.
 * Proper error handling is included to handle and report errors during command execution.
 *
 * @param cmd Null-terminated array of strings representing the command and its arguments.
 */
void execute_cmds(char **cmd) 
{
    pid_t pid = fork();
    if (pid == 0) {
        if (execvp(cmd[0], cmd) == -1) {
            handle_error("execvp failed");
        }
    } else if (pid > 0) {
        int status;
        if (waitpid(pid, &status, 0) == -1) {
            handle_error("waitpid failed");
        }
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
            exit(EXIT_FAILURE);
        }
    } else {
        handle_error("fork failed");
    }
}

/**
 * Handles system errors by printing an error message and exiting.
 * Useful for reporting errors from system calls and other library functions.
 *
 * @param msg The error message to print. The function uses perror to provide context
 *            from the global errno if applicable.
 */
void handle_error(const char *msg) 
{
    perror(msg);
    exit(EXIT_FAILURE);
}
