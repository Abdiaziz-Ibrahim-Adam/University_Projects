/*
 * A data type representing a list.
 *
 * The list is implemented as an dubble linked list.
 *
 * For more information see the corresponding .h-file.
 *
 * Copyright 2024 Jonny Pettersson (jonny@cs.umu.se). Permission is
 * given for usage of this file within the course DV2 at Umeå
 * University. All other usage requires the authors permission.
 *
 * @author  Lars Karlsson
 * @since   2017-10-04
 * @author  Jonny Pettersson
 * @since   2024-02-02
 */

#include <stdlib.h>
#include <assert.h>

#include "list.h"


/* A structure used to represent a node in a list.
 *
 * @elem next       A pointer to the next node or NULL.
 * @elem prev       A pointer to the previous node or NULL.
 * @elem value      A pointer to the value of the node or NULL.
 */
struct node
{
	struct node *next;
	struct node *prev;
	void *value;
};


/* A structure used to represent a list.
 *
 * @elem first      A pointer to the first node in the list or NULL.
 * @elem end        A pointer to the last node in the list or NULL.
 * @elem mfunc      The function for handling dynamically allocated
 *                  memory.
 */
struct list
{
	struct node *first;
	struct node *end;
	list_mem_func mfunc;
};


/* Declaration of internal functions */
static struct node *make_node(void *value);


/* ---------------------- External functions ---------------------- */

void list_set_memhandler(list *const l, const list_mem_func mfunc)
{
	l->mfunc = mfunc;
}


list *list_empty(void)
{
	list *l = malloc(sizeof *l);
	assert(l != NULL);

	l->first = NULL;
	l->end = NULL;
	l->mfunc = NULL;

	return l;
}


bool list_is_empty(const list *const l)
{
	return l->first == NULL;
}


list_position list_first(list *const l)
{
	if (l->first == NULL) {
		return (list_position) { &l->first, &l->end };
	} else {
		return (list_position) { &l->first, &l->first->prev };
	}
}


list_position list_end(list *const l)
{
	if (l->first == NULL) {
		return (list_position) { &l->first, &l->end };
	} else {
		return (list_position) { &l->end->next, &l->end };
	}
}


list_position list_next(list *const l, const list_position pos)
{
	if ((*pos.forward)->next == NULL) {
		return (list_position) { &(*pos.forward)->next, &l->end };
	} else {
		return (list_position) { &(*pos.forward)->next,
                               &(*pos.forward)->next->prev };
	}
}


list_position list_previous(list *const l, const list_position pos)
{
	if ((*pos.backward)->prev == NULL) {
		return (list_position) { &l->first, &(*pos.backward)->prev };
	} else {
		return (list_position) { &(*pos.backward)->prev->next,
                               &(*pos.backward)->prev };
	}
}


bool list_is_first(const list *const l, const list_position pos)
{
	assert(l != NULL);

	return *pos.backward == NULL;
}


bool list_is_end(const list *const l, const list_position pos)
{
	assert(l != NULL);

	return *pos.forward == NULL;
}


void* list_inspect(const list *const l, const list_position pos)
{
	assert(l != NULL);

	return (*pos.forward)->value;
}


list_position list_insert(const list *const l,
                          const list_position pos,
                          void *value)
{
	assert(l != NULL);

	struct node *n = make_node(value);
	n->next = *pos.forward;
	n->prev = *pos.backward;
	*pos.forward = n;
	*pos.backward = n;

	return (list_position){pos.forward, &n->prev};
}


list_position list_remove(list *const l, list_position pos)
{
	struct node *n = *pos.forward;
	*pos.forward = n->next;
	if (*pos.forward) {
		pos.backward = &(*pos.forward)->prev;
	} else {
		pos.backward = &l->end;
	}
	*pos.backward = n->prev;

	if(l->mfunc != NULL) {
		l->mfunc(n->value);
	}
	free(n);

	return pos;
}

void list_kill(list *l)
{
	while (l->first != NULL) {
		struct node *n = l->first;
		l->first = n->next;
		if(l->mfunc != NULL) {
			l->mfunc(n->value);
		}
		free(n);
	}
	free(l);
}


/* ---------------------- Internal function ----------------------- */

/**
 * @brief           Creats a new node.
 *
 * @param value     The value to put in the new node.
 * @param mem_func  The memory function.
 * @return          A pointer to the new node.
 */
static struct node *make_node(void *value)
{
	struct node *n = malloc(sizeof *n);
	assert(n != NULL);

	n->next = NULL;
	n->prev = NULL;
	n->value = value;

	return n;
}
