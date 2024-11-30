/*
 * Programming in C
 * Spring 2024
 *
 * File: huff_table.c
 * Description: Implements functionality to generate and free a Huffman table from a Huffman trie.
 * 
 * Author     : Abdiaziz Ibrahim Adam
 * CS username: dv23aam
 * Date       : 18 March 2024
 */

#include "huff_table.h"
#include "bit_buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---------------------- Internal functions ---------------------------------------------- */

/*
 * Helper function to perform depth-first search on the Huffman trie to generate Huffman codes.
 * 
 * @param node Current node being visited.
 * @param path Current path (Huffman code) being constructed as a string of '0's and '1's.
 * @param depth Current depth in the trie, used to keep track of the path's length.
 * @param huffmanTable The Huffman table being generated, where codes will be stored.
 */
static void trie_DFS(Trie *node, char *path, int depth, char **huffmanTable) 
{
    if (node == NULL) {
        fprintf(stderr, "can't access node\n");
        return;
    }

    if (node->left_child == NULL && node->right_child == NULL) {
        path[depth] = '\0';
        
        huffmanTable[node->byte] = (char *)malloc((depth + 1) * sizeof(char));
        if (huffmanTable[node->byte] != NULL) {
            // Copy the constructed path (Huffman code) into the table
            strcpy(huffmanTable[node->byte], path);
        } else {
            fprintf(stderr, "Memory allocation failed\n");
        }
        return; 
    }

    // Continue traversing the trie
    if (node->left_child != NULL) {
        path[depth] = '0';
        trie_DFS(node->left_child, path, depth + 1, huffmanTable);
    }
    if (node->right_child != NULL) {
        path[depth] = '1';
        trie_DFS(node->right_child, path, depth + 1, huffmanTable);
    }
}

char** huff_table(Trie *root) 
{
    char path[256];
    char **huffmanTable = malloc(256 * sizeof(char*));

    // // Initialize all pointers to NULL
    for (int i = 0; i < 256; i++) {
         huffmanTable[i] = NULL;
     }

    trie_DFS(root, path, 0, huffmanTable);

    return huffmanTable;
}

void free_huff_table(char **huffmanTable) 
{
    for (int i = 0; i < 256; i++) {
        if (huffmanTable[i] != NULL) {
            free(huffmanTable[i]);
        }
    }
    free(huffmanTable);
}
