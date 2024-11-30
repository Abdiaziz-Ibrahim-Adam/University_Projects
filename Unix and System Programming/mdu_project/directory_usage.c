#include "directory_usage.h"

/* ------------------------------- Function Implementations ------------------------------------------------------------------------------------------------ */

int calculate_directory_usage(ThreadPool *thread_pool, Path *current_path_task) {
    int exit_code = EXIT_SUCCESS;
    char *base_path = current_path_task->path_name;
    struct stat s;

    // Get base path stats
    if (lstat(base_path, &s) < 0) {
        perror(base_path);
        return EXIT_FAILURE; 
    }

    long directory_size = s.st_blocks;

    // If it’s a directory, traverse it
    if (S_ISDIR(s.st_mode)) {
        int traversal_result = traverse_directory(base_path, current_path_task->path_id, thread_pool, &directory_size);
        if (traversal_result != 0) {
            exit_code = EXIT_FAILURE; 
        }
    }

    // Update the shared directory size
    update_directory_size(thread_pool, current_path_task, directory_size);

    return exit_code; // Return status indicating success or failure
}

int traverse_directory(const char *base_path, int path_id, ThreadPool *thread_pool, long *directory_size) {
    DIR *directory = opendir(base_path);
    if (!directory) {
        fprintf(stderr, "du: cannot read directory '%s': %s\n", base_path, strerror(errno));
        return EXIT_FAILURE; 
    }

    struct dirent *directory_entry;
    while ((directory_entry = readdir(directory)) != NULL) {
        if (strcmp(directory_entry->d_name, ".") == 0 || strcmp(directory_entry->d_name, "..") == 0) {
            continue;
        }

        char *full_path = concatenate_dir_path(base_path, directory_entry->d_name); 
        if (!full_path) {
            closedir(directory); 
            return EXIT_FAILURE; 
        }

        // Process each path entry
        if (process_path_entry(full_path, path_id, thread_pool, directory_size) != 0) {
            free(full_path); 
            closedir(directory); 
            return EXIT_FAILURE; 
        }

        free(full_path); 
    }

    closedir(directory); 
    return EXIT_SUCCESS; 
}

int process_path_entry(char *full_path, int path_id, ThreadPool *thread_pool, long *directory_size) {
    struct stat s;
    if (lstat(full_path, &s) < 0) {
        fprintf(stderr, "du: cannot open directory '%s': %s\n", full_path, strerror(errno));  // Print error if lstat fails
        return EXIT_FAILURE;
    }

    // Add size if it’s a file
    if (!S_ISDIR(s.st_mode)) {
        *directory_size += s.st_blocks;
    } else {
        // Add subdirectory as a new task
        Path *new_subdir_task = create_path(full_path, path_id);
        if (!new_subdir_task) {
            return EXIT_FAILURE;  // Return failure if path creation fails
        }

        if (pthread_mutex_lock(&thread_pool->stack_mut) != 0) {
            perror("pthread_mutex_lock failed");  
            cleanup_path(new_subdir_task);  
            return EXIT_FAILURE;
        }
        
        push(thread_pool->stack, new_subdir_task);  // Push new subdir task

        // Signal condition variable 
        if (pthread_cond_signal(&thread_pool->cond) != 0) {
            perror("pthread_cond_signal failed");
            cleanup_path(new_subdir_task);
            pthread_mutex_unlock(&thread_pool->stack_mut);  // Unlock mutex before returning
            return EXIT_FAILURE;
        }

        if (pthread_mutex_unlock(&thread_pool->stack_mut) != 0) {
            perror("pthread_mutex_unlock failed");  // Handle mutex unlock error
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}

void update_directory_size(ThreadPool *thread_pool, Path *current_path_task, long directory_size) {
    // Lock mutex for thread-safe update
    if (pthread_mutex_lock(&thread_pool->dir_details->size_count_mut[current_path_task->path_id]) != 0) {
        perror("pthread_mutex_lock failed");  // Handle mutex lock error
        return;
    }
    
    thread_pool->dir_details->dir_size[current_path_task->path_id] += directory_size;

    // Unlock mutex after update
    if (pthread_mutex_unlock(&thread_pool->dir_details->size_count_mut[current_path_task->path_id]) != 0) {
        perror("pthread_mutex_unlock failed");  // Handle mutex unlock error
    }
}

/* --------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */