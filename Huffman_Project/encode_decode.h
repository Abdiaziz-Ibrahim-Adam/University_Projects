/**
 * @defgroup EncodingDecoding
 * @brief Functions for encoding and decoding files using Huffman coding.
 * 
 * This module provides the functionality to encode a file into a compressed format using Huffman coding, 
 * and to decode a previously encoded file back to its original format. It utilizes Huffman trees for encoding and decoding processes.
 * 
 * @section Author
 *  - Author: Abdiaziz Ibrahim Adam
 * @since Datum
 *  - 18 March 2024
 * @{
 */

#ifndef ENCODE_DECODE_H
#define ENCODE_DECODE_H

#include "bit_buffer.h" 
#include "Huff_Trie.h"  

/**
 * @brief Encodes an input file using Huffman codes and writes the encoded data to an output file.
 * 
 * This function reads each character from the input file, looks up its corresponding Huffman code in the provided
 * Huffman table, and writes the encoded bits to the output file. The encoding process uses a bit buffer to manage the
 * bit-level operations required for writing encoded data.
 * 
 * Memory Management: The function allocates a bit buffer for encoding purposes and it is users responsiblity to free this buffer before returning.
 * 
 * @note It's crucial that the Huffman table contains valid Huffman codes for every character that may appear in the input file.
 * 
 * @param input Pointer to a FILE structure for the input file. Must be opened in read mode.
 * @param output Pointer to a FILE structure for the output file. Must be opened in write mode.
 * @param huffmanTable An array of strings where each index corresponds to a character (byte) and each string represents
 *        the Huffman code for that character.
 */
void encode_file(FILE *input, FILE *output, char **huffmanTable);

/**
 * @brief Decodes an encoded file using a Huffman tree and writes the decoded data to an output file.
 * 
 * This function reads the encoded data from the input file bit by bit, traverses the provided Huffman tree according
 * to these bits, and writes the decoded characters to the output file. When the traversal reaches a leaf node,
 * the corresponding character is written out, and the traversal restarts from the root for the next series of bits.
 * 
 * Memory Management: The function allocates a bit buffer to assist in the decoding process and it is user's responsiblity to free this buffer before returning.
 * 
 * @note The function expects the input file to be encoded according to the provided Huffman tree. If the encoded data
 * does not match the Huffman tree structure, the decoding process may result in undefined behavior.
 * 
 * @param input Pointer to a FILE structure for the input file, containing encoded data. Must be opened in read mode.
 * @param output Pointer to a FILE structure for the output file where the decoded data will be written. Must be opened in write mode.
 * @param huffmanTree A pointer to the root of the Huffman tree used for decoding.
 */
void decode_file(FILE *input, FILE *output, Trie *huffmanTree, char **huffmanTable) ;

#endif /* ENCODE_DECODE_H */

/** @} */
