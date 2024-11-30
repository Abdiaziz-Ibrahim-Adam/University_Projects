/*
 * A data type representing a buffer that can hold bit values.
 *
 * The buffer can hold bit values. The buffer has a direction, single
 * bits or single bytes (8 bits) is inserted in one end of the buffer
 * and single bits or single bytes can be removed from the other end.
 * It also possible to inspect bit values without removing them from
 * the buffer. The buffer dynamically increases it size when needed,
 * it does not dynamically decrease it size.
 *
 * The bit buffer uses an array of chars to hold the bits. This means
 * that there could be bits in the array which does not contain
 * inserted (and not removed) contents. To keep track of where to
 * insert content a "pointer" (the index of the bit in the array),
 * next_insert, points to where the next bit should be inserted. To
 * keep track of the content to be removed a "pointer" (the index of
 * the bit in the array), next_removed, points to the next bit to be
 * removed. The buffer is circular and dynamically increases it size
 * when needed.
 *
 * For more information see the corresponding .h-file.
 *
 * Copyright 2024 Jonny Pettersson (jonny@cs.umu.se). Permission is
 * given for usage of this file within the course DV2 at Umeå
 * University. All other usage requires the authors permission.
 *
 * @author  Jonny Pettersson
 * @since   2024-02-02
 */

#include "bit_buffer.h"
#include <stdlib.h>
#include <assert.h>

/* A structure used to handle the resources connected to the bit
 * buffer.
 *
 * @elem capacity       The total number of bits in the char array.
 * @elem array          The char array holding the contents.
 * @elem size           The number of inserted (and not removed) bits
 *                      into the bit buffer.
 * @elem next_insert    A "pointer" (the index of the bit) to the bit
 *                      where the next inserted bit should be put in
 *                      the array.
 * @elem next_remove    A "pointer" (the index of the bit) to the bit
 *                      that is next to be removed from the array.
 */
struct bit_buffer {
	int capacity;
	char *array;
	int size;
	int next_insert;
	int next_remove;
};


/* Declaration of internal functions */
static int bit_buffer_get_bit_value(const bit_buffer *const b,
                                    const int bit_in_array);
static void bit_buffer_set_bit_value(bit_buffer *b,
                                     const int bit_in_array,
                                     const int value);


/* ---------------------- External functions ---------------------- */

bit_buffer *bit_buffer_copy(const bit_buffer *const b)
{
	assert(b);
	assert(b->array);
	bit_buffer *res = bit_buffer_empty();
	for (int i = 0 ; i < bit_buffer_size(b) ; i++) {
		bit_buffer_insert_bit(res, bit_buffer_inspect_bit(b, i));
	}

	return res;
}


bit_buffer *bit_buffer_create(const char *const byte_array,
                              const int size)
{
	bit_buffer *b = bit_buffer_empty();
	b->array = realloc(b->array, size);
	assert(b->array);

	for (int i = 0 ; i < size ; i++) {
		b->array[i] = byte_array[i];
	}

	b->capacity = size * 8;
	b->size = size * 8;

	return b;
}


bit_buffer *bit_buffer_empty()
{
	bit_buffer *b = malloc(sizeof(*b));
	assert(b);

	b->capacity = 16;
	b->array = calloc(2, sizeof(char));
	assert(b->array);
	b->size = 0;
	b->next_insert = 0;
	b->next_remove = 0;

	return b;
}


void bit_buffer_free(bit_buffer *b)
{
	assert(b);
	assert(b->array);
	free(b->array);
	free(b);
}


void bit_buffer_insert_bit(bit_buffer *b, const int value)
{
	assert(b);
	assert(b->array);

	/* Extend the capacity of the buffer if needed */
	if (bit_buffer_size(b) + 1 == b->capacity) {
		b->array = realloc(b->array, b->capacity / 8 + 1);
		b->capacity += 8;
		b->array[(b->capacity / 8) - 1] = 0;

		/* Handle the case if the new byte is inserted in the middle of
		legal data in the buffer */
		if (b->next_remove >= b->next_insert) {
			int i;
			for (i = b->capacity - 1 ; i - 8 >= b->next_remove ; i--) {
				int value = bit_buffer_get_bit_value(b, i - 8);
				bit_buffer_set_bit_value(b, i, value);
			}
			for (i = b->next_remove ; i < b->next_remove + 8 ; i++) {
				bit_buffer_set_bit_value(b, i, 0);
			}
			b->next_remove += 8;
		}
	}

	/* Update the value of the bit in the buffer */
	bit_buffer_set_bit_value(b, b->next_insert, value);

	/* Update information */
	b->next_insert = (b->next_insert + 1) % b->capacity;
	b->size++;
}


