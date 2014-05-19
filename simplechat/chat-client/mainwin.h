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
 * \brief       Returns a pointer to the main curses window.
 * \details     Returns a pointer to the main curses window.
 * \returns     A pointer to the main curses window.
 */
WINDOW * get_main_window(void);

/*!
 * \brief       Gets a character input by the player.
 * \details     Gets a character input by the player. This function will
 * not block if no input is ready.
 * \returns     The character input by the player, or -1 if no character
 * was ready.
 */
int get_character(void);

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

void chatc_ui_print_message(const char * msg);
void chatc_ui_process_input(void);
bool chatc_ui_input_ready(void);
char * chatc_ui_retrieve_input(void);
void chatc_ui_message_used(void);

#endif      /*  PG_CHAT_CLIENT_MAINWIN_H  */

