/*
 * Programming in C
 * Spring 2024
 *
 * File: Huff_trie.c
 * Description: Provides functionality for creating and managing a Huffman Trie used in Huffman encoding and decoding.
 * This includes creating individual Trie nodes, combining them into a Huffman Trie based on character frequencies,
 * and properly deallocating memory used by the Trie.
 *
 * Author     : Abdiaziz Ibrahim Adam
 * CS username: dv23aam
 * Date       : 18 March 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Huff_Trie.h"
#include "bit_buffer.h"

/* ------------------------------------ Internal functions ---------------------------------------------- */

/*
 * This function compares two Trie nodes based on their weight. It is designed to be used
 * by the priority queue to order the nodes from the lowest to the highest weight. This ordering
 * is crucial for the construction of the Huffman tree, ensuring that nodes with lower frequencies (weights)
 * are combined earlier, adhering to the Huffman coding algorithm's requirements.
 *
 * @param a A void pointer to the first Trie node to be compared.
 * @param b A void pointer to the second Trie node to be compared.
 * @return An integer less than, equal to, or greater than zero if the first argument is considered
 *         to be respectively less than, equal to, or greater than the second. Specifically:
 *         - Returns a negative value if the weight of the first node is less than the weight of the second.
 *         - Returns zero if the weights are equal.
 *         - Returns a positive value if the weight of the first node is greater than the weight of the second.
 *
 * Note: This function is intended to be used as a callback by the priority queue implementation. Both
 *       parameters are expected to be pointers to Trie nodes, and the function does not perform any
 *       type checking on these inputs. It is the caller's responsibility to ensure that the correct
 *       data types are passed to this function.
 */
static int compare(void *a, void *b) 
{
    Trie *node_a = (Trie *)a;
    Trie *node_b = (Trie *)b;

    return (node_a->weight > node_b->weight) - (node_a->weight < node_b->weight);
}

Trie *trie_create(int weight, int byte) 
{
    Trie* node = (Trie*)malloc(sizeof(Trie));

    if (node == NULL) {
        fprintf(stderr, "Failed to allocate memory for Trie\n");
        return NULL;
    }

    // Allocate memory for the byte value
    node->byte = byte;
    node->weight = weight;
    node->left_child = NULL;
    node->right_child = NULL;

    return node;
}

Trie *trie_combine(Trie *left, Trie *right) 
{
    Trie* parent = (Trie*)malloc(sizeof(Trie));

    if (parent == NULL) {
        fprintf(stderr, "Failed to allocate memory for Trie\n");
        return NULL;
    }

    parent->weight = left->weight + right->weight;
    parent->byte = -1; // Internal node, no byte value
    parent->left_child = left;
    parent->right_child = right;

    return parent;
}

Trie *build_huff_trie(int *frequency_table)
{
    pqueue *pq = pqueue_empty(compare); 

    for (int i = 0; i < 256; i++) {
        Trie *node = trie_create(frequency_table[i], i);
    
        pqueue_insert(pq, node);
    }

    while (!pqueue_is_empty(pq)){
        Trie *A_node = pqueue_inspect_first(pq);
        pqueue_delete_first(pq);

        if(pqueue_is_empty(pq)){
            pqueue_kill(pq);
            return A_node;
        }

        Trie *B_node = pqueue_inspect_first(pq);
        pqueue_delete_first(pq);

        Trie *P_node = trie_combine(A_node, B_node);

        pqueue_insert(pq, P_node);
    }
    pqueue_kill(pq);
    return NULL;
}

void trie_kill(Trie *trie) 
{
    if (trie != NULL) {
        trie_kill(trie->left_child);
        trie_kill(trie->right_child);
        free(trie);
    }
}









