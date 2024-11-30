/*
 * Programmering i C
 * Spring 24
 *
 * File:         frequency_table.c
 * Description:  Provides functionality to create a frequency table from a given input file.
 *               This table is used to determine the frequency of each byte (character) in the file,
 *               which is a crucial step in the Huffman encoding process.
 * 
 * Author:       Abdiaziz Ibrahim Adam
 * CS username:  dv23aam
 * Date:         18 March 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include "frequency_table.h"

#define NUM_BYTES 256

int *create_frequency_table(FILE *file) 
{
    int *frequency = calloc(NUM_BYTES, sizeof(int));

    int c;
    while ((c = fgetc(file)) != EOF) {
        frequency[c]++;
    }
    frequency[4]++;
    
    return frequency;
}


