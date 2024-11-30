/*
 * Module       : stack.c
 * Description  : This module implements a basic stack structure with functions to create, push, pop, check 
 *                if the stack is empty, and free the stack.
 * 
 * Dependencies : Requires `stack.h` for the stack data structure and function prototypes.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"

/* ------------------------------- Function Implementations ------------------------------------------------------------------------------------------------ */

Stack* create_stack(void) {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    if (stack == NULL) {
        perror("Failed to create stack");
        return NULL;
    }
    stack->top = NULL;
    return stack;
}

void push(Stack* stack, void* data) {
    if (stack == NULL) {
        fprintf(stderr, "Error: Cannot push to a NULL stack.\n");
        return;
    }
    Stack_Node* new_node = (Stack_Node*)malloc(sizeof(Stack_Node));
    if (new_node == NULL) {
        perror("Failed to allocate memory for new node");
        return;
    }
    new_node->data = data;
    new_node->next = stack->top;
    stack->top = new_node;
}

void* pop(Stack* stack) {
    if (stack == NULL) {
        fprintf(stderr, "Error: Cannot pop from a NULL stack.\n");
        return NULL;
    }
    if (is_empty(stack)) {
        return NULL;
    }
    Stack_Node* top_node = stack->top;
    void* data = top_node->data;
    stack->top = top_node->next;
    free(top_node);
    return data;
}

int is_empty(Stack* stack) {
    if (stack == NULL) {
        fprintf(stderr, "Error: Cannot check if NULL stack is empty.\n");
        return 1;  
    }
    return stack->top == NULL;
}

void free_stack(Stack* stack) {
    if (stack == NULL) {
        fprintf(stderr, "Error: Cannot free a NULL stack.\n");
        return;
    }
    while (!is_empty(stack)) {
        void* data = pop(stack);
        if (data != NULL) {
            free(data);
        }
    }
    free(stack);
}

/* --------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */