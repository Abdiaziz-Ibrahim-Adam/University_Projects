/*
 * Programmering i C
 * Autumn 24
 * 
 * File         : mexec.c
 * Course       : Systemnära programmering (C programming and Unix)
 * 
 * Description  : This program executes a series of commands as a pipeline, 
 *                linking the output of one command to the input of the next.
 * 
 * Author       : Abdiaziz Ibrahim Adam
 * CS usernames : dv23aam
 * Date         : 2024-10-14
 * 
 * Input        : Commands from either a file or standard input
 * Output       : Output comes from executed commands.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "mexec.h"

/* ------------------------------- Constants ------------------------------- */
#define LINE_LENGTH 1024  
#define INITIAL_CAPACITY 10
/* ------------------------------------------------------------------------- */

int main(int argc, char *argv[]) 
{
    int numCommands = 0;
    int capacity = INITIAL_CAPACITY;
    FILE *in_file;

    char **command_arguments = calloc(INITIAL_CAPACITY, sizeof(char *)); 
    if (command_arguments == NULL) {
        free_memory_for_commands(numCommands, command_arguments);
        handle_error("Failed to allocate initial memory for commands array");
    }

    // Handle input source (stdin or file)
    if (argc == 1) {
        in_file = stdin;
    } else if (argc == 2) {
        in_file = fopen(argv[1], "r");
        if (!in_file) {
            free_memory_for_commands(numCommands,command_arguments);
            handle_error("Error opening the file");
        }
    } else {
        free_memory_for_commands(numCommands, command_arguments);
        handle_error("Too many arguments, usage: ./mexec [FILE]");
    }

    // Read command lines from file or stdin
    command_arguments = read_commandlines(command_arguments, &numCommands, in_file, &capacity);

    if (command_arguments == NULL) {
        if (in_file != stdin) {
            fclose(in_file);
        }
        free_memory_for_commands(numCommands, command_arguments);
        handle_error("Failed to read commands");
    }

    // Execute commands with pipes
    execute_commands(numCommands, command_arguments);

    if (in_file != stdin) {
        fclose(in_file);
    }
    free_memory_for_commands(numCommands, command_arguments);

    return EXIT_SUCCESS; 
}

/* -------------------------- Function implementations -------------------------------------------- */

// Function to read all input into a single buffer and tokenize it
char **read_commandlines(char **command_arguments, int *numCommands, FILE *in_file, int *capacity) {
    char line[LINE_LENGTH];

    // Read input lines
    while (fgets(line, sizeof(line), in_file)) {
        char *trimmed_line = strtok(line, "\n");  // Remove newline
        if (trimmed_line == NULL || strlen(trimmed_line) == 0) {
            continue;
        }

        if (*numCommands >= *capacity) {
            *capacity *= 2;
            char **newCommands = realloc(command_arguments, (*capacity) * sizeof(char *));
            if (newCommands == NULL) {
                free_memory_for_commands(*numCommands, command_arguments);
                handle_error("Failed to reallocate memory for commands");
            }
            command_arguments = newCommands;
        }

        // Allocate memory for each command
        command_arguments[*numCommands] = malloc(strlen(trimmed_line) + 1);  
        if (command_arguments[*numCommands] == NULL) {
            free_memory_for_commands(*numCommands, command_arguments);
            handle_error("Failed to allocate memory for command");
        }
        strcpy(command_arguments[*numCommands], trimmed_line);  // Copy the line
        (*numCommands)++;
    }

    return command_arguments;
}

// Function to parse a command string into an array of arguments
char **parse_command(char *command) {
    int arg_capacity = INITIAL_CAPACITY;
    char **commands = calloc(arg_capacity, sizeof(char *));
    if (commands == NULL) {
        free_memory_for_commands(arg_capacity, commands);
        handle_error("Failed to allocate memory for arguments");
    }
    char *cmd = strtok(command, " \n");

    int cmd_index = 0;

    while (cmd != NULL) {
        commands[cmd_index] = strdup(cmd);  

        if (commands[cmd_index] == NULL) 
        {
            handle_error("Failed to allocate memory for argument");
        }
        cmd_index++;

        if (cmd_index >= arg_capacity) 
        {
            arg_capacity *= 2;
            commands = realloc(commands, arg_capacity * sizeof(char *));
            if (commands == NULL) 
            {
                free_memory_for_commands(arg_capacity, commands);
                handle_error("Failed to reallocate memory for arguments");
            }
        }
        cmd = strtok(NULL, " \n");
    }

    commands[cmd_index] = NULL; // Null-terminate the arguments list

    return commands;
}

