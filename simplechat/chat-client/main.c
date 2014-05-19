/*!
 * \file            main.c
 * \brief           Main function for chat-client.
 * \details         Main function for chat-client.
 * \author          Paul Griffiths
 * \copyright       Copyright 2013 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */


#include <stdio.h>
#include <stdlib.h>
#include "mainwin.h"
#include "mainloop.h"
#include "logging.h"

/*!
 * \brief       Main function.
 * \details     Main function.
 * \returns     Exit status.
 */

int main(void) {
    set_logging(true);

    chatc_ui_initialize_screen();
    chatc_main_loop();
    chatc_ui_free_screen();

    set_logging(false);

    return EXIT_SUCCESS;
}

