/**
 * @file directory_usage.h
 * @brief Functions for calculating disk usage for specified directories and files,
 *        including recursive traversal of directories.
 *
 * This module provides functions to calculate the disk usage of directories and files.
 * It includes helper functions for traversing directories, processing individual path
 * entries, and safely updating shared directory size data in a multithreaded environment.
 *
 * @section Author
 * - Author: Abdiaziz Ibrahim Adam
 * - Date: 2024-11-18
 */

#ifndef DIRECTORY_USAGE_H
#define DIRECTORY_USAGE_H

#include "mdu.h"
#include "paths.h"
#include "threads.h"
#include "stack.h"

/**
 * @brief Calculates the disk usage of a specified path and its subdirectories.
 *
 * Determines if the path is a file or directory. For directories, recursively traverses
 * subdirectories and calculates disk usage. Adds the block count to the pool’s shared directory
 * size structure.
 *
 * @param thread_pool Pointer to the ThreadPool structure managing tasks.
 * @param current_path_task Pointer to the current Path being processed.
 * @return int Returns 0 on success, or non-zero on failure.
 */
int calculate_directory_usage(ThreadPool *thread_pool, Path *current_path_task);

/**
 * @brief Traverses a directory and enqueues subdirectories as tasks.
 *
 * Opens the specified directory, iterates over each entry, and processes it by either updating
 * the size (if it’s a file) or adding it to the thread pool’s task stack (if it’s a subdirectory).
 *
 * @param base_path Base directory path to traverse.
 * @param path_id Unique identifier of the path being processed.
 * @param thread_pool Pointer to the ThreadPool structure managing tasks.
 * @param directory_size Pointer to a long integer where the directory size will be accumulated.
 * @return int Returns 0 on success, or non-zero on failure.
 */
int traverse_directory(const char *base_path, int path_id, ThreadPool *thread_pool, long *directory_size);

/**
 * @brief Processes an individual directory entry, updating size or enqueuing if it’s a subdirectory.
 *
 * This function is used by `traverse_directory` to handle each entry within a directory.
 * It updates the size if the entry is a file and enqueues a new task for subdirectories.
 *
 * @param full_path Full path of the directory entry being processed.
 * @param path_id Unique identifier of the path being processed.
 * @param thread_pool Pointer to the ThreadPool structure managing tasks.
 * @param directory_size Pointer to a long integer where the entry's size will be accumulated.
 * @return int Returns 0 on success, or non-zero on failure.
 */
int process_path_entry(char *full_path, int path_id, ThreadPool *thread_pool, long *directory_size);

/**
 * @brief Updates the total size for a directory in the thread pool.
 *
 * This function adds the size of the current directory (in blocks) to the total size
 * stored in the `ThreadPool` structure for the specific directory path.
 *
 * @param thread_pool Pointer to the ThreadPool structure.
 * @param current_path_task Pointer to the current Path structure being processed.
 * @param directory_size The size (in blocks) of the directory to add.
 */
void update_directory_size(ThreadPool *thread_pool, Path *current_path_task, long directory_size);

#endif /* DIRECTORY_USAGE_H */
