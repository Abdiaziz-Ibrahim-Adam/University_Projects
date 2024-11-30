/**
 * @defgroup list_h List
 *
 * @brief A data type representing a list.
 *
 * The data type represent a list.
 *
 * Copyright 2024 Jonny Pettersson (jonny@cs.umu.se). Permission is
 * given for usage of this file within the course DV2 at Umeå
 * University. All other usage requires the authors permission.
 *
 * @author  Lars Karlsson
 * @since   2017-10-04
 * @author  Jonny Pettersson
 * @since   2024-02-02
 *
 * @{
 */

#pragma once

#include <stdbool.h>

/**
 * @brief          Function used for handling the data pointed to in
 *                 each node in the list.
 */
typedef void (*list_mem_func)(void*);

/**
 * @brief          A struture holding information related to the list.
 */
typedef struct list list;

/**
 * @brief          A structure used to represent a position in a list.
 *
 * @elem forward   A pointer to the next node or NULL.
 * @elem backward  A pointer to the previous node or NULL.
 */
typedef struct list_position
{
	/** Beskrivning */
	struct node **forward;
	struct node **backward;
} list_position;

/**
 * @brief          Sets the function to be used when deallocating
 *                 dynamically allocated memory used by the list.
 *
 * @param l        The list.
 * @param mem_func The function used to deallocate the memory used by
 *                 the list.
 * @return         -
 */
void list_set_memhandler(list *const l, const list_mem_func mem_func);

/**
 * @brief          Allocate an empty list. The memory is allocated
 *                 dynamically. The memory should be deallocated using
 *                 the function list_kill.
 *
 * @return         The new list.
 */
list *list_empty(void);

/**
 * @brief          Check if the list is empty or not.
 *
 * @param l        The list to check.
 * @return         True if the list is empty, otherwise false.
 */
bool list_is_empty(const list *const l);

/**
 * @brief          Returns the position of the first node in the list.
 *
 * @param l        The list.
 * @return         The position of the first node in the list.
 */
list_position list_first(list *const l);

/**
 * @brief          Returns the position past the end of the list.
 *
 * @param l        The list.
 * @return         The position after the last node in the list.
 */
list_position list_end(list *const l);

/**
 * @brief          Returns the next position in the list.
 *
 * @param l        The list.
 * @param pos      The position before the call to the function.
 * @return         The position of the next node in the list.
 */
list_position list_next(list *const l, const list_position pos);

/**
 * @brief          Returns the previous position in the list.
 *
 * @param l        The list.
 * @param pos      The position before the call to the function.
 * @return         The position of the previous node in the list.
 */
list_position list_previous(list *const l, const list_position pos);

/**
 * @brief          Check if the position points at the first node in
 *                 the list.
 *
 * @param l        The list.
 * @param pos      The position to be checked.
 * @return         True if the position points at the first node in
 *                 the list, otherwise false.
 */
bool list_is_first(const list *const l, const list_position pos);

/**
 * @brief          Check if the position points at the last node in
 *                 the list.
 *
 * @param l        The list.
 * @param pos      The position to be checked.
 * @return         True if the position points at the last node in the
 *                 list, otherwise false.
 */
bool list_is_end(const list *const l, const list_position pos);

/**
 * @brief          Get the value of the node at the given position.
 *
 * @param l        The list.
 * @param pos      The position to be inspected.
 * @return         A pointer to the value.
 */
void* list_inspect(const list *const l, const list_position pos);

/**
 * @brief          Insert a value in the list. The value is inserted
 *                 immediately before the given position.
 *
 * @param l        The list.
 * @param pos      The position of the node that the new node will be
 *                 inserted immediately before.
 * @param value    The value to be inserted.
 * @return         The position of the new node in the list.
 */
list_position list_insert(const list *const l,
                          const list_position pos,
                          void *value);

/**
 * @brief          Remove a node from the list. The node will be
 *                 killed.
 *
 * @param l        The list.
 * @param pos      The position of the node to remove.
 * @return         The position of the node immediately after the
 *                 removed element.
 */
list_position list_remove(list *const l, list_position pos);

/**
 * @brief          Deallocate a list after killing all the nodes it
 *                 contains.
 *
 * @param l        The list to deallocate.
 * @return         -
 */
void list_kill(list *l);


/**
 * @}
 */
