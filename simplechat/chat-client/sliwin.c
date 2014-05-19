#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>
#include <curses.h>
#include "win_s.h"
#include "sliwin.h"
#include "input_buffer.h"

#define INPUT_BUFFER_LEN 40

struct sliwin {
    WINDOW * parent_window;
    WINDOW * border_window;
    struct win_s * window;
    struct input_buffer * buffer;
    bool message_ready;
};

struct sliwin * sliwin_create(WINDOW * parent,
                              const int x,
                              const int y,
                              const int cols,
                              const int rows) {
    struct sliwin * new_sliwin = malloc(sizeof *new_sliwin);
    if ( !new_sliwin ) {
        perror("chat-client: couldn't allocate sliwin");
        exit(EXIT_FAILURE);
    }
    new_sliwin->parent_window = parent;
    new_sliwin->message_ready = false;

    new_sliwin->border_window = subwin(parent, rows, cols, y, x);
    if ( !new_sliwin->border_window ) {
        perror("chat-client: couldn't create subwindow");
        exit(EXIT_FAILURE);
    }
    box(new_sliwin->border_window, 0, 0);

    struct win_s * new_window = malloc(sizeof *new_window);
    if ( !new_window ) {
        perror("chat-client: couldn't allocate win_s");
        exit(EXIT_FAILURE);
    }

    new_window->window = subwin(new_sliwin->border_window,
                                rows - 2, cols - 2, y + 1, x + 1);
    if ( !new_window->window ) {
        perror("chat-client: couldn't create subwindow");
        exit(EXIT_FAILURE);
    }
    scrollok(new_window->window, FALSE);
    keypad(new_window->window, TRUE);
    new_window->cols = cols;
    new_window->rows = rows;
    new_window->x = x;
    new_window->y = y;

    struct input_buffer * new_buffer = input_buffer_create(INPUT_BUFFER_LEN);

    new_sliwin->window = new_window;
    new_sliwin->buffer = new_buffer;

    return new_sliwin;
}

void sliwin_destroy(struct sliwin * sliwin) {
    input_buffer_destroy(sliwin->buffer);
    delwin(sliwin->window->window);
    delwin(sliwin->border_window);
    free(sliwin->window);
    free(sliwin);
}

void sliwin_refresh(struct sliwin * sliwin) {
    wrefresh(sliwin->border_window);
    wrefresh(sliwin->window->window);
}

void sliwin_get_char(struct sliwin * sliwin) {
    if ( sliwin->message_ready == false ) {
        int c = wgetch(sliwin->window->window);

        if ( isprint(c) ) {
            if ( !input_buffer_is_full(sliwin->buffer) ) {
                waddch(sliwin->window->window, c);
                input_buffer_push(sliwin->buffer, c);
            }
        }
        else if ( c == KEY_BACKSPACE || c == KEY_DC ) {
            if ( !input_buffer_is_empty(sliwin->buffer) ) {
                int x, y;
                getyx(sliwin->window->window, y, x);
                wmove(sliwin->window->window, y, x - 1);
                waddch(sliwin->window->window, ' ');
                wmove(sliwin->window->window, y, x - 1);
                input_buffer_pop(sliwin->buffer);
            }
        }
        else if ( c == KEY_ENTER || c == '\n' ) {
            sliwin->message_ready = true;
            wmove(sliwin->window->window, 0, 0);
            werase(sliwin->window->window);
            sliwin_refresh(sliwin);
        }
        else {
            assert(false);
        }
        sliwin_refresh(sliwin);
    }
}

bool sliwin_message_ready(const struct sliwin * sliwin) {
    return sliwin->message_ready;
}

void sliwin_message_used(struct sliwin * sliwin) {
    sliwin->message_ready = false;
    input_buffer_clear(sliwin->buffer);
}

char * sliwin_get_contents(struct sliwin * sliwin) {
    return input_buffer_get_buffer(sliwin->buffer);
}

