/*!
 * \file            server.c
 * \brief           Implementation of listening server functions.
 * \author          Paul Griffiths
 * \copyright       Copyright 2013 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <netinet/in.h>
#include "server.h"
#include "echo_server.h"
#include "helper.h"


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
 * \brief           Creates a listening socket.
 * \details         The function creates an IPv4 socket by default, but
 * creates an IPv6 socket if the IPV6 preprocessor macro is defined.
 * \param listening_port The port the socket should listen on
 * \returns         The file descriptor of the created listening socket
 * on success, or -1 on encountering an error.
 */

int create_server_socket(const uint16_t listening_port) {
    char * error_msg;

#ifdef IPV6
    struct sockaddr_in6 server_address;
    int listening_socket = socket(AF_INET6, SOCK_STREAM, 0);
#else
    struct sockaddr_in server_address;
    int listening_socket = socket(AF_INET, SOCK_STREAM, 0);
#endif

    if ( listening_socket == -1 ) {
        mk_errno_errmsg("Error opening listening socket", &error_msg);
        fprintf(stderr, "%s\n", error_msg);
        free(error_msg);
        return -1;
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
        mk_errno_errmsg("Error binding listening socket", &error_msg);
        fprintf(stderr, "%s\n", error_msg);
        free(error_msg);
        return -1;
    }

    if ( listen(listening_socket, backlog) == -1 ) {
        mk_errno_errmsg("Error calling listen()", &error_msg);
        fprintf(stderr, "%s\n", error_msg);
        free(error_msg);
        return -1;
    }

    return listening_socket;
}


/*!
 * \brief           Starts an active server.
 * \details         Connections are passed to a new server thread.
 * \param listening_socket A file descriptor for a listening socket.
 * \returns         Returns non-zero on encountering an error. The
 * server runs in an infinite loop, and this function will not return
 * unless an error is countered.
 */

int start_server(const int listening_socket) {
    ServerTag * server_tag;
    pthread_t thread_id;
    char * error_msg;
    int failure_code = 0;
    int conn_socket;

    while ( failure_code == 0 ) {
        DFPRINTF ((stderr, "Number of active threads: %d\n",
                    get_thread_count()));

        if ( (conn_socket = accept(listening_socket, NULL, NULL)) == -1 ) {
            mk_errno_errmsg("Error accepting connection", &error_msg);
            fprintf(stderr, "%s\n", error_msg);
            free(error_msg);
            failure_code = EXIT_FAILURE;
            break;
        }

        if ( (server_tag = malloc(sizeof(*server_tag))) == NULL ) {
            mk_errno_errmsg("Error allocating server tag", &error_msg);
            fprintf(stderr, "%s\n", error_msg);
            free(error_msg);
            failure_code = EXIT_FAILURE;
            break;
        }

        server_tag->c_socket = conn_socket;
        if ( pthread_create(&thread_id, NULL, echo_server, server_tag) != 0 ) {
            mk_errno_errmsg("Error creating thread", &error_msg);
            fprintf(stderr, "%s\n", error_msg);
            free(error_msg);
            free(server_tag);
            failure_code = EXIT_FAILURE;
            break;
        }
    }

    return failure_code;
}


#ifdef DEBUG

/*!
 * \brief           Struct to synchronize access to the active thread count.
 */

typedef struct ThreadCount {
    pthread_mutex_t mutex;      /*!< Mutex for synchronized access */
    int count;                  /*!< Active thread count variable */
} ThreadCount;


/*!
 * \brief           File scope variable holding the active thread count.
 * \details         This variable is accessed and manipulated solely
 * through the get_thread_count(), increment_thread_count(), and
 * decrement_thread_count() functions.
 */

static ThreadCount thread_count = {PTHREAD_MUTEX_INITIALIZER, 0};


/*!
 * \brief           Gets the active thread count.
 * \details         Used for debugging purposes to check that threads
 * are exiting and being destroyed when expected.
 * \returns         The number of active threads (excluding the main
 * thread).
 */

int get_thread_count(void) {
    int num_threads;
    char * error_msg;

    if ( pthread_mutex_lock(&thread_count.mutex) != 0 ) {
        mk_errmsg("Couldn't lock thread count mutex", &error_msg);
        fprintf(stderr, "%s\n", error_msg);
        free(error_msg);
        exit(EXIT_FAILURE);
    }

    num_threads = thread_count.count;

    if ( pthread_mutex_unlock(&thread_count.mutex) != 0 ) {
        mk_errmsg("Couldn't unlock thread count mutex", &error_msg);
        fprintf(stderr, "%s\n", error_msg);
        free(error_msg);
        exit(EXIT_FAILURE);
    }

    return num_threads;
}


/*!
 * \brief           Increments the active thread count.
 * \details         Used for debugging purposes to check that threads
 * are exiting and being destroyed when expected.
 */

void increment_thread_count(void) {
    char * error_msg;

    if ( pthread_mutex_lock(&thread_count.mutex) != 0 ) {
        mk_errmsg("Couldn't lock thread count mutex", &error_msg);
        fprintf(stderr, "%s\n", error_msg);
        free(error_msg);
        exit(EXIT_FAILURE);
    }

    ++thread_count.count;

    if ( pthread_mutex_unlock(&thread_count.mutex) != 0 ) {
        mk_errmsg("Couldn't lock thread count mutex", &error_msg);
        fprintf(stderr, "%s\n", error_msg);
        free(error_msg);
        exit(EXIT_FAILURE);
    }
}


/*!
 * \brief           Decrements the active thread count.
 * \details         Used for debugging purposes to check that threads
 * are exiting and being destroyed when expected.
 */

void decrement_thread_count(void) {
    char * error_msg;

    if ( pthread_mutex_lock(&thread_count.mutex) != 0 ) {
        mk_errmsg("Couldn't lock thread count mutex", &error_msg);
        fprintf(stderr, "%s\n", error_msg);
        free(error_msg);
        exit(EXIT_FAILURE);
    }

    --thread_count.count;

    if ( pthread_mutex_unlock(&thread_count.mutex) != 0 ) {
        mk_errmsg("Couldn't lock thread count mutex", &error_msg);
        fprintf(stderr, "%s\n", error_msg);
        free(error_msg);
        exit(EXIT_FAILURE);
    }
}

#endif
