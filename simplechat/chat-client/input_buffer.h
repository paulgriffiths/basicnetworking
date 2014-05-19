#ifndef PG_INPUT_BUFFER_H
#define PG_INPUT_BUFFER_H

#include <stddef.h>
#include <stdbool.h>

struct input_buffer * input_buffer_create(const size_t capacity);
void input_buffer_destroy(struct input_buffer * buffer);
void input_buffer_push(struct input_buffer * buffer, const char c);
char input_buffer_pop(struct input_buffer * buffer);
size_t input_buffer_capacity(const struct input_buffer * buffer);
bool input_buffer_is_full(const struct input_buffer * buffer);
bool input_buffer_is_empty(const struct input_buffer * buffer);
void input_buffer_clear(struct input_buffer * buffer);
char * input_buffer_get_buffer(struct input_buffer * buffer);

#endif      /*  PG_INPUT_BUFFER_H  */
