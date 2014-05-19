/*!
 * \file            sliwin.h
 * \brief           Interface to single line input window functions.
 * \details         Interface to single line input window functions.
 * \author          Paul Griffiths
 * \copyright       Copyright 2014 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */

#ifndef PG_CHAT_CLIENT_SLIWIN_H
#define PG_CHAT_CLIENT_SLIWIN_H

#include <curses.h>

/*!
 * \brief           Creates an input window.
 * \details         Creates a window for obtaining single line input.
 * \param parent    A pointer to the parent window.
 * \param x         The X-axis origin of the new window.
 * \param y         The Y-axis origin of the new window.
 * \param cols      The number of columns in the new window.
 * \param rows      The number of rows in the new window.
 * \returns         A pointer to the new input window. The caller is
 * responsible for `free()`ing this pointer by calling `sliwin_destroy()`.
 */
struct sliwin * sliwin_create(WINDOW * parent,
                              const int x,
                              const int y,
                              const int cols,
                              const int rows);

/*!
 * \brief           Destroys an input window created by `sliwin_create()`.
 * \details         Destroys an input window created by `sliwin_create()`.
 * \param sliwin    A pointer to the input window to destroy.
 */
void sliwin_destroy(struct sliwin * sliwin);

/*!
 * \brief           Refreshes an input window.
 * \details         Refreshes an input window.
 * \param sliwin    A pointer to the input window to refresh.
 */
void sliwin_refresh(struct sliwin * sliwin);

/*!
 * \brief           Gets a character of input.
 * \details         Gets a character of input and stores it in the
 * input window's buffer. This function should be called when `select()`
 * or other methods indicate that input is ready on the standard input
 * stream.
 * \param sliwin    A pointer to the input window.
 */
void sliwin_get_char(struct sliwin * sliwin);

/*!
 * \brief           Checks if input is ready.
 * \details         Checks if input is ready.
 * \param sliwin    A pointer to the input window.
 * \returns         `true` if the user has entered a complete line of input
 * (i.e. has pressed the `RETURN` key) and that input is ready to be
 * retrieved. `false` if input is not ready.
 */
bool sliwin_message_ready(const struct sliwin * sliwin);

/*!
 * \brief           Signal that input has been obtained and is no longer needed.
 * \details         This function should be called once input has been obtained
 * (and copied, if necessary) and is no longer needed. After calling this
 * function, the input may be deleted and the interface made ready for new
 * input. After entering a complete line of input, the user will be
 * prevented from entering a new line until this function has been called.
 * \param sliwin    A pointer to the input window.
 */
void sliwin_message_used(struct sliwin * sliwin);

/*!
 * \brief           Returns ready input.
 * \details         This function should only be called after
 * `sliwin_message_ready()` has returned `true` to indicate that a
 * complete line of input is ready.
 * \param sliwin    A pointer to the input window.
 * \returns         A pointer to the complete line of input. This pointer
 * is to statically allocated memory.
 */
const char * sliwin_get_contents(struct sliwin * sliwin);

#endif      /*  PG_CHAT_CLIENT_SLIWIN_H  */

