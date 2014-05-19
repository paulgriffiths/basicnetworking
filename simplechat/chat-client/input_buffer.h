/*!
 * \file            input_buffer.h
 * \brief           Interface to stack-based input buffer.
 * \details         Interface to stack-based input buffer.
 * \author          Paul Griffiths
 * \copyright       Copyright 2014 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */

#ifndef PG_INPUT_BUFFER_H
#define PG_INPUT_BUFFER_H

#include <stddef.h>
#include <stdbool.h>

/*!
 * \brief           Creates a new input buffer.
 * \details         Creates a new input buffer.
 * \param capacity  The desired capacity of the new input buffer.
 * \returns         A pointer to the new input buffer. The caller is
 * responsible for `free()`ing this buffer by calling
 * `input_buffer_destroy()`.
 */
struct input_buffer * input_buffer_create(const size_t capacity);

/*!
 * \brief           Frees an input buffer.
 * \details         Frees an input buffer.
 * \param buffer    The input buffer created with `input_buffer_create()`.
 */
void input_buffer_destroy(struct input_buffer * buffer);

/*!
 * \brief           Pushes a character onto an input buffer.
 * \details         Pushes a character onto an input buffer.
 * \param buffer    The input buffer.
 * \param c         The character to push onto the buffer.
 */
void input_buffer_push(struct input_buffer * buffer, const char c);

/*!
 * \brief           Pops a character from an input buffer.
 * \details         Pops a character from an input buffer.
 * \param buffer    The input buffer.
 * \returns         The popped character.
 */
char input_buffer_pop(struct input_buffer * buffer);

/*!
 * \brief           Returns the capacity of an input buffer.
 * \details         Returns the capacity of an input buffer.
 * \param buffer    The input buffer.
 * \returns         The capacity of the input buffer.
 */
size_t input_buffer_capacity(const struct input_buffer * buffer);

/*!
 * \brief           Checks if an input buffer is full.
 * \details         Checks if an input buffer is full.
 * \param buffer    The input buffer.
 * \returns         `true` if the input buffer is full, `false` otherwise.
 */
bool input_buffer_is_full(const struct input_buffer * buffer);

/*!
 * \brief           Checks if an input buffer is empty.
 * \details         Checks if an input buffer is empty.
 * \param buffer    The input buffer.
 * \returns         `true` if the input buffer is empty, `false` otherwise.
 */
bool input_buffer_is_empty(const struct input_buffer * buffer);

/*!
 * \brief           Clears and empties an input buffer.
 * \details         Clears and empties an input buffer.
 * \param buffer    The input buffer.
 */
void input_buffer_clear(struct input_buffer * buffer);

/*!
 * \brief           Retrieves the text in an input buffer.
 * \details         Retrieves the text in an input buffer.
 * \param buffer    The input buffer.
 * \returns         A pointer to the text in the input buffer. The caller
 * should not modify this text.
 */
const char * input_buffer_get_buffer(struct input_buffer * buffer);

#endif      /*  PG_INPUT_BUFFER_H  */
