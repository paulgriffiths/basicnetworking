/*!
 * \file            win_s.h
 * \brief           Interface to generic window structure.
 * \details         Interface to generic window structure.
 * \author          Paul Griffiths
 * \copyright       Copyright 2014 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */

#ifndef PG_CHAT_CLIENT_WIN_S_H
#define PG_CHAT_CLIENT_WIN_S_H

#include <curses.h>

/*!  Structure to hold a generic window  */
struct win_s {
    WINDOW * window;    /*!<  Pointer to curses window structure        */
    int x;              /*!<  X-axis origin of window                   */
    int y;              /*!<  Y-axis origin of window                   */
    int cols;           /*!<  Number of columns in the window           */
    int rows;           /*!<  Number of rows in the window              */
};

#endif      /*  PG_CHAT_CLIENT_WIN_S_H  */