// Function to execute commands with pipes
void execute_commands(int numCommands, char **command_arguments) {
    int **pipes = create_pipes(numCommands);

    if (pipes == NULL)
    {
        free_pipes(pipes, numCommands);
        handle_error("failed to create pipes");
    }
    fork_and_execute(numCommands, command_arguments, pipes);

    close_pipes(pipes, numCommands);

    wait_for_children_and_cleanup(pipes, numCommands, command_arguments);
}

// Helper function: Create pipes for inter-process communication
int **create_pipes(int numCommands) {
    int **pipes = malloc((numCommands - 1) * sizeof(int *));
    if (pipes == NULL) 
    {
        free_pipes(pipes, numCommands);
        handle_error("Failed to allocate memory for pipes");
    }

    for (int i = 0; i < numCommands - 1; i++) 
    {
        pipes[i] = malloc(2 * sizeof(int));
        if (pipes[i] == NULL) {
            free_pipes(pipes, numCommands);
            handle_error("Failed to allocate memory for pipes");
        }
        if (pipe(pipes[i]) == -1) {
            free_pipes(pipes, numCommands);
            handle_error("pipe failed");
        }
    }
    return pipes;
}

// Helper function: Fork processes and execute commands
void fork_and_execute(int numCommands, char **command_arguments, int **pipes) {
    for (int i = 0; i < numCommands; i++) {
        pid_t pid = fork();

        if (pid == -1) {
            free_pipes(pipes, numCommands - 1);
            handle_error("fork failed");
        }

        if (pid == 0) {  // Child process
            if (i > 0) {
                if (dup2(pipes[i - 1][0], STDIN_FILENO) == -1) {
                    handle_error("Failed to redirect standard input");
                }
            }
            if (i < numCommands - 1) {
                if (dup2(pipes[i][1], STDOUT_FILENO) == -1) {
                    handle_error("Failed to redirect standard output");
                }
            }
            close_pipes(pipes, numCommands); // Close all pipes in the child process

            char **args = parse_command(command_arguments[i]);
            execvp(args[0], args); // Execute the command

            free_pipes(pipes, numCommands); 
            free_memory_for_commands(numCommands, command_arguments);
            fprintf(stderr, "%s: No such file or directory\n", args[0]);
            free_memory_for_commands(numCommands, args); 
            exit(EXIT_FAILURE);
        }

    }
}

// Function to close all pipes in a process
void close_pipes(int **pipes, int numCommands) {
    for (int i = 0; i < numCommands - 1; i++) {
        if (pipes[i] != NULL) {
            close(pipes[i][0]);
            close(pipes[i][1]);
        }
    }
}

// Helper function: Wait for child processes and clean up resources
void wait_for_children_and_cleanup(int **pipes, int numCommands, char **command_arguments) {
    int status, exit_status = 0;
    while (wait(&status) != -1) {
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
            exit_status = WEXITSTATUS(status);
        }
    }
    free_pipes(pipes, numCommands);
    free_memory_for_commands(numCommands, command_arguments);

    exit(exit_status);
}

// Function to free memory allocated for the pipes
void free_pipes(int **pipes, int numCommands) {
    for (int i = 0; i < numCommands - 1; i++) {
        if (pipes[i] != NULL) {
            free(pipes[i]);  // Free each pipe array
        }
    }
    free(pipes);  // Free the pipes array itself
}

// Function to free memory allocated for commands
void free_memory_for_commands(int numCommands, char **command_arguments) {
    for (int i = 0; i < numCommands; i++) {
        if (command_arguments[i] != NULL) {
            free(command_arguments[i]);  // Free each command argument
        }
    }
    free(command_arguments);  // Free the command argument array itself
}

// Error handling function
void handle_error(const char *msg) {
    perror(msg);  
    exit(EXIT_FAILURE);  
}