void bit_buffer_insert_byte(bit_buffer *b, const char the_byte)
{
	assert(b);
	assert(b->array);
	for (int bit = 7 ; bit >= 0 ; bit--) {
		bit_buffer_insert_bit(b, (the_byte & (1 << bit)));
	}
}


int bit_buffer_inspect_bit(const bit_buffer *const b,
                           const int bit_no)
{
	assert(b);
	assert(b->array);
	int bit_in_buffer = (bit_no + b->next_remove) % b->capacity;

	return bit_buffer_get_bit_value(b, bit_in_buffer);
}


int bit_buffer_remove_bit(bit_buffer *b)
{
	assert(b);
	assert(b->array);
	assert(b->size > 0);
	int value = bit_buffer_get_bit_value(b, b->next_remove);
	bit_buffer_set_bit_value(b, b->next_remove, false);
	b->next_remove = (b->next_remove + 1) % b->capacity;
	b->size--;

	return value;
}


char bit_buffer_remove_byte(bit_buffer *b)
{
	assert(b);
	assert(b->array);
	assert(b->size >= 8);
	char the_byte = 0;

	for (int bit = 7 ; bit >= 0 ; bit--) {
		if (bit_buffer_remove_bit(b)) {
			the_byte = the_byte | 1 << bit;
		}
	}

	return the_byte;
}


int bit_buffer_size(const bit_buffer *const b)
{
	assert(b);

	return b->size;
}


char *bit_buffer_to_byte_array(const bit_buffer *const b)
{
	assert(b);
	assert(b->array);
	char *res = calloc(b->capacity / 8, sizeof(char));
	for (int i = 0 ; i < b->capacity / 8 ; i++) {
		res[i] = b->array[i];
	}

	return res;
}



/* ---- External functions used for debugging - Not part of API --- */

char *bit_buffer_array_to_string(const bit_buffer *const b)
{
	assert(b);
	assert(b->array);
	char *res = calloc(b->capacity + 1, sizeof(char));
	assert(res);
	for (int i = 0 ; i < b->capacity ; i++) {
		res[i] = bit_buffer_get_bit_value(b, i) ? '1' : '0';
	}
	res[b-> capacity] = '\0';

	return res;
}


void bit_buffer_print(const bit_buffer *const b)
{
	assert(b);
	assert(b->array);
	printf("b->capacity = %d\n", b->capacity);
	printf("b->size = %d\n", b->size);
	for (int i = 0 ; i < b->next_insert ; i++) {
		printf(" ");
	}
	printf("i\n");
	char *bb_str = bit_buffer_array_to_string(b);
	printf("%s\n", bb_str);
	for (int i = 0 ; i < b->next_remove ; i++) {
		printf(" ");
	}
	printf("r\n");
	printf("\n");
	free (bb_str);
}


char *bit_buffer_to_string(const bit_buffer *const b)
{
	assert(b);
	assert(b->array);
	char *res = calloc(bit_buffer_size(b) + 1, sizeof(char));
	for (int i = 0 ; i < bit_buffer_size(b) ; i++) {
		res[i] = bit_buffer_inspect_bit(b, i) ? '1' : '0';
	}
	res[bit_buffer_size(b)] = '\0';

	return res;
}


/* ---------------------- Internal functions ---------------------- */

/*
 * @brief               Returns the value (0 or 1) of the bit
 *                      (bit_in_array) in the array.
 *
 * @param b             The bit buffer.
 * @param bit_in_array  The index of the bit in the array.
 * @return              The value.
 */
static int bit_buffer_get_bit_value(const bit_buffer *const b,
                                    const int bit_in_array)
{
	assert(b);
	assert(b->array);
	int byte_no = bit_in_array / 8;
	char the_byte = b->array[byte_no];
	int bit = 7 - (bit_in_array % 8);

	return (the_byte & (1 << bit)) > 0;
}


/*
 * @brief               Sets the value (0 or 1) of the bit
 *                      (bit_in_array) in the array.
 *
 * @param b             The bit buffer.
 * @param bit_in_array  The index of the bit in the array.
 * @param value         The bit value to be set in the bit_in_array.
 * @return              The value.
 */
static void bit_buffer_set_bit_value(bit_buffer *b,
                                     const int bit_in_array,
                                     const int value)
{
	assert(b);
	assert(b->array);

	/* Find and copy the correct byte in the buffer */
	int byte_no = bit_in_array / 8;
	char the_byte = b->array[byte_no];

	/* Find the correct bit in the byte and update the value of that
	   bit */
	int bit = 7 - bit_in_array % 8;
	if (value) {
		the_byte = the_byte | 1 << bit;
	}
	else {
		the_byte = the_byte & ~(1 << bit);
	}

	/* Copy the updated byte to the buffer */
	b->array[byte_no] = the_byte;
}
