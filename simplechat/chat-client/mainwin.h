/*!
 * \file            mainwin.h
 * \brief           Interface to main window functions.
 * \details         Interface to main window functions.
 * \author          Paul Griffiths
 * \copyright       Copyright 2014 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */

#ifndef PG_CHAT_CLIENT_MAINWIN_H
#define PG_CHAT_CLIENT_MAINWIN_H

#include <curses.h>

/*!
 * \brief       Initializes the main screen.
 * \details     Initializes the main screen.
 */
void chatc_ui_initialize_screen(void);

/*!
 * \brief       Frees and destroys the main screen.
 * \details     Frees and destroys the main screen.
 */
void chatc_ui_free_screen(void);

/*!
 * \brief       Returns the number of rows in the terminal.
 * \details     Returns the number of rows in the terminal.
 * \returns     The number of rows in the terminal.
 */
int chatc_term_rows(void);

/*!
 * \brief       Returns the number of columns in the terminal.
 * \details     Returns the number of columns in the terminal.
 * \returns     The number of columns in the terminal.
 */
int chatc_term_cols(void);

/*!
 * \brief       Prints a message to the message window.
 * \details     Prints a message to the message window.
 * \param msg   The message to print.
 */
void chatc_ui_print_message(const char * msg);

/*!
 * \brief       Processes pending input.
 * \details     Processes pending input. This function should be called
 * when `select()` or other methods indicate that there is input waiting
 * on the standard input stream.
 */
void chatc_ui_process_input(void);

/*!
 * \brief       Checks if input is ready.
 * \details     Checks if input is ready.
 * \returns     `true` if the user has entered a complete line of input
 * (i.e. has pressed the `RETURN` key) and that input is ready to be
 * retrieved. `false` if input is not ready.
 */
bool chatc_ui_input_ready(void);

/*!
 * \brief       Returns ready input.
 * \details     This function should only be called after
 * `chatc_ui_input_ready()` has returned `true` to indicate that a
 * complete line of input is ready.
 * \returns     A pointer to the complete line of input. This pointer
 * is to statically allocated memory.
 */
const char * chatc_ui_retrieve_input(void);

/*!
 * \brief       Signal that input has been obtained and is no longer needed.
 * \details     This function should be called once input has been obtained
 * (and copied, if necessary) and is no longer needed. After calling this
 * function, the input may be deleted and the interface made ready for new
 * input. After entering a complete line of input, the user will be
 * prevented from entering a new line until this function has been called.
 */
void chatc_ui_message_used(void);

#endif      /*  PG_CHAT_CLIENT_MAINWIN_H  */

