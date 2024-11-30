/**
 * @defgroup pqueue_h Priorty queue
 *
 * @brief A data type representing a priority queue.
 *
 * The data type represent a priority queue. Takes a compare function
 * when creating a new priority queue, the compare function is used
 * to determine priority of elements within the priority queue.
 *
 * Copyright 2024 Jonny Pettersson (jonny@cs.umu.se). Permission is
 * given for usage of this file within the course DV2 at Umeå
 * University. All other usage requires the authors permission.
 *
 * @author: Sebastian Sandberg <cass@cs.umu.se>
 * @since:  2017-12-11
 * @author  Jonny Pettersson
 * @since   2024-02-02
 *
 * @{
 */

#ifndef PQUEUE_H
#define PQUEUE_H

#include <stdbool.h>
#include <stddef.h>

/**
 * @brief             Function used for handling the data pointed to
 *                    in each element in the priority queue.
 */
typedef void (*pqueue_mem_func)(void*);

/**
 * @brief             Function that is called for comparing two
 *                    priorities.
 *
 * If val1 is the first value and val2 the second, the following must
 * be returned: \n
 *   \> 0   if val1 \> val2 \n
 *   \< 0   if val1 \< val2 \n
 *   0     if val1 = val2
 */
typedef int (*pqueue_cmp_func)(void*, void*);

/**
 * @brief             Function used for printing each element in the
 *                    priority queue.
 */
typedef void (*pqueue_print_func)(void*);

/**
 * @brief          A struture holding information related to the
 *                 priority queue.
 */
typedef struct pqueue pqueue;


/**
 * @brief             Create a new empty priority queue.
 *
 * @param cmp_func    The compare function used to determine priority
 *                    within the queue.
 * @return            The new priority queue.
 */
pqueue* pqueue_empty(pqueue_cmp_func cmp_func);


/**
 * @brief             Set the memory handling funciton for the
 *                    priority queue.
 *
 * @param pq          The priority queue.
 * @param mfunc       The memory handling function.
 */
void pqueue_set_memhandler(const pqueue *const pq,
                           pqueue_mem_func mfunc);


/**
 * @brief             Remove the first element from the priority
 *                    queue.
 *
 * @param pq          The priority queue.
 * @return            -
 */
void pqueue_delete_first(pqueue *const pq);


/**
 * @brief             Insert a new element with a value in the
 *                    priority queue based on the priority defined in
 *                    pqueue_cmp_func.
 *
 * @param pq          The priority queue.
 * @param value       The value to put in the new element.
 * @return            -
 */
void pqueue_insert(pqueue *const pq, void* value);


/**
 * @brief             Get the value from the front element in the
 *                    priority queue.
 *
 * @param pq          The pririty queue.
 * @return            A pointer to the value in the front element in
 *                    the priority queue.
 */
void *pqueue_inspect_first(const pqueue *const pq);


/**
 * @brief             Check if the priority queue is empty.
 *
 * @param pq          The priority queue.
 * @return            True if empty, else false.
 */
bool pqueue_is_empty(const pqueue *const pq);


/**
 * @brief             Deallocate resources for the priority queue.
 *
 * @param pq          The priority queue.
 * @return            -
 */
void pqueue_kill(pqueue* pq);

/**
 * @}
 */

/* ---- External function used for debugging - Not part of API ---- */

/*
 * @brief             Print the priority queue using the print
 *                    function given as a parameter to the function.
 *
 * @param pq          The priority queue.
 * @param print_func  The print function to be used.
 * @return            -
 */
void pqueue_print(const pqueue *const pq,
                  pqueue_print_func print_func);

#endif
