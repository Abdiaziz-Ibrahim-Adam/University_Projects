/**
 * @file paths.h
 * @brief Functions for managing path structures and preparing paths for processing.
 *
 * This module provides utility functions for handling paths, including creating, initializing, 
 * and cleaning up path structures. It also includes helper functions for manipulating and managing 
 * directory and file paths, and ensuring thread-safe management of tasks in the thread pool.
 *
 * @section Author
 * - Author: Abdiaziz Ibrahim Adam
 * - Date: 2024-11-18
 */

#ifndef PATHS_H
#define PATHS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include "mdu.h"
#include "threads.h"

/* ------------------------------- Function Prototypes ------------------------------------------------------------------------------------------------ */

/**
 * @brief Initializes the path tasks for processing and pushes them to the task stack.
 *
 * This function prepares each specified path by creating a corresponding `Path` structure
 * and assigning a unique identifier. Each path task is then added to the stack in the 
 * thread pool for processing.
 *
 * @param path_num The number of paths specified by the user.
 * @param argv Argument vector containing paths to process.
 * @param Pool Pointer to the ThreadPool managing path tasks.
 */
void initialize_paths(int path_num, char *argv[], ThreadPool *thread_pool);

/**
 * @brief Creates and initializes a new `Path` structure.
 *
 * Allocates memory for a `Path` structure and assigns a unique identifier to each path.
 * This function is used to create task items for paths before they are added to the thread pool.
 *
 * @param path_name String representing the name of the directory or file.
 * @param path_id Unique identifier for tracking this path in the thread pool.
 * @return Path* Pointer to the newly created `Path` structure, or NULL on failure.
 *
 * @note The caller is responsible for freeing the memory by calling `cleanup_path`.
 */
Path *create_path(char *path_name, int path_id);

/**
 * @brief Assigns a unique identity to each path within the thread pool.
 *
 * Duplicates the `path_name` string and associates it with the specified path ID in the thread pool’s
 * data structure. This function ensures each path in the pool is tracked with its unique identifier.
 *
 * @param data Pointer to the ThreadPool structure containing path data.
 * @param path_name String representing the path name to assign.
 * @param path_id Unique identifier for the path.
 * @return int Returns 0 on success, or non-zero if memory allocation fails.
 */
int give_path_identity(ThreadPool *data, char *path_name, int path_id);

/**
 * @brief Concatenates a base path with a specified filename.
 *
 * Constructs a new path by combining a base directory path with a filename, ensuring correct path 
 * separators. This function is used to build full paths during recursive traversal.
 *
 * @param base_path Base directory path.
 * @param filename Filename to append to the base path.
 * @return char* Newly created full path string, or NULL on memory allocation failure.
 *
 * @note The caller is responsible for freeing the memory allocated for the returned path string.
 */
char *concatenate_dir_path(const char *base_path, const char *filename);

/**
 * @brief Releases memory associated with a `Path` structure.
 *
 * Frees the memory allocated for the path name and the `Path` structure itself. This function 
 * should be called when a path task is no longer needed.
 *
 * @param path_task Pointer to the `Path` structure to free.
 */
void cleanup_path(Path *path_task);

#endif /* PATHS_H */
