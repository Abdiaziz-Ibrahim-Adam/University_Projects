/*
 * Programming in C
 * Spring 2024
 *
 * File:         encode_decode.c
 * Description:  Provides an implementation for the Huffman encoding and decoding process. 
 *               This file contains functions for encoding input files using Huffman codes,
 *               generating Huffman trees based on frequency analysis, and decoding encoded files
 *               back to their original content using the generated Huffman tree.
 * 
 * Author:       Abdiaziz Ibrahim Adam
 * CS username:  dv23aam
 * Date:         18 March 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bit_buffer.h"
#include "Huff_Trie.h"
#include "encode_decode.h"

void encode_file(FILE *input, FILE *output, char **huffmanTable) 
{
    bit_buffer *buffer = bit_buffer_empty();
    int c;
    long input_size = 0;
    long output_size = 0;

    fseek(input, 0, SEEK_END);
    input_size = ftell(input);
    rewind(input); 
    

    // Encode all characters from input
    while ((c = fgetc(input)) != EOF) {
        char *code = huffmanTable[c];
        for (int i = 0; code[i] != '\0'; i++) {
            bit_buffer_insert_bit(buffer, code[i] == '1');
        }
    }

    // Encode EOT symbol
    char *eotCode = huffmanTable[4]; // Assuming '4' is the index for EOT in your table
    for (int i = 0; eotCode[i] != '\0'; i++) {
        bit_buffer_insert_bit(buffer, eotCode[i] == '1');
    }

    while (bit_buffer_size(buffer) > 0) {
        if (bit_buffer_size(buffer) < 8) {
            // Pading with zeros to complete the final byte
            while (bit_buffer_size(buffer) < 8) {
                bit_buffer_insert_bit(buffer, 0);
            }
        }
        char byte = bit_buffer_remove_byte(buffer);
        fputc(byte, output);
        output_size++;
    }
    printf("\n%ld bytes read from input file.\n", input_size);
    printf("%ld bytes used in encoded form.\n\n", output_size);

    bit_buffer_free(buffer);
}

void decode_file(FILE *input, FILE *output, Trie *huffmanTree, char **huffmanTable) 
{
    Trie *current = huffmanTree;
    bit_buffer *buffer = bit_buffer_empty();

    int c;
    while ((c = fgetc(input)) != EOF) {
        for (int i = 7; i >= 0; --i) {
            bit_buffer_insert_bit(buffer, (c >> i) & 1);
        }
    }

    // Decoding until EOT is encountered
    bool foundEOT = false;
    while (bit_buffer_size(buffer) > 0 && !foundEOT) {
        if (current->left_child == NULL && current->right_child == NULL) { // Leaf node
            if (strcmp(huffmanTable[current->byte], huffmanTable[4]) == 0) { // Check if it's EOT
                foundEOT = true;
                break;
            } else {
                fputc(current->byte, output); // Write decoded character
                current = huffmanTree; // Reset to root
            }
        }
        int bit = bit_buffer_remove_bit(buffer);
        current = (bit == 0) ? current->left_child : current->right_child;
    }
    printf("\nFile decoded succesfully.\n\n");

    bit_buffer_free(buffer);
}







