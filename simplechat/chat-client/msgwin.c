#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <curses.h>
#include "win_s.h"
#include "msgwin.h"

struct msgwin {
    WINDOW * parent_window;
    WINDOW * border_window;
    struct win_s * window;
};

struct msgwin * msgwin_create(WINDOW * parent,
                              const int x,
                              const int y,
                              const int cols,
                              const int rows) {
    struct msgwin * new_msgwin = malloc(sizeof *new_msgwin);
    if ( !new_msgwin ) {
        perror("chat-client: couldn't allocate msgwin");
        exit(EXIT_FAILURE);
    }
    new_msgwin->parent_window = parent;

    new_msgwin->border_window = subwin(parent, rows, cols, y, x);
    if ( !new_msgwin->border_window ) {
        perror("chat-client: couldn't create subwindow");
        exit(EXIT_FAILURE);
    }
    box(new_msgwin->border_window, 0, 0);

    struct win_s * new_window = malloc(sizeof *new_window);
    if ( !new_window ) {
        perror("chat-client: couldn't allocate win_s");
        exit(EXIT_FAILURE);
    }

    new_window->window = subwin(new_msgwin->border_window,
                                rows - 2, cols - 2, y + 1, x + 1);
    if ( !new_window->window ) {
        perror("chat-client: couldn't create subwindow");
        exit(EXIT_FAILURE);
    }
    scrollok(new_window->window, TRUE);
    keypad(new_window->window, FALSE);
    new_window->cols = cols;
    new_window->rows = rows;
    new_window->x = x;
    new_window->y = y;
    wmove(new_window->window, 0, 1);

    new_msgwin->window = new_window;

    return new_msgwin;
}

void msgwin_destroy(struct msgwin * msgwin) {
    delwin(msgwin->window->window);
    delwin(msgwin->border_window);
    free(msgwin->window);
    free(msgwin);
}

void msgwin_refresh(struct msgwin * msgwin) {
    wrefresh(msgwin->border_window);
    wrefresh(msgwin->window->window);
}

void msgwin_print_message(struct msgwin * msgwin, const char * msg) {
    waddch(msgwin->window->window, '\n');
    waddstr(msgwin->window->window, msg);
}

