/*!
 * \file            mainloop.c
 * \brief           Implementation of main loop functions.
 * \details         Implementation of main loop functions.
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
#include "mainwin.h"
#include "messages.h"

/* NOTE: Currently all this function does is takes messages
 * from the user and prints them to the message window. This
 * function will need to be modified once the network functionality
 * is added.
 */

void chatc_main_loop(void) {
    bool keep_going = true;

    while ( keep_going ) {
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);

        int status = select(STDIN_FILENO + 1, &fds, NULL, NULL, NULL);
        if ( status == -1 ) {
            perror("chat-client: error calling select()");
            exit(EXIT_FAILURE);
        }

        if ( FD_ISSET(STDIN_FILENO, &fds) ) {
            chatc_ui_process_input();
        }

        if ( chatc_ui_input_ready() ) {
            const char * text = chatc_ui_retrieve_input();
            struct chat_msg * msg = chatc_parse_message(text);
            char * fmt;
            char * out;
            switch ( msg->msg_id ) {
                case CHAT_MESSAGE_SAY:
                    fmt = "[You said: %s]";
                    out = malloc(strlen(fmt) +
                                 strlen(msg->parameters[0]) + 1);
                    if ( !out ) {
                        perror("chat-client: couldn't allocate memory");
                        exit(EXIT_FAILURE);
                    }
                    sprintf(out, fmt, msg->parameters[0]);
                    chatc_ui_print_message(out);
                    free(out);
                    break;

                case CHAT_MESSAGE_TELL:
                    fmt = "[You told '%s': %s]";
                    out = malloc(strlen(fmt) +
                                 strlen(msg->parameters[0]) +
                                 strlen(msg->parameters[1]) + 1);
                    if ( !out ) {
                        perror("chat-client: couldn't allocate memory");
                        exit(EXIT_FAILURE);
                    }
                    sprintf(out, fmt, msg->parameters[0], msg->parameters[1]);
                    chatc_ui_print_message(out);
                    free(out);
                    break;

                case CHAT_MESSAGE_NICK:
                    fmt = "[You changed your nick to '%s']";
                    out = malloc(strlen(fmt) +
                                 strlen(msg->parameters[0]) + 1);
                    if ( !out ) {
                        perror("chat-client: couldn't allocate memory");
                        exit(EXIT_FAILURE);
                    }
                    sprintf(out, fmt, msg->parameters[0]);
                    chatc_ui_print_message(out);
                    free(out);
                    break;

                case CHAT_MESSAGE_QUIT:
                    chatc_ui_print_message("[Quitting - goodbye!]");
                    keep_going = false;
                    break;

                case CHAT_MESSAGE_BADMSG:
                    chatc_ui_print_message("[Bad message!]");
                    break;
            }

            chatc_ui_message_used();
            chatc_free_message(msg);
        }
    }
}

