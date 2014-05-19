/*!
 * \file            msgwin.h
 * \brief           Interface to message window functions.
 * \details         Interface to message window functions.
 * \author          Paul Griffiths
 * \copyright       Copyright 2014 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */

#ifndef PG_CHAT_CLIENT_MSGWIN_H
#define PG_CHAT_CLIENT_MSGWIN_H

#include <curses.h>

/*!
 * \brief           Creates a message window.
 * \details         Creates a window for displaying messages.
 * \param parent    A pointer to the parent window.
 * \param x         The X-axis origin of the new window.
 * \param y         The Y-axis origin of the new window.
 * \param cols      The number of columns in the new window.
 * \param rows      The number of rows in the new window.
 * \returns         A pointer to the new message window. The caller is
 * responsible for `free()`ing this pointer by calling `msgwin_destroy()`.
 */
struct msgwin * msgwin_create(WINDOW * parent,
                              const int x,
                              const int y,
                              const int cols,
                              const int rows);

/*!
 * \brief           Destroys a message window created by `msgwin_create()`.
 * \details         Destroys a message window created by `msgwin_create()`.
 * \param msgwin    A pointer to the message window to destroy.
 */
void msgwin_destroy(struct msgwin * msgwin);

/*!
 * \brief           Refreshes a message window.
 * \details         Refreshes a message window.
 * \param msgwin    A pointer to the message window to refresh.
 */
void msgwin_refresh(struct msgwin * msgwin);

/*!
 * \brief           Prints a message in a message window.
 * \details         Prints a message in a message window.
 * \param msgwin    A pointer to the message window.
 * \param msg       The message to print.
 */
void msgwin_print_message(struct msgwin * msgwin, const char * msg);

#endif      /*  PG_CHAT_CLIENT_MSGWIN_H  */

