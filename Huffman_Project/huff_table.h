/**
 * @defgroup huff_table.h Huffman Table Generation and Management
 * @brief Provides functionality to generate a Huffman table from a Huffman trie and to free the allocated table.
 * 
 * This module is responsible for creating a Huffman table that maps each byte value to its corresponding Huffman code
 * and ensuring the proper management of memory allocated for the table. Users of this module are responsible for freeing
 * the Huffman table using the provided function to prevent memory leaks.
 * 
 * @warning The user is responsible for deallocating the Huffman table to prevent memory leaks. Failure to do so may result
 * in undefined behavior. Additionally, accessing uninitialized parts of the table or attempting to free the table more
 * than once may also lead to undefined behavior.
 * 
 * @section Author
 * - Author: Abdiaziz Ibrahim Adam
 * @since Datum
 * - 18 March 2024
 * @{
 */

#ifndef HUFF_TABLE_H
#define HUFF_TABLE_H

#include "Huff_Trie.h" // Include the Trie structure definition
#include "bit_buffer.h" // Include the bit buffer structure definition

/**
 * @brief Generates a Huffman table from a Huffman trie.
 * 
 * This function traverses a given Huffman trie to generate a table mapping byte values to Huffman codes.
 * The table is dynamically allocated and must be freed by the caller to avoid memory leaks.
 * 
 * @param root Pointer to the root node of the Huffman trie.
 * @return A dynamically allocated array of strings where each string represents the Huffman code for
 *         the corresponding byte value. The caller is responsible for freeing this table.
 */
char **huff_table(Trie *root);

/**
 * @brief Frees the memory allocated for the Huffman table.
 * 
 * This function safely deallocates the memory used by the Huffman table and its contents. It should be
 * called when the table is no longer needed to prevent memory leaks.
 * 
 * @param huffmanTable Pointer to the Huffman table to be freed.
 */
void free_huff_table(char **huffmanTable);

#endif // HUFF_TABLE_H

/** @} */

