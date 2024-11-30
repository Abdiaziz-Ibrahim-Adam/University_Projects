/**
 * @mainpage Huffman Program
 *
 * @section introduction Introduction
 * This program implements Huffman encoding and decoding algorithms. Huffman encoding is a technique used for lossless data compression, 
 * where frequently occurring characters are represented using shorter codes, while less frequent characters are represented using longer codes.
 * 
 *
 * @section program_summary Program Summary and File Structures
 * The program consists of several modules, each responsible for different functionalities:
 * 
 * - Frequency Analysis: Performs frequency analysis on the input file to build a frequency table.
 * - Huffman Tree Construction: Constructs a Huffman tree based on the frequency table.
 * - Encoding: To encode a file, the program traverses the Huffman tree to assign a unique binary code to each character based on its path in the tree. 
 *   The input file is then read character by character, and each character is replaced with its corresponding binary code, resulting in a compressed output file.
 * - Decoding: The decoding process involves reading the binary codes from the encoded file and traversing the Huffman tree to find the corresponding characters. 
 *   This reconstructs the original file from its compressed form.
 * 
 * The program is organized into multiple files, each containing related functionalities:
 * 
 * - "huffman.h"           : Defines the interfaces for the Huffman encoding and decoding process.
 * - "huffman.c"           : Implements the functions defined in "huffman.h", coordinating the encoding and decoding processes.
 * - "frequency_table.h"   : Header file for the frequency analysis module. Defines structures and functions for building the frequency table.
 * - "frequency_table.c"   : Implements frequency analysis to generate a frequency table from the input data.
 * - "huffman_tree.h"      : Header file for the Huffman tree construction module. Contains definitions for the tree and node structures.
 * - "huffman_tree.c"      : Implements the logic for constructing the Huffman tree based on the frequency table.
 * - "encoding_decoding.h" : Defines interfaces for encoding and decoding functions, tying together the Huffman tree and bit buffer operations.
 * - "encoding_decoding.c" : Implements the core logic for converting input data into encoded format and vice versa.
 *
 * @section datatypes Datatypes
 *
 * In this Huffman encoding and decoding project, several key data types are used to efficiently process and manage data. 
 * These include both standard C data types and custom structures designed specifically for the project's needs. 
 * Below is a description of these data types and their roles within the project.
 *
 * - FILE           : A standard C data type used for file operations. It is used to read the input file that will be encoded or decoded, 
 *                    and to write the output file after processing.
 * - int            : A standard C data type for integers. It is used to represent frequencies of characters in the source data, the size of data structures, 
 *                    and as return values for functions indicating success or failure.
 * - unsigned char  : Represents individual bytes of data. This is crucial for encoding and decoding processes, as Huffman coding operates at the bit level within bytes.
 * - HuffmanNode    : A structure representing a node in the Huffman tree. Each node may represent a character (in leaf nodes) or a combination of two child nodes. 
 *                    It includes fields for character frequency, the character itself (for leaf nodes), and pointers to left and right child nodes.
 *
 * - PriorityQueue  : A custom data type used to efficiently select the two lowest-frequency nodes during the Huffman tree construction. 
 *                    It ensures that the tree is built in a way that assigns shorter codes to more frequent characters.
 * 
 * - BitBuffer      : A structure designed to manage the bit-level operations required for encoding and decoding. 
 *                    It allows for the dynamic collection of bits into bytes and vice versa,facilitating the compression and decompression processes.
 *
 * - HuffmanTree    : Represents the Huffman tree itself, constructed from the HuffmanNode structures. 
 *                    It is used both to generate the Huffman codes for each character and to decode encoded data back into its original form.
 *
 * These data types are fundamental to the functioning of the Huffman encoding and decoding algorithms, providing the necessary structures for frequency analysis, 
 * tree construction, and the encoding/decoding processes.
 *
 * @section acknowledgments Acknowledgments
 * Special thanks to the course instructors Jonny Pettersson, Lars Karlsson, and Sebastian Sandberg for providing essential utility modules such as (bit_buffer, list, and pqueue. 
 * These modules play critical roles in the Huffman encoding and decoding process:
 * 
 * Bit Buffer      : This module is instrumental in managing bits for both encoding and decoding operations. During encoding, 
 *                   it collects individual bits of Huffman codes and compiles them into bytes for efficient storage. In decoding, 
 *                   it aids in reading encoded bits from the compressed file, facilitating the reconstruction of the original data.
 * 
 * List            : The List module underpins the Priority Queue, serving as the foundational data structure. It efficiently manages the nodes within the priority queue, 
 *                   allowing for dynamic insertions and deletions as the Huffman tree is constructed. This flexibility is crucial for maintaining the queue's order based on,
 *                   node frequencies.
 * 
 * Priority Queue  : Essential for the Huffman tree construction, the Priority Queue module uses the List to organize tree nodes by their frequencies. 
 *                   It ensures that nodes with lower frequencies are combined first, leading to an optimal Huffman tree that minimizes the overall length of encoded data.
 * 
 * The integration of these modules into the Huffman project facilitates critical functionalities such as bit-level data handling, dynamic data structuring, 
 * and efficient prioritization in tree construction. Their usage exemplifies the application of advanced data structures and algorithms in implementing, 
 * effective compression techniques.
 * 
 * @section Author
 *  - Author: Abdiaziz Ibrahim Adam
 * @since Datum
 *  -  18 March 2024
 */

/**
 * @defgroup Huffman
 * @brief Interfaces and structures for Huffman encoding and decoding.
 * 
 * This module contains the definitions and interfaces for the Huffman encoding and
 * decoding process, including file handling, frequency analysis, and the Huffman tree
 * construction and manipulation.
 * @{
 */

#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdio.h> 
#include "frequency_table.h"
#include "Huff_Trie.h"
#include "huff_table.h"
#include "encode_decode.h"

/**
 * @brief Structure to hold file pointers for input and output files.
 * 
 * This structure contains file pointers for the frequency analysis file,
 * the input file to be encoded or decoded, and the output file where
 * the results are stored.
 */
typedef struct files {
    FILE *in_frequency_file; ///< File pointer for the input frequency analysis file.
    FILE *in_file;           ///< File pointer for the input file to encode/decode.
    FILE *out_file;          ///< File pointer for the output file where the result is stored.
} files;

/**
 * @brief Check program parameters and open necessary files.
 *
 * This function checks the command-line arguments passed to the program and
 * opens the required files for Huffman encoding or decoding. It ensures that
 * the correct number of arguments are provided and that the specified files
 * can be opened successfully.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of strings containing the command-line arguments.
 * @param file A pointer to a structure holding file pointers for input and output operations.
 * @return An integer indicating success (0) or failure (-1).
 */
int validate_program_arguments(int argc, const char *argv[], files *file);

/**
 * @brief Displays an error message.
 *
 * This function is called when an error occurs within the Huffman encoding or decoding process. and if files can not be opened.
 * It provides a generic error message to the user.
 *
 * @return None.
 */
void error_message(void);

#endif /* HUFFMAN_H */

/** @} */
