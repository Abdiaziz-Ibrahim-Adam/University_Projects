/**
 * @mainpage Disk Usage Calculation Program (MDU)
 * 
 * @section Introduction
 * This project implements a program to calculate the disk usage of specified directories or files.
 * It mimics the functionality of the UNIX `du` command and includes support for multithreading to
 * improve performance. The user can specify the number of threads to use for traversing directories.
 * Disk usage of directories is recursively calculated, including any subdirectories and files within.
 * 
 * The main data structures used include a stack for task management, path information structures,
 * and a thread pool for handling multiple paths concurrently.
 * 
 * @section Author
 * - Author: Abdiaziz Ibrahim Adam
 * - Date: 2024-11-18
 * 
 * @section files File Structure
 * - mdu.h: Header file containing data structure and function declarations.
 * - mdu.c: Main implementation file for the program.
 * 
 * @section Dependencies Dependencies
 * - `stack.h`, `stack.c`: Provides stack-based storage for managing directory paths as tasks.
 * - `threads.h`, `threads.c`: Manages thread pool initialization, task dispatching, and synchronization.
 * - `directory_usage.h`, `directory_usage.c`: Implements directory traversal and disk usage calculations.
 * - `paths.h`, `paths.c`: Defines and manages path-related functions.
 * 
*/

#ifndef MDU_H
#define MDU_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include "stack.h"

/* ------------------------------- Structures --------------------------------------------------------------------------------------------------- */

/**
 * @struct Path
 * @brief Represents an individual path or directory for processing.
 *
 * This structure holds information about a single path (directory or file), including its name
 * and a unique identifier used for tracking.
 *
 * @param path_name The name or path string of the directory or file.
 * @param path_id Unique identifier associated with each path.
 */
typedef struct Path {
    char *path_name;  /**< Base path of the directory or file. */
    int path_id;      /**< Unique identifier for each path. */
} Path;

/**
 * @struct PathDetails
 * @brief Stores metadata and mutexes for each path processed in the program.
 *
 * This structure keeps track of each path's name and disk usage (in blocks). It also includes an array
 * of mutexes for managing concurrent access to each path's data when using multiple threads.
 *
 * @param path_identity Array of strings storing each path's name.
 * @param dir_size Array storing the disk usage for each path.
 * @param size_count_mut Mutex array for synchronizing access to disk usage data.
 * 
 * @warning Memory is allocated for `path_identity`, `dir_size`, and `size_count_mut` arrays in the
 * `init_ThreadPool` function. These allocations must be freed by calling `cleanup_ThreadPool`.
 */
typedef struct Dir_details {
    char **path_identity;         /**< Array storing each path's name. */
    long *dir_size;               /**< Array for each directory's size count. */
    pthread_mutex_t *size_count_mut; /**< Mutex array for accessing disk usage data. */
} Dir_details;

/**
 * @struct ThreadPool
 * @brief Manages a pool of worker threads for parallel processing.
 *
 * This structure manages synchronization for multithreaded traversal, including the stack of tasks,
 * thread and path counts, exit code, and mutexes. A condition variable is used for coordinating
 * thread activity.
 *
 * @param stack Stack used for managing paths to be processed.
 * @param path_num Total number of paths to process.
 * @param num_threads Number of threads available in the pool.
 * @param exit_code Program's exit code.
 * @param stack_mut Mutex for synchronizing stack access.
 * @param exit_code_mut Mutex for controlling access to the exit code.
 * @param cond Condition variable for coordinating thread activity.
 * @param path_details Pointer to a `Dir_details` structure holding metadata for paths.
 */
typedef struct ThreadPool {
    Stack *stack;               /**< Stack of tasks for path processing. */
    int path_num;               /**< Total number of paths. */
    int num_threads;            /**< Number of available threads. */
    int exit_code;              /**< Program's return code. */
    pthread_mutex_t stack_mut;  /**< Mutex for stack access. */
    pthread_mutex_t exit_code_mut; /**< Mutex for return code access. */
    pthread_cond_t cond;        /**< Condition variable for stack processing. */
    Dir_details *dir_details;  /**< Pointer to Dir_details structure. */
} ThreadPool;

/* ------------------------------- Function Prototypes ------------------------------------------------------------------------------------------------ */

/**
 * @brief Parses command-line arguments for thread count and paths.
 *
 * Parses `argc` and `argv` to determine the number of threads (`-j` flag) and collects paths
 * to process. Returns the specified thread count.
 *
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return int Number of threads specified.
 */
int parse_cmds(int argc, char *argv[]);


/**
 * @brief Prints the results of disk usage calculation.
 *
 * Outputs each path's disk usage, as stored in the ThreadPool's `PathDetails`.
 *
 * @param threadpool Pointer to the ThreadPool with path results.
 */
void print_total_size(ThreadPool *threadpool);

/**
 * @brief Handles errors by printing a message and exiting.
 *
 * Prints the specified error message and terminates the program.
 *
 * @param msg Error message to display.
 */
void handle_error(char *msg);

#endif /* MDU_H */
