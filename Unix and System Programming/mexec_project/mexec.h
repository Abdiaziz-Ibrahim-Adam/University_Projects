/**
 * @mainpage 
 *
 * @section Introduction
 * This program implements a pipeline executor where the output of one command 
 * becomes the input for the next command.
 * 
 * @section Author
 *  - Author: Abdiaziz Ibrahim Adam
 * 
 * @since October 2024
 *
 * @section files File Structure
 *
 * The program is organized into multiple files, each containing related functionalities:
 * - `mexec.h`: Function prototypes and structure definitions for the program.
 * - `mexec.c`: Implements the core functionality of the pipeline executor.
 */

/**
 * @defgroup mexec Pipeline Executor Functions
 * @brief Functions and structures for pipeline execution.
 * @{
 */

#ifndef MEXEC_H
#define MEXEC_H

#include <stdio.h>

/**
 * @brief Reads commands from a file or standard input and stores them in an array.
 * 
 * @warning This function allocates memory for command arguments. 
 * The caller is responsible for deallocating this memory using `free_memory_for_commands`.
 * 
 * @param command_arguments Array of command strings.
 * @param numCommands Pointer to an integer to store the number of commands.
 * @param in_file File pointer to read commands from.
 * @param capacity Pointer to an integer to store the capacity of the command array.
 * @return Pointer to the updated command arguments array.
 */
char **read_commandlines(char **command_arguments, int *numCommands, FILE *in_file, int *capacity);

/**
 * @brief Parses a command string into an array of arguments for execvp().
 * 
 * @warning This function allocates memory for the parsed arguments array.
 * The caller is responsible for freeing this memory using `free_memory_for_commands`.
 * 
 * @param command Command string to parse.
 * @return Pointer to the array of arguments.
 */
char **parse_command(char *command);

/**
 * @brief Executes a series of commands using pipes.
 * 
 * @param numCommands Number of commands to execute.
 * @param command_arguments Array of command strings.
 */
void execute_commands(int numCommands, char **command_arguments);

/**
 * @brief Creates pipes for inter-process communication.
 * 
 * @warning This function allocates memory for the pipes array. 
 * The caller is responsible for deallocating this memory using `free_pipes`.
 * 
 * @param numCommands Number of commands, which determines the number of pipes.
 * @return A pointer to an array of pipes.
 */
int **create_pipes(int numCommands);

/**
 * @brief Closes all pipes in a process.
 * 
 * @param pipes Array of pipes.
 * @param numCommands Number of commands, which determines the number of pipes.
 */
void close_pipes(int **pipes, int numCommands);

/**
 * @brief Forks processes and executes the commands in the pipeline.
 * 
 * @param numCommands Number of commands to execute.
 * @param command_arguments Array of command strings.
 * @param pipes Array of pipes used for communication between processes.
 */
void fork_and_execute(int numCommands, char **command_arguments, int **pipes);

/**
 * @brief Waits for child processes to finish and cleans up resources.
 * 
 * @warning This function frees the memory allocated for pipes and command arguments.
 * The caller should not attempt to use or free these resources after this function is called.
 * 
 * @param pipes Array of pipes used for communication between processes.
 * @param numCommands Number of commands executed.
 * @param command_arguments Array of command strings.
 */
void wait_for_children_and_cleanup(int **pipes, int numCommands, char **command_arguments);

/**
 * @brief Frees the memory allocated for the pipes.
 * 
 * @param pipes Array of pipes.
 * @param numCommands Number of commands, which determines the number of pipes.
 */
void free_pipes(int **pipes, int numCommands);

/**
 * @brief Frees the memory allocated for the array of command strings.
 * 
 * @param numCommands Number of commands.
 * @param command_arguments Array of command strings.
 */
void free_memory_for_commands(int numCommands, char **command_arguments);

/**
 * @brief Handles errors by printing an error message and exiting the program.
 * 
 * @param msg Error message to print.
 */
void handle_error(const char *msg);

#endif /* MEXEC_H */

/** @} */
