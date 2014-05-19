/*!
 * \file            cc_signals.c
 * \brief           Implementation of TGE signals functions.
 * \details         Implementation of TGE signals functions.
 * \author          Paul Griffiths
 * \copyright       Copyright 2014 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */

/*!  POSIX feature test macro  */
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

/*!
 * \brief           Generic signal handler.
 * \details         Generic signal handler.
 * \param signum    The signal number.
 */
static void handler(int signum);

void set_signal_handlers(void) {
    struct sigaction sa;

    /*  Fill in sigaction struct  */

    sa.sa_handler = handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    /*  Set signal handlers for SIGTERM, SIGINT and SIGLARM  */

    if ( sigaction(SIGTERM, &sa, NULL) == -1 ) {
        perror("chat-client: couldn't set SIGTERM handler");
        exit(EXIT_FAILURE);
    }
    if ( sigaction(SIGINT, &sa, NULL) == -1 ) {
        perror("chat-client: couldn't set SIGINT handler");
        exit(EXIT_FAILURE);
    }

    /*  Ignore SIGTSTP  */

    sa.sa_handler = SIG_IGN;
    if ( sigaction(SIGTSTP, &sa, NULL) == -1 ) {
        perror("chat-client: couldn't ignore SIGTSTP");
        exit(EXIT_FAILURE);
    }
}

static void handler(int signum) {
    switch (signum) {
        case SIGTERM:
        case SIGINT:
            /* TODO: handle end signals */
            break;

        default:
            break;
    }
}

