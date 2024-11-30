/**
 * @defgroup FrequencyAnalysis
 * @brief Functions for performing frequency analysis on input files.
 * 
 * @section Author
 *  - Author: Abdiaziz Ibrahim Adam
 * @since Datum
 *  -  18 March 2024
 * @{
 */

#ifndef FREQUENCY_TABLE_H
#define FREQUENCY_TABLE_H

#include <stdio.h>

/**
 * @brief Create a frequency table for bytes in the input file.
 *
 * This function reads the input file byte by byte and increments the frequency count for each byte encountered. 
 * The function allocates memory for a frequency table of 256 integers (assuming an 8-bit byte and hence 256 possible byte values),
 * initializing each to zero. It is the caller's responsibility to free this memory when it is no longer needed to avoid memory leaks.
 *
 * Note: If the file pointer is NULL or points to a closed file, the behavior of this function is undefined. Ensure the file is properly opened before calling this function.
 * 
 *
 * @param file Pointer to a FILE structure representing the input file. Must not be NULL.
 * @return A pointer to an array representing the frequency of each byte. The index represents
 *         the byte value (0-255), and the value at each index represents the frequency of that byte.
 *         The array is allocated dynamically and must be freed by the caller.
 */
int *create_frequency_table(FILE *file);

#endif /* FREQUENCY_TABLE_H */

/** @} */
