#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <bits/getopt_core.h>
#include <sys/stat.h>
#include "paths.h"

/* ------------------------------- Function Implementations ------------------------------------------------------------------------------------- */

// Initializes paths within the thread pool
void initialize_paths(int path_num, char *argv[], ThreadPool *thread_pool) {
    Path *path_task;
    struct stat statbuf;

    for (int i = 0; i < path_num; i++) {
        // Check if the specified path exists
        if (stat(argv[i + optind], &statbuf) != 0) {
            perror(argv[i + optind]);  // Log error if path doesn't exist, continue
            continue;
        }

        // Create a path task and check for allocation success
        path_task = create_path(argv[i + optind], i);
        if (path_task == NULL) {
            perror("Failed to create path task");  // Log and continue if path task creation failed
            continue;
        }

        // Assign unique identifier and push task to the stack
        if (give_path_identity(thread_pool, path_task->path_name, i) != EXIT_SUCCESS) {
            cleanup_path(path_task);  // Cleanup and continue on failure
            perror("Failed to assign path identity");
            continue;
        }

        if (pthread_mutex_lock(&thread_pool->stack_mut) != 0) {
            handle_error("pthread_mutex_lock");  // Exit if mutex lock fails
        }
        push(thread_pool->stack, (char *)path_task);
        if (pthread_cond_signal(&thread_pool->cond) != 0) {
            handle_error("pthread_cond_signal");  // Exit if signaling fails
        }
        if (pthread_mutex_unlock(&thread_pool->stack_mut) != 0) {
            handle_error("pthread_mutex_unlock");  // Exit if mutex unlock fails
        }
    }

    // If no paths are available to process, clean up and exit
    if (is_empty(thread_pool->stack)) {
        cleanup_ThreadPool(thread_pool);
        handle_error("No valid paths to process");
    }
}

// Creates a new path structure
Path *create_path(char *path_name, int path_id) {
    // Allocate memory for the Path structure
    Path *path_task = calloc(1, sizeof(Path));
    if (path_task == NULL) {
        perror("Failed to allocate memory for Path");
        return NULL;
    }

    // Duplicate the path name for the Path structure
    char *new_path = strdup(path_name);
    if (new_path == NULL) {
        perror("Failed to duplicate path name");
        free(path_task);  // Cleanup path_task if strdup fails
        return NULL;
    }

    // Assign fields to the path_task structure
    path_task->path_name = new_path;
    path_task->path_id = path_id;

    return path_task;
}

// Assigns a unique identity to each path
int give_path_identity(ThreadPool *Pool, char *path_name, int path_id) {
    char *new_id;
    if (path_name == NULL) {
        free(Pool->dir_details->path_identity[path_id]);
        Pool->dir_details->path_identity[path_id] = NULL;
        return EXIT_SUCCESS;
    }
    if ((new_id = strdup(path_name)) == NULL) {
        perror("Failed to duplicate path name for identity assignment");
        Pool->dir_details->path_identity[path_id] = NULL;
        return EXIT_FAILURE;
    }
    Pool->dir_details->path_identity[path_id] = new_id;
    return EXIT_SUCCESS;
}

// Concatenates base path and filename into a full path
char *concatenate_dir_path(const char *base_path, const char *filename) {
    size_t len_base = strlen(base_path);
    size_t len_file = strlen(filename);

    // Allocate memory for the concatenated path
    char *full_path = malloc(len_base + len_file + 2);  // +1 for '/' +1 for '\0'
    if (!full_path) {
        perror("Failed to allocate memory for full path");
        return NULL;
    }

    // Use snprintf to format the string safely
    snprintf(full_path, len_base + len_file + 2, "%s/%s", base_path, filename);

    return full_path;
}

// Cleans up memory allocated for a Path structure
void cleanup_path(Path *path_task) {
    free(path_task->path_name);
    free(path_task);
}

/* --------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */
