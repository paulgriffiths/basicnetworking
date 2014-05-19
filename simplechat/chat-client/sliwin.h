#ifndef PG_SLIWIN_H
#define PG_SLIWIN_H

#include <curses.h>


struct sliwin * sliwin_create(WINDOW * parent,
                              const int x,
                              const int y,
                              const int cols,
                              const int rows);
void sliwin_destroy(struct sliwin * sliwin);
void sliwin_refresh(struct sliwin * sliwin);
void sliwin_get_char(struct sliwin * sliwin);
bool sliwin_message_ready(const struct sliwin * sliwin);
void sliwin_message_used(struct sliwin * sliwin);
char * sliwin_get_contents(struct sliwin * sliwin);

#endif      /*  PG_SLIWIN_H  */

