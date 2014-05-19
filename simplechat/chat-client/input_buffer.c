#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "input_buffer.h"

struct input_buffer {
    char * buffer;
    size_t capacity;
    size_t top;
};

struct input_buffer * input_buffer_create(const size_t capacity) {
    struct input_buffer * new_buffer = malloc(sizeof *new_buffer);
    if ( !new_buffer ) {
        perror("chat-client: couldn't allocate input buffer");
        exit(EXIT_FAILURE);
    }

    new_buffer->buffer = calloc(capacity + 1, 1);
    if ( !new_buffer->buffer ) {
        perror("chat-client: couldn't allocate input buffer");
        exit(EXIT_FAILURE);
    }

    new_buffer->capacity = capacity;
    new_buffer->top = 0;

    return new_buffer;
}

void input_buffer_destroy(struct input_buffer * buffer) {
    free(buffer->buffer);
    free(buffer);
}

void input_buffer_push(struct input_buffer * buffer, const char c) {
    assert(input_buffer_is_full(buffer) == false);
    buffer->buffer[buffer->top++] = c;
}

char input_buffer_pop(struct input_buffer * buffer) {
    assert(input_buffer_is_empty(buffer) == false);
    char c = buffer->buffer[--buffer->top];
    buffer->buffer[buffer->top] = '\0';
    return c;
}

size_t input_buffer_capacity(const struct input_buffer * buffer) {
    return buffer->capacity;
}

bool input_buffer_is_full(const struct input_buffer * buffer) {
    return buffer->top >= buffer->capacity;
}

bool input_buffer_is_empty(const struct input_buffer * buffer) {
    return buffer->top == 0;
}

void input_buffer_clear(struct input_buffer * buffer) {
    memset(buffer->buffer, 0, buffer->capacity + 1);
    buffer->top = 0;
}

char * input_buffer_get_buffer(struct input_buffer * buffer) {
    return buffer->buffer;
}

