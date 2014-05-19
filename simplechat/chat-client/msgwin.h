#ifndef PG_MSGWIN_H
#define PG_MSGWIN_H

#include <curses.h>

struct msgwin * msgwin_create(WINDOW * parent,
                              const int x,
                              const int y,
                              const int cols,
                              const int rows);
void msgwin_destroy(struct msgwin * msgwin);
void msgwin_refresh(struct msgwin * msgwin);
void msgwin_print_message(struct msgwin * msgwin, const char * msg);

#endif      /*  PG_SLIWIN_H  */

