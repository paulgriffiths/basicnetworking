#ifndef PG_WIN_S_H
#define PG_WIN_S_H

#include <curses.h>

struct win_s {
    WINDOW * window;
    int x;
    int y;
    int cols;
    int rows;
};

#endif      /*  PG_WIN_S_H  */

