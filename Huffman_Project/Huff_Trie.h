/**
 * @defgroup HuffTrie
 * @brief Functions for Huffman Tree construction and manipulation.
 * 
 * This module provides the functionalities required for creating a Huffman tree, 
 * which is central to the Huffman encoding and decoding process. The tree is constructed 
 * using frequency data of input characters, allowing for efficient compression based on character 
 * frequency.
 * 
 * @section Author
 *  - Author: Abdiaziz Ibrahim Adam
 * @since Datum
 *  - 18 March 2024
 * @{
 */

#ifndef TRIE_H
#define TRIE_H

#include <stdio.h>
#include <stdbool.h>
#include "pqueue.h"
#include "bit_buffer.h"

/**
 * @brief Structure representing a node in the Huffman tree.
 * 
 * Each node represents either a leaf node with a character (byte) and its frequency (weight)
 * or an internal node with combined weights of its child nodes.
 */
typedef struct Trie {
    int weight;  ///< The frequency of the character or sum of frequencies for internal nodes.
    int byte;    ///< The character value for leaf nodes, -1 for internal nodes.
    struct Trie *left_child, *right_child; ///< Pointers to left and right child nodes.
} Trie;

/**
 * @brief Creates a new Trie node.
 * 
 * Allocates memory and initializes a new Trie node with the given weight and byte value.
 * For leaf nodes, byte holds the character's value; for internal nodes, byte should be -1.
 * 
 * @warning Memory allocation: It's the caller's responsibility to free allocated memory.
 * @warning Undefined behavior may occur if memory allocation fails.
 *
 * @param weight The frequency of the character or sum of frequencies for internal nodes.
 * @param byte The character value for leaf nodes, -1 for internal nodes.
 * @return Pointer to the newly created Trie node, or NULL if memory allocation fails.
 */
Trie *trie_create(int weight, int byte); 

/**
 * @brief Combines two Trie nodes into a new parent node.
 * 
 * Creates a new internal Trie node with children `left` and `right`. The new node's weight
 * is the sum of the children's weights. This function is used during Huffman tree construction
 * to combine nodes with the lowest frequencies.
 * 
 * @warning Memory allocation: It's the caller's responsibility to free allocated memory.
 *
 * @param left Pointer to the left child node.
 * @param right Pointer to the right child node.
 * @return Pointer to the new parent node combining `left` and `right`.
 */
Trie *trie_combine(Trie *left, Trie *right);

/**
 * @brief Builds the Huffman tree from a frequency table.
 * 
 * Constructs the Huffman tree using a frequency table of characters. The tree is built by 
 * creating leaf nodes for each character and combining the nodes with the lowest frequencies
 * until only one node remains - the root of the Huffman tree.
 * 
 * @warning Memory allocation: It's the caller's responsibility to free allocated memory.
 *
 * @param frequency_table Array of 256 integers representing the frequency of each byte/character.
 * @return Pointer to the root of the constructed Huffman tree.
 */
Trie *build_huff_trie(int *frequency_table);

/**
 * @brief Recursively frees memory allocated for the Huffman tree.
 * 
 * Traverses the Huffman tree starting from the given node, freeing memory for each node.
 * This function should be called on the root node of the Huffman tree to free all allocated memory.
 *
 * @param trie Pointer to the Trie node to start freeing memory from.
 */
void trie_kill(Trie *trie);

#endif /* TRIE_H */

/** @} */
