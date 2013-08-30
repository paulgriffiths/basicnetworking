/*!
 * \file            echo_server.c
 * \brief           Implementation of echo server functions.
 * \author          Paul Griffiths
 * \copyright       Copyright 2013 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "server.h"
#include "echo_server.h"
#include "helper.h"
#include "socket_helpers.h"


/*!
 * \brief           Maximum character buffer size/
 */

#define MAX_BUFFER_LEN 1024


/*!
 * \brief           File scope variable for default time out seconds.
 */

static const long time_out_secs = 5;


/*!
 * \brief           File scope variable for default time out microseconds.
 */
static const long time_out_usecs = 0;


/*!
 * \brief           File scope variable for timeout message.
 */

static const char time_out_msg[] = "Timeout - closing connection.\n";


/*!
 * \brief           Main echo server handler thread function.
 * \details         Provides echo server service to a provided connected
 * socket. The server loops and echoes any whole lines provided. The
 * server will time-out after a pre-defined period, if no input, or if
 * no more input, is received.
 * \param arg Pointer to a ServerTag struct
 * \returns         NULL
 */

void * echo_server(void * arg) {
    int status;
    char buffer[MAX_BUFFER_LEN];
    ServerTag * server_tag = arg;
    int c_socket = server_tag->c_socket;
    ssize_t num_read;
    struct timeval time_out;

    /*  Free struct allocated by calling function before we do
        anything that might cause us to exit()                   */

    free(server_tag);

    DINCREMENT_THREAD_COUNT();
    DFPRINTF ((stderr, "Entering thread - number of active threads is %d.\n",
            get_thread_count()));

    /*  Detach thread, the main server doesn't wait for it */

    status = pthread_detach(pthread_self());
    if ( status != 0 ) {
        print_errno_message("Error detaching thread.");
        exit(EXIT_FAILURE);
    }

    /*  Loop over input lines  */

    while ( 1 ) {

        /*  Note: Linux modifies the struct timeval on a call to
            select(), so we need to reset it before each call.    */

        time_out.tv_sec = time_out_secs;
        time_out.tv_usec = time_out_usecs;

        num_read = socket_readline_timeout(c_socket, buffer,
                MAX_BUFFER_LEN, &time_out);

        if ( num_read == -1 ) {

            /*  We've timed out getting a line of input  */

            DFPRINTF ((stderr, "No input available.\n"));
            socket_writeline(c_socket, time_out_msg, strlen(time_out_msg));
            break;
        }

        /*  Echo the line of input  */

        DFPRINTF ((stderr, "Echoing input.\n"));
        socket_writeline(c_socket, buffer, strlen(buffer));
    }

    if ( close(c_socket) == - 1 ) {
        print_errno_message("Error closing socket");
        exit(EXIT_FAILURE);
    }

    DFPRINTF ((stderr, "Exiting from thread.\n"));
    DDECREMENT_THREAD_COUNT();

    return NULL;
}
