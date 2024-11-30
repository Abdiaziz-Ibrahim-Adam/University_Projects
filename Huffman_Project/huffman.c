/*
 * Programmering i C
 * Spring 24
 *
 * File:         huffman.c
 * Description:  A simple implementation of the Huffman encoding and decoding algorithms.
 *               This program reads an input file to perform frequency analysis, constructs a Huffman tree,
 *               and either encodes or decodes a file based on the specified mode. The encoding process
 *               compresses the input file using the Huffman coding scheme, while the decoding process
 *               reconstructs the original file from its encoded form.
 * 
 * Author:       Abdiaziz Ibrahim Adam
 * CS username:  dv23aam
 * Date:         18 March 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"

int main(int argc, const char *argv[]) 
{
    files my_files;

    // Check and parse command line arguments. If incorrect, terminate the program.
    if (validate_program_arguments(argc, argv, &my_files) != 0) {
        return 1; 
    }

    int *frequency_table = create_frequency_table(my_files.in_frequency_file); 

    // Use the frequency table for Huffman tree construction
    Trie *huffman_trie_root = build_huff_trie(frequency_table);

    //The huffman table:
    char **huffmanTable = huff_table(huffman_trie_root);

    if (strcmp("-encode", argv[1]) == 0){
        encode_file(my_files.in_file, my_files.out_file, huffmanTable) ;
    }

    else if (strcmp("-decode", argv[1]) == 0){
        decode_file(my_files.in_file, my_files.out_file, huffman_trie_root, huffmanTable);
    }

    trie_kill(huffman_trie_root);
    free_huff_table(huffmanTable);
    free(frequency_table); 

    fclose(my_files.in_frequency_file); 
    fclose(my_files.in_file); 
    fclose(my_files.out_file); 

    return 0;
}


int validate_program_arguments(int argc, const char *argv[], files *my_files)
{
    if (argc != 5){
        error_message();
        return 1;
    }

    if (strcmp("-encode", argv[1])!= 0 && strcmp("-decode", argv[1])!= 0) {
        error_message();
        return 1;
    }

    my_files->in_frequency_file = fopen(argv[2], "rb");
    if (my_files->in_frequency_file == NULL){
        error_message();
        return 1;
    }

    my_files->in_file = fopen(argv[3], "rb");
    if (my_files->in_file== NULL){
        error_message();
        return 1;
    }

    my_files->out_file = fopen(argv[4], "wb");
    if (my_files->out_file == NULL){
        error_message();
        return 1;
    }
    return 0;
}

void error_message(void) 
{
    fprintf(stderr, 
    "\nUSAGE:\n"
    "huffman [OPTION] [FILE0] [FILE1] [FILE2]\n" 
    "Options:\n" 
    "-encode encodes FILE1 according to frequence analysis done on FILE0. Stores the result in FILE2\n"
    "-decode decodes FILE1 according to frequence analysis done on FILE0. Stores the result in FILE2\n\n");
}
