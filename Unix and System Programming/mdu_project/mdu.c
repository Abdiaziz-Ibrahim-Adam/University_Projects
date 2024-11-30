/*
 * Programmering i C
 * Autumn 24
 * 
 * File         : mdu.c
 * Course       : Systemnära programmering (C programming and Unix)
 * 
 * Description  : This program calculates the disk usage of specified files and directories.
 *                It supports multithreading to handle directory traversal in parallel if specified
 *                using the -j flag. The user can specify multiple directories or files, and the program
 *                will calculate the total size of each, including subdirectories.
 * 
 *                The program is designed to operate similarly to the du command in Unix, where
 *                directory sizes are recursively calculated, and the total disk space occupied is displayed
 *                in 512-byte blocks.
 * 
 *                When the -j option is provided, the number of threads is specified, and the traversal
 *                is parallelized, dividing work across the threads for improved performance.
 *                If the -j option is not provided, the program defaults to a single-threaded operation.
 * 
 * Author       : Abdiaziz Ibrahim Adam
 * CS usernames : dv23aam
 * Date         : 2024-11-18
 * 
 * Input        : Command-line arguments specifying files or directories and an optional -j flag
 *                to specify the number of threads.
 *       
 * Output       : Prints the size of each directory or file in 512-byte blocks.
 * 
 * Dependencies : 
 *               - `stack.h`, `stack.c`: Provides stack-based storage for managing directory paths as tasks.
 *               - `threads.h`, `threads.c`: Manages thread pool initialization, task dispatching, and synchronization.
 *               - `directory_usage.h`, `directory_usage.c`: Implements directory traversal and disk usage calculations.
 *               - `paths.h`, `paths.c`: Defines and manages path-related functions.
 * 
 * Usage        : 
 *                - Single-threaded mode: ./mdu [file/directory paths...]
 *                - Multithreaded mode:   ./mdu -j [num_threads] [file/directory paths...]
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <bits/getopt_core.h>
#include "mdu.h"
#include "paths.h"
#include "threads.h"
#include "directory_usage.h"


/* ------------------------------- Main Function ------------------------------------------------------------------------------------------------ */

int main(int argc, char *argv[]) 
{
    int path_num, exit_code;
    pthread_t *thread_id;

    // Parse command-line arguments and retrieve the number of threads specified
    int num_threads = parse_cmds(argc, argv);
    path_num = argc - optind;

    // Initialize the thread pool
    ThreadPool *threadpool = init_ThreadPool(path_num, num_threads);
    if (threadpool == NULL) {
        fprintf(stderr, "Couldn't initialize ThreadPool structure.\n");
        exit(EXIT_FAILURE);
    }

    // Initialize paths and dispatch threads for processing
    initialize_paths(path_num, argv, threadpool);
    initialize_dispatch_threads(num_threads, &thread_id, threadpool);
    join_threads(num_threads, thread_id);

    // Print final disk usage result
    print_total_size(threadpool);

    //
    exit_code = threadpool->exit_code;
    cleanup_ThreadPool(threadpool);

    return exit_code;
}
/* ---------------------------------------------------------------------------------------------------------------------------------------------- */


/* ------------------------------- Function Implementations ------------------------------------------------------------------------------------- */

// Parses command-line arguments for the program
int parse_cmds(int argc, char *argv[]) {
    int opt;
    int num_threads = 1;  // Default to single-threaded operation

    while ((opt = getopt(argc, argv, "j:")) != -1) {
        switch (opt) {
            case 'j':
                num_threads = atoi(optarg);
                if (num_threads < 1) {
                    fprintf(stderr, "Error: Number of threads must be at least 1.\n");
                    exit(EXIT_FAILURE);
                }
                break;
            default:
                fprintf(stderr, "Usage: %s [-j num_threads] file1 [file2 ...]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (optind >= argc) {
        fprintf(stderr, "Expected argument after options\n");
        exit(EXIT_FAILURE);
    }
    return num_threads;
}

// Prints disk usage results for each path
void print_total_size(ThreadPool *threadpool) {
    for (int i = 0; i < threadpool->path_num; i++) {
        if (threadpool->dir_details->path_identity[i] != NULL) {
            printf("%ld        %s\n", threadpool->dir_details->dir_size[i], threadpool->dir_details->path_identity[i]);
        }
    }
}

// Handles error reporting and exits the program
void handle_error(char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

/* ---------------------------------------------------------------------------------------------------------------------------------------------- */
