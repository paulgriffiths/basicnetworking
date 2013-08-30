/*!
 * \file            echo_server.c
 * \brief           Implementation of echo server functions.
 * \details         Implementation of echo server functions.
 * \author          Paul Griffiths
 * \copyright       Copyright 2013 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */


#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>
#include "server.h"
#include "echo_server.h"
#include "helper.h"
#include "socket_helpers.h"


#define MAX_BUFFER_LEN 1024
static struct timeval time_out = {5, 0};
static const char time_out_msg[] = "Timeout - closing connection.\n";
static const char ready_msg[] = "Ready to get input.\n";
static const char welcome_msg[] = "Ready to get input.\n";

void * echo_server(void * arg) {
    int status;
    char buffer[MAX_BUFFER_LEN];
    ServerTag * server_tag = arg;
    int c_socket = server_tag->c_socket;

    status = pthread_detach(pthread_self());
    if ( status != 0 ) {
        print_errno_message("Error detaching thread.");
        exit(EXIT_FAILURE);
    }

    socket_writeline(c_socket, welcome_msg, strlen(welcome_msg));
    while ( 1 ) {
        /*
        status = wait_for_single_socket_input(c_socket, time_out);
        */
        status = 1;
        if ( status == 0 ) {
            socket_writeline(c_socket, time_out_msg, strlen(time_out_msg));
            break;
        } else {
            socket_readline(c_socket, buffer, MAX_BUFFER_LEN);
            socket_writeline(c_socket, buffer, strlen(buffer));
        }
    }

    if ( close(c_socket) == - 1 ) {
        print_errno_message("Error closing socket");
        exit(EXIT_FAILURE);
    }

    return NULL;
}
