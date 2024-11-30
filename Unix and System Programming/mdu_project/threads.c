#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "threads.h"

/* ------------------------------- Function Implementations ------------------------------------------------------------------------------------------------ */

ThreadPool *init_ThreadPool(int path_num, int num_threads) {
    Stack *stack = create_stack();
    if (stack == NULL) {
        handle_error("Failed to create stack");  // Exit if stack creation fails
    }

    // Allocate memory for the ThreadPool structure
    ThreadPool *thread_pool = calloc(1, sizeof(ThreadPool));
    if (thread_pool == NULL) {
        handle_error("Failed to allocate memory for ThreadPool");  // Exit on memory allocation failure
        free_stack(stack);
    }

    // Allocate memory for the Dir_details structure within the pool
    Dir_details *dir_details = calloc(1, sizeof(Dir_details));
    if (dir_details == NULL) {
        handle_error("Failed to allocate memory for Dir_details");
        free_stack(stack);
        free(thread_pool);
    }

    // Allocate memory for dir_size, path_identity, and size_count_mut arrays
    dir_details->dir_size = calloc(path_num, sizeof(long));
    dir_details->path_identity = calloc(path_num, sizeof(char *));
    dir_details->size_count_mut = calloc(path_num, sizeof(pthread_mutex_t));

    if (!dir_details->dir_size || !dir_details->path_identity || !dir_details->size_count_mut) {
        handle_error("Failed to allocate memory for Dir_details fields");
        free(dir_details->dir_size);
        free(dir_details->path_identity);
        free(dir_details->size_count_mut);
        free(dir_details);
        free_stack(stack);
        free(thread_pool);
    }

    // Initialize ThreadPool fields
    thread_pool->stack = stack;
    thread_pool->dir_details = dir_details;
    thread_pool->path_num = path_num;
    thread_pool->num_threads = num_threads;
    thread_pool->exit_code = EXIT_SUCCESS;

    // Initialize mutexes and condition variables in the ThreadPool
    if (pthread_mutex_init(&thread_pool->stack_mut, NULL) != 0 ||
        pthread_mutex_init(&thread_pool->exit_code_mut, NULL) != 0 ||
        pthread_cond_init(&thread_pool->cond, NULL) != 0) {
        handle_error("Failed to initialize thread pool synchronization primitives");
    }

    // Initialize each mutex in the size_count_mut array
    for (int i = 0; i < path_num; i++) {
        if (pthread_mutex_init(&dir_details->size_count_mut[i], NULL) != 0) {
            handle_error("Failed to initialize mutex for size_count_mut array");
        }
    }

    return thread_pool;
}

// Initializes and dispatches threads in the thread pool
void initialize_dispatch_threads(int num_threads, pthread_t **thread_id, ThreadPool *thread_pool) {
    *thread_id = calloc(num_threads - 1, sizeof(pthread_t));
    if (*thread_id == NULL) {
        handle_error("Failed to allocate memory for thread IDs");
        cleanup_ThreadPool(thread_pool);
    }

    // Create worker threads, with error handling for thread creation failures
    for (int i = 0; i < num_threads - 1; i++) {
        int s = pthread_create(&(*thread_id)[i], NULL, &thread_worker, thread_pool);
        if (s != 0) {
            handle_error("pthread_create failed");
        }
    }

    // Run the main thread as a worker
    thread_worker(thread_pool);
}

// Worker function for each thread in the pool
void *thread_worker(void *args) {
    ThreadPool *thread_pool = (ThreadPool *)args;

    static int waiting_threads_count = 0;
    static bool work_finished = false;
    static bool error_occurred = false;

    if (pthread_mutex_lock(&thread_pool->stack_mut) != 0) {
        handle_error("pthread_mutex_lock failed");
    }

    while (!work_finished) {
        if (is_empty(thread_pool->stack)) {
            waiting_threads_count++;
            if (waiting_threads_count == thread_pool->num_threads) {
                work_finished = true;
                if (pthread_cond_broadcast(&thread_pool->cond) != 0) {
                    handle_error("pthread_cond_broadcast failed");
                }
                if (pthread_mutex_unlock(&thread_pool->stack_mut) != 0) {
                    handle_error("pthread_mutex_unlock failed");
                }
                break;
            }

            if (pthread_cond_wait(&thread_pool->cond, &thread_pool->stack_mut) != 0) {
                handle_error("pthread_cond_wait failed");
            }
            waiting_threads_count--;

            if (work_finished) {
                if (pthread_mutex_unlock(&thread_pool->stack_mut) != 0) {
                    handle_error("pthread_mutex_unlock failed");
                }
                break;
            }
        }
        if (!is_empty(thread_pool->stack)) {
            Path *current_task = (Path *)pop(thread_pool->stack);
            if (pthread_mutex_unlock(&thread_pool->stack_mut) != 0) {
                handle_error("pthread_mutex_unlock failed");
            }

            if (calculate_directory_usage(thread_pool, current_task) && !error_occurred) {
                if (pthread_mutex_lock(&thread_pool->exit_code_mut) != 0) {
                    handle_error("pthread_mutex_lock failed");
                }
                error_occurred = true;
                thread_pool->exit_code = EXIT_FAILURE;
                if (pthread_mutex_unlock(&thread_pool->exit_code_mut) != 0) {
                    handle_error("pthread_mutex_unlock failed");
                }
            }

            cleanup_path(current_task);

            if (pthread_mutex_lock(&thread_pool->stack_mut) != 0) {
                handle_error("pthread_mutex_lock failed");
            }
        }
    }
    return NULL;
}

// Joins threads in the thread pool
void join_threads(int num_threads, pthread_t *thread_id) {
    for (int i = 0; i < num_threads - 1; i++) {
        if (pthread_join(thread_id[i], NULL) != 0) {
            handle_error("pthread_join failed");
        }
    }
    free(thread_id);
}

// Cleans up all resources allocated for the ThreadPool
void cleanup_ThreadPool(ThreadPool *Pool) {
    free(Pool->dir_details->dir_size);

    for (int i = 0; i < Pool->path_num; i++) {
        free(Pool->dir_details->path_identity[i]);
    }
    free(Pool->dir_details->path_identity);

    while (!is_empty(Pool->stack)) {
        Path *t = pop(Pool->stack);
        cleanup_path(t);
    }
    free_stack(Pool->stack);

    if (pthread_cond_destroy(&Pool->cond) != 0) {
        handle_error("pthread_cond_destroy failed");
    }
    if (pthread_mutex_destroy(&Pool->stack_mut) != 0) {
        handle_error("pthread_mutex_destroy failed");
    }
    if (pthread_mutex_destroy(&Pool->exit_code_mut) != 0) {
        handle_error("pthread_mutex_destroy failed");
    }
    for (int i = 0; i < Pool->path_num; i++) {
        if (pthread_mutex_destroy(&Pool->dir_details->size_count_mut[i]) != 0) {
            handle_error("pthread_mutex_destroy failed");
        }
    }
    free(Pool->dir_details->size_count_mut);

    free(Pool->dir_details);
    free(Pool);
}

/* --------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */
