/*
 * A data type representing a priority gueue.
 *
 * The data type represent a priority queue. The priority queue uses
 * a list as the internal representation. Takes a compare function
 * when creating a new priority queue, the compare function is used
 * to determine priority of elements within the priority queue.
 *
 * For more information see the corresponding .h-file.
 *
 * Copyright 2024 Jonny Pettersson (jonny@cs.umu.se). Permission is
 * given for usage of this file within the course DV2 at Umeå
 * University. All other usage requires the authors permission.
 *
 * @author: Sebastian Sandberg <cass@cs.umu.se>
 * @since:  2017-12-11
 * @author  Jonny Pettersson
 * @since   2024-02-02
 */

#include <stdlib.h>
#include "list.h"
#include "pqueue.h"
#include <assert.h>

/* A structure used to represent an element in a priority queue.
 *
 * @elem the_list  A pointer to the internally used list.
 * @elem cmp_func  The function used to decide priority between
 *                 elements in the priority queue.
 */
struct pqueue {
	list *the_list;
	pqueue_cmp_func cmp_func;
};


pqueue* pqueue_empty(pqueue_cmp_func cmp_func)
{
	pqueue* pq = malloc(sizeof *pq);
	assert(pq);

	pq->the_list = list_empty();
	assert(pq->the_list);
	pq->cmp_func = cmp_func;

	return pq;
}


void pqueue_set_memhandler(const pqueue *const pq,
                           pqueue_mem_func mfunc)
{
	assert(pq);
	assert(pq->the_list);

	list_set_memhandler(pq->the_list, mfunc);
}


void pqueue_delete_first(pqueue *const pq)
{
	assert(pq);
	assert(pq->the_list);

	if(!list_is_empty(pq->the_list)) {
		list_remove(pq->the_list, list_first(pq->the_list));
	}
}


void pqueue_insert(pqueue *const pq, void *value)
{
	assert(pq);
	assert(pq->the_list);

	if(list_is_empty(pq->the_list)) {
		list_insert(pq->the_list, list_first(pq->the_list), value);
	} else {
		list_position pos = list_first(pq->the_list);
		while(!list_is_end(pq->the_list, pos)) {
			if(pq->cmp_func != NULL) {
				if(pq->cmp_func(value, list_inspect(pq->the_list,
					                                 pos)) < 0) {
					list_insert(pq->the_list, pos, value);
					return;
				}
			}
			pos = list_next(pq->the_list, pos);
		}
		list_insert(pq->the_list, pos, value);
	}
}


void* pqueue_inspect_first(const pqueue *const pq)
{
	assert(pq);
	assert(pq->the_list);

	return list_inspect(pq->the_list, list_first(pq->the_list));
}


bool pqueue_is_empty(const pqueue *const pq)
{
	assert(pq);
	assert(pq->the_list);

	return list_is_empty(pq->the_list);
}


void pqueue_kill(pqueue *pq)
{
	assert(pq);
	assert(pq->the_list);

	list_kill(pq->the_list);
	free(pq);
}


/* ---- External function used for debugging - Not part of API ---- */

void pqueue_print(const pqueue *const pq,
                  pqueue_print_func print_func)
{
	assert(pq);
	assert(pq->the_list);

	list_position pos = list_first(pq->the_list);
	while(!list_is_end(pq->the_list, pos)){
		print_func(list_inspect(pq->the_list, pos));
		pos = list_next(pq->the_list, pos);
	}
}


