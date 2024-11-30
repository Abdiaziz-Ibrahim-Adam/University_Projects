/**
 * @file stack.h
 * @brief Stack Data Structure for Generic Pointers
 *
 * This header file defines a generic stack data structure, implemented with linked nodes.
 * Each node holds a pointer to generic data, allowing the stack to store various data types.
 * The stack supports basic operations such as push, pop, and checking if it's empty. It also
 * provides a function to free all allocated memory in the stack.
 * 
 * @section Author
 * - Author: Abdiaziz Ibrahim Adam
 * - Date: 2024-11-18
 *
 * @warning The `push` function allocates memory for each new element, and `pop` retrieves and
 *          frees each element's node. Users should free any data in stack nodes manually if 
 *          `free_stack` is not used. The `free_stack` function frees all data stored in the stack.
 */

#ifndef STACK_H
#define STACK_H

#include <stdbool.h>

/* ------------------------------- Structures ------------------------------------------------------------------------------------------------ */

/**
 * @struct Stack_Node
 * @brief Node structure representing an element in the stack.
 *
 * Each node stores a pointer to data and a pointer to the next node in the stack.
 *
 * @param data Generic pointer to the data stored in the node.
 * @param next Pointer to the next node in the stack.
 */
typedef struct stack_Node {
    void* data;                /**< Generic pointer to the node's data. */
    struct stack_Node* next;   /**< Pointer to the next node in the stack. */
} Stack_Node;

/**
 * @struct Stack
 * @brief Represents the stack, containing a pointer to the top element.
 *
 * The `Stack` structure holds a pointer to the top node of the stack, allowing for push, pop, 
 * and other stack operations.
 *
 * @param top Pointer to the top element (node) of the stack.
 */
typedef struct {
    Stack_Node* top;           /**< Pointer to the top node in the stack. */
} Stack;

/* ------------------------------- Function Prototypes ------------------------------------------------------------------------------------------------------- */

/**
 * @brief Creates a new empty stack.
 *
 * Allocates memory for a `Stack` structure and initializes it to be empty.
 *
 * @return Stack* Pointer to the newly created empty stack, or NULL on failure.
 * 
 * @warning The caller is responsible for deallocating the stack by calling `free_stack`.
 */
Stack* create_stack(void);

/**
 * @brief Pushes a new element onto the stack.
 *
 * Allocates a new `Stack_Node` and pushes it onto the stack. The `data` parameter can point to
 * any type of data.
 *
 * @param stack Pointer to the Stack where the data will be pushed.
 * @param data Generic pointer to the data being pushed onto the stack.
 * 
 * @warning The `push` function allocates memory for each new node. To avoid memory leaks,
 *          ensure each node is popped and freed by calling `pop` or `free_stack`.
 */
void push(Stack* stack, void* data);

/**
 * @brief Pops and returns the top element from the stack.
 *
 * Removes the top node from the stack, returns its data, and frees the node's memory.
 * If the stack is empty, returns NULL.
 *
 * @param stack Pointer to the Stack from which the top element is popped.
 * @return void* Pointer to the data stored in the popped element, or NULL if the stack is empty.
 */
void* pop(Stack* stack);

/**
 * @brief Checks if the stack is empty.
 *
 * Evaluates whether the stack has any elements. Returns true if the stack is empty.
 *
 * @param stack Pointer to the Stack to check.
 * @return int Returns 1 (true) if the stack is empty, 0 (false) otherwise.
 */
int is_empty(Stack* stack);

/**
 * @brief Frees all memory used by the stack.
 *
 * This function removes all nodes from the stack, frees their data, and releases memory
 * allocated for the stack structure itself.
 *
 * @param stack Pointer to the Stack to be freed.
 * 
 * @warning This function frees all data stored in the stack nodes. Any additional dynamic memory
 *          associated with `data` in each node must be managed before calling `free_stack`.
 */
void free_stack(Stack* stack);

#endif // STACK_H
