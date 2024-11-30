/**
 * @defgroup bit_buffer_h Bit buffer
 *
 * @brief A data type representing a buffer that can hold bit values.
 *
 * The buffer can hold bit values. The buffer has a direction, single
 * bits or single bytes (8 bits) is inserted in one end of the buffer
 * and single bits or single bytes can be removed from the other end.
 * It also possible to inspect bit values without removing them from
 * the buffer. The buffer dynamically increases it size when needed,
 * it does not dynamically decrease it size.
 *
 * The user is recommended to use an instance of the buffer with either
 * operations (insert/remove) for bits or bytes. If the user want to
 * use an instance of the buffer with both operations (insert/remove)
 * for bits and bytes, the user is recommended to experiment with the
 * buffer to study the behaviour.
 *
 * Copyright 2024 Jonny Pettersson (jonny@cs.umu.se). Permission is
 * given for usage of this file within the course DV2 at Umeå
 * University. All other usage requires the authors permission.
 *
 * @author  Jonny Pettersson
 * @since   2024-02-02
 *
 * @{
 */

#pragma once

#include <stdio.h>
#include <stdbool.h>

/**
 * @brief             A struture holding information related to the
 *                    bit_buffer.
 */
typedef struct bit_buffer bit_buffer;


/**
 * @brief             Copies the content of the supplied bit buffer
 *                    to a new allocated bit buffer. The user is
 *                    responsible for deallocating the new buffer.
 *
 * @param b           The bit buffer which content should be copied.
 * @return            The new allocated bit buffer.
 */
bit_buffer *bit_buffer_copy(const bit_buffer *const b);

/**
 * @brief             Creates a new bit buffer and copies the data
 *                    supplied to the function into the new buffer.
 *                    The user is responsible for deallocating the new
 *                    buffer.
 *
 * @param byte_array  Contains the information to be copied into the
 *                    new buffer.
 * @param size        The number of bytes in the byte_array.
 * @return            The new allocated bit buffer.
 */
bit_buffer *bit_buffer_create(const char *const byte_array,
                              const int size);

/**
 * @brief             Creates a new empty bit buffer. The user is
 *                    responsible for deallocating the new buffer.
 *
 * @return            The new allocated bit buffer.
 */
bit_buffer *bit_buffer_empty();

/**
 * @brief             Deallocates all memory used by the supplied bit
 *                    buffer.
 *
 * @param b           The bit buffer to be deallocated.
 * @return            -
 */
void bit_buffer_free(bit_buffer *b);

/**
 * @brief             Insert a bit (the value) into the given bit
 *                    buffer. The size of the buffer is increased if
 *                    needed.
 *
 * @param b           The bit buffer.
 * @param value       The bit value to be inserted.
 * @return            -
 */
void bit_buffer_insert_bit(bit_buffer *b, const int value);

/**
 * @brief             Insert a byte (containing 8 bits) into the given
 *                    bit buffer. The size of the buffer is increased
 *                    if needed.
 *
 * @param b           The bit buffer.
 * @param the_byte    The bit values to be inserted.
 * @return            -
 */
void bit_buffer_insert_byte(bit_buffer *b, const char the_byte);

/**
 * @brief             Returns the value of the given bit_no within the
 *                    bit buffer. bit_no = 0 referes to the first bit
 *                    to be removed, bit_no = 1 referes to the second
 *                    bit to be removed, and so on. If the given
 *                    bit_no is not a member in the bitset (bit_no >=
 *                    bit_buffer_size(b)) the behaviour is undefined.
 *
 * @param b           The bit buffer.
 * @param bit_no      The number of the bit which should be inspected.
 * @return            The value (0 or 1) of the inspected bit.
 */
int bit_buffer_inspect_bit(const bit_buffer *const b,
                           const int bit_no);

/**
 * @brief             Removs the next bit to be removed from the bit
 *                    buffer. Returns the value of that bit. If the
 *                    bit buffer is empty the behaviour is undefined.
 *
 * @param b           The bit buffer.
 * @return            The value (0 or 1) of the removed bit.
 */
int bit_buffer_remove_bit(bit_buffer *b);

/**
 * @brief             Removs the next byte of bits (8 bits) to be
 *                    removed from the bit buffer. Returns the values
 *                    of the removed bits. If the bit buffer contains
 *                    less than 8 bits the behaviour is undefined.
 *
 * @param b           The bit buffer.
 * @return            The values (0 or 1) of the removed bits in the
 *                    byte (8 bits).
 */
char bit_buffer_remove_byte(bit_buffer *b);

/**
 * @brief             Returns the number of bits in the bit buffer.
 *
 * @param b           The bit buffer.
 * @return            The number of bits.
 */
int bit_buffer_size(const bit_buffer *const b);

/**
 * @brief             Converts the bit buffer to an array of bytes.
 *                    The array will contain all bits existing within
 *                    the bit buffer. If the size of the bit buffer is
 *                    not a multiple of 8 bits, the last byte of the
 *                    array will be padded with bits of value 0.
 *                    Memory is allocated for the array. The user is
 *                    responsible for deallocating the returned array.
 *
 * @param b           The bit buffer.
 * @return            A byte(char)-array of the content in the bitset.
 */
char *bit_buffer_to_byte_array(const bit_buffer *const b);

/**
 * @}
 */

/* ---- External functions used for debugging - Not part of API --- */

/*
 * @brief             Converts the whole array in the bit buffer to a
 *                    string. The string will contain all bits
 *                    existing within the array in the bit buffer (see
 *                    the corresponding .c file for more informaiton).
 *                    Memory is allocated for the string. The user is
 *                    responsible for deallocating the returned
 *                    string.
 *
 * @param b           The bit buffer.
 * @return            A string of the content in the array in the
 *                    bitset.
 */
char *bit_buffer_array_to_string(const bit_buffer *const b);

/*
 * @brief             Print information contained in the bit buffer.
 *
 * @param b           The bit buffer.
 * @return            -
 */
void bit_buffer_print(const bit_buffer *const b);

/*
 * @brief             Converts the contents in the bit buffer to a
 *                    string. The string will contain the content of
 *                    the bit buffer (see the corresponding .c file
 *                    for more informaiton). Memory is allocated for
 *                    the string. The user is responsible for
 *                    deallocating the returned string.
 *
 * @param b           The bit buffer.
 * @return            A string of the content in the bitset.
 */
char *bit_buffer_to_string(const bit_buffer *const b);
