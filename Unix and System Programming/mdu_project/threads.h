/**
 * @file threads.h
 * @brief Provides functions for initializing and managing threads in the ThreadPool structure.
 *
 * This module handles the creation, initialization, and management of threads within a thread pool.
 * It includes functions for setting up a thread pool, dispatching threads, and coordinating their 
 * activity for concurrent processing of directory and file paths. It also provides cleanup functions 
 * to properly release resources when threads complete their tasks.
 *
 * @section Author
 * - Author: Abdiaziz Ibrahim Adam
 * - Date: 2024-11-18
 */

#ifndef THREADS_H
#define THREADS_H

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
#include "paths.h"
#include "directory_usage.h"

/* ------------------------------- Function Prototypes ------------------------------------------------------------------------------------------------ */

/**
 * @brief Initializes the ThreadPool structure, setting up paths and threading components.
 *
 * Allocates memory for the `ThreadPool` and its components, including mutexes, condition variables,
 * and the stack for task management. The thread pool is essential for handling concurrent 
 * directory traversal.
 *
 * @param num_paths Number of paths to process.
 * @param num_threads Number of threads to initialize in the pool.
 * @return ThreadPool* A pointer to the initialized ThreadPool, or NULL if allocation fails.
 *
 * @note It is the caller's responsibility to free allocated memory by calling `cleanup_ThreadPool`
 * after all threads are done processing.
 */
ThreadPool *init_ThreadPool(int num_paths, int num_threads);

/**
 * @brief Dispatches threads in the pool to start processing tasks from the stack.
 *
 * This function creates the specified number of threads and assigns them to process tasks
 * in the stack concurrently. The threads work until all paths are processed or an error occurs.
 *
 * @param num_threads Number of threads to launch.
 * @param thread_id A pointer to an array that stores thread identifiers.
 * @param Pool Pointer to the ThreadPool managing tasks.
 *
 * @note The caller should invoke `join_threads` after initializing threads to ensure
 * proper cleanup.
 */
void initialize_dispatch_threads(int num_threads, pthread_t **thread_id, ThreadPool *thread_pool);

/**
 * @brief Function executed by each worker thread in the pool.
 *
 * Each thread repeatedly pulls tasks from the stack and processes paths, handling files
 * and directories until no tasks remain. It synchronizes access to shared resources
 * and exits when processing is complete.
 *
 * @param args A pointer to the ThreadPool managing the thread’s tasks.
 * @return void* Returns NULL upon completion.
 */
void *thread_worker(void *args);

/**
 * @brief Joins threads after all tasks are completed.
 *
 * This function waits for each thread to finish its task, ensuring all resources
 * are properly released. It is essential for preventing resource leaks after
 * multithreaded execution.
 *
 * @param num_threads Number of threads to join.
 * @param thread_id Array containing the identifiers of the threads to join.
 */
void join_threads(int num_threads, pthread_t *thread_id);

/**
 * @brief Cleans up the ThreadPool, freeing allocated memory and resources.
 *
 * Deallocates memory for the `ThreadPool` structure and its components, including
 * mutexes, condition variables, and stack resources. This function should be called 
 * once all threads have completed processing.
 *
 * @param Pool Pointer to the ThreadPool to be cleaned up.
 */
void cleanup_ThreadPool(ThreadPool *Pool);

#endif /* THREADS_H */
