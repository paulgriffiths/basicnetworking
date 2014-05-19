/*!
 * \file            mainwin.c
 * \brief           Implementation of main window functions.
 * \details         Implementation of main window functions.
 * \author          Paul Griffiths
 * \copyright       Copyright 2014 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */

/*!  POSIX feature test macro  */
#define _POSIX_C_SOURCE 200809L

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <curses.h>
#include "mainwin.h"
#include "sliwin.h"
#include "msgwin.h"
#include "messages.h"

/*!
 * \brief       Structure for main window
 */
struct mainwin {
    WINDOW * window;        /*!<  Pointer to main curses window     */
    struct winsize ws;      /*!<  Contains dimensions of terminal   */
    int old_cursor;         /*!<  To store the old cursor           */
    struct msgwin * msgwin; /*!<  Pointer to message window         */
    struct sliwin * inpwin; /*!<  Pointer to input window           */
};

/*!  File scope variable to hold main game window  */
static struct mainwin mainwin = {NULL, {0, 0, 0, 0}, 0, NULL, NULL};

void chatc_ui_initialize_screen(void) {
    if ( (mainwin.window = initscr()) == NULL ) {
        perror("chat-client: couldn't initialize screen");
        exit(EXIT_FAILURE);
    }

    if ( ioctl(0, TIOCGWINSZ, &mainwin.ws) == -1 ) {
        perror("chat-client: couldn't get terminal size");
        exit(EXIT_FAILURE);
    }

    noecho();
    keypad(mainwin.window, true);
    mainwin.old_cursor = curs_set(0);
    refresh();
    
    const int iwh = 3;
    int rows = chatc_term_rows();
    int cols = chatc_term_cols();

    mainwin.msgwin = msgwin_create(mainwin.window, 0, 0, cols, rows - iwh);
    mainwin.inpwin = sliwin_create(mainwin.window, 0, rows - iwh, cols, iwh);

    msgwin_refresh(mainwin.msgwin);
    sliwin_refresh(mainwin.inpwin);
}

void chatc_ui_free_screen(void) {
    msgwin_destroy(mainwin.msgwin);
    mainwin.msgwin = NULL;
    sliwin_destroy(mainwin.inpwin);
    mainwin.inpwin = NULL;
    delwin(mainwin.window);
    mainwin.window = NULL;
    curs_set(mainwin.old_cursor);
    endwin();
    refresh();
}
    
int chatc_term_rows(void) {
    return mainwin.ws.ws_row;
}

int chatc_term_cols(void) {
    return mainwin.ws.ws_col;
}

void chatc_ui_print_message(const char * msg) {
    msgwin_print_message(mainwin.msgwin, msg);
    msgwin_refresh(mainwin.msgwin);
}

void chatc_ui_process_input(void) {
    sliwin_get_char(mainwin.inpwin);
}

bool chatc_ui_input_ready(void) {
    return sliwin_message_ready(mainwin.inpwin);
}

const char * chatc_ui_retrieve_input(void) {
    return sliwin_get_contents(mainwin.inpwin);
}

void chatc_ui_message_used(void) {
    sliwin_message_used(mainwin.inpwin);
}

