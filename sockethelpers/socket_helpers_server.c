/*!
 * \file            socket_helpers_server.c
 * \brief           Implementation of server helper functions.
 * \details         Implementation of server helper functions.
 * \author          Paul Griffiths
 * \copyright       Copyright 2013 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <netinet/in.h>
#include <paulgrif/chelpers.h>
#include "socket_helpers_server.h"


/*!
 * \brief           Create an IPv6 rather than IPv4 listening socket.
 */

#define IPV6


/*!
 * \brief           File scope variable for default backlog.
 * \details         Determines the maximum length to which the queue of
 * pending connections may grow. Used when calling listen().
 */

static const int backlog = 1024;


/*!
 * \brief           Creates a TCP listening socket.
 * \details         The function creates an IPv4 socket by default, but
 * creates an IPv6 socket if the IPV6 preprocessor macro is defined.
 * \param listening_port The port the socket should listen on
 * \returns         The file descriptor of the created listening socket
 * on success, or -1 on encountering an error.
 */

int create_tcp_server_socket(const uint16_t listening_port) {

#ifdef IPV6
    struct sockaddr_in6 server_address;
    int listening_socket = socket(AF_INET6, SOCK_STREAM, 0);
#else
    struct sockaddr_in server_address;
    int listening_socket = socket(AF_INET, SOCK_STREAM, 0);
#endif

    if ( listening_socket == -1 ) {
        set_errno_errmsg("error opening listening socket");
        return ERROR_RETURN;
    }

    memset(&server_address, 0, sizeof(server_address));

#ifdef IPV6
    server_address.sin6_family = AF_INET6;
    server_address.sin6_addr = in6addr_any;
    server_address.sin6_port = htons(listening_port);
#else
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(listening_port);
#endif

    if ( bind(listening_socket, (struct sockaddr *) &server_address,
                    sizeof(server_address)) == -1 ) {
        set_errno_errmsg("error binding listening socket");
        return ERROR_RETURN;
    }

    if ( listen(listening_socket, backlog) == -1 ) {
        set_errno_errmsg("error calling listen()");
        return ERROR_RETURN;
    }

    return listening_socket;
}


/*!
 * \brief           Starts an active server.
 * \details         Connections are passed to a new server thread.
 * \param listening_socket A file descriptor for a listening socket.
 * \param sfunc     A pointer to a server thread function. The function
 * should return a pointer to void and accept a single pointer to void
 * as an argument, which should be interpreted as a pointer to a
 * `ServerTag` struct.
 * \returns         Returns non-zero on encountering an error. The
 * server runs in an infinite loop, and this function will not return
 * unless an error is countered.
 */

int start_threaded_tcp_server(const int listening_socket,
                              void * (*sfunc)(void *)) {
    ServerTag * server_tag;
    pthread_t thread_id;
    int failure_code = 0;
    int conn_socket;

    while ( failure_code == 0 ) {
        if ( (conn_socket = accept(listening_socket, NULL, NULL)) == -1 ) {
            set_errno_errmsg("Error accepting connection");
            failure_code = ERROR_RETURN;
            break;
        }

        if ( (server_tag = malloc(sizeof(*server_tag))) == NULL ) {
            set_errno_errmsg("Error allocating server tag");
            failure_code = ERROR_RETURN;
            break;
        }

        server_tag->c_socket = conn_socket;
        if ( pthread_create(&thread_id, NULL, sfunc, server_tag) != 0 ) {
            set_errmsg("Error creating thread");
            free(server_tag);
            failure_code = ERROR_RETURN;
            break;
        }
    }

    return failure_code;
}
