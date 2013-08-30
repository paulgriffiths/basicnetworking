/*!
 * \file            server.c
 * \brief           Implementation of listening server functions.
 * \details         Implementation of listening server functions.
 * \author          Paul Griffiths
 * \copyright       Copyright 2013 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "server.h"
#include "echo_server.h"
#include "helper.h"


static const int backlog = 1024;

#ifdef DEBUG

static ThreadCount thread_count = {PTHREAD_MUTEX_INITIALIZER, 0};

int get_thread_count(void) {
    int status;
    int num_threads;

    status = pthread_mutex_lock(&thread_count.mutex);
    if ( status != 0 ) {
        print_errno_message("Couldn't lock thread count mutex");
        exit(EXIT_FAILURE);
    }

    num_threads = thread_count.count;

    status = pthread_mutex_unlock(&thread_count.mutex);
    if ( status != 0 ) {
        print_errno_message("Couldn't lock thread count mutex");
        exit(EXIT_FAILURE);
    }

    return num_threads;
}


void increment_thread_count(void) {
    int status;

    status = pthread_mutex_lock(&thread_count.mutex);
    if ( status != 0 ) {
        print_errno_message("Couldn't lock thread count mutex");
        exit(EXIT_FAILURE);
    }

    ++thread_count.count;

    status = pthread_mutex_unlock(&thread_count.mutex);
    if ( status != 0 ) {
        print_errno_message("Couldn't lock thread count mutex");
        exit(EXIT_FAILURE);
    }
}

void decrement_thread_count(void) {
    int status;

    status = pthread_mutex_lock(&thread_count.mutex);
    if ( status != 0 ) {
        print_errno_message("Couldn't lock thread count mutex");
        exit(EXIT_FAILURE);
    }

    --thread_count.count;

    status = pthread_mutex_unlock(&thread_count.mutex);
    if ( status != 0 ) {
        print_errno_message("Couldn't lock thread count mutex");
        exit(EXIT_FAILURE);
    }
}

#endif

uint16_t get_port_from_commandline(const int argc, char ** argv) {
    long port_value;
    char * endptr;

    if ( argc < 2 ) {
        fprintf(stderr, "%s: not enough command line arguments.\n", argv[0]);
        return 0;
    } else if ( argc > 2 ) {
        fprintf(stderr, "%s: too many command line arguments.\n", argv[0]);
        return 0;
    }

    port_value = strtol(argv[1], &endptr, 10);
    if ( *endptr != '\0' ) {
        fprintf(stderr, "Usage: %s [listening port number]\n", argv[0]);
        return 0;
    }

    if ( port_value < 1 || port_value > 49151 ) {
        fprintf(stderr, "%s: port number should be in the range [1 - 49151]\n",
                argv[0]);
        return 0;
    }

    return (uint16_t) port_value;
}


int create_server_socket(const uint16_t listening_port) {
    int listening_socket;
    struct sockaddr_in server_address;
    /*struct sockaddr_in6 server_address;*/


    /*if ( (listening_socket = socket(AF_INET6, SOCK_STREAM, 0)) == -1 ) {*/
    if ( (listening_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1 ) {
        print_errno_message("Error opening listening socket");
        return -1;
    }

    memset(&server_address, 0, sizeof(server_address));
    /*server_address.sin6_family = AF_INET6;
    server_address.sin6_addr = in6addr_any;
    server_address.sin6_port = htons(listening_port);*/
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(listening_port);

    if ( bind(listening_socket, (struct sockaddr *) &server_address,
                    sizeof(server_address)) == -1 ) {
        print_errno_message("Error binding listening socket");
        return -1;
    }


    if ( listen(listening_socket, backlog) == -1 ) {
        print_errno_message("Error calling listen()");
        return -1;
    }

    return listening_socket;
}

int start_server(const int listening_socket) {
    int c_socket;
    int status;
    ServerTag * server_tag;
    pthread_t thread_id;

    while ( 1 ) {
        DFPRINTF ((stderr, "Number of active threads: %d\n",
                    get_thread_count()));

        if ( (c_socket = accept(listening_socket, NULL, NULL)) == -1 ) {
            print_errno_message("Error accepting connection");
            return EXIT_FAILURE;
        }

        if ( (server_tag = malloc(sizeof(*server_tag))) == NULL ) {
            print_errno_message("Error allocating server tag");
            return EXIT_FAILURE;
        }

        server_tag->c_socket = c_socket;
        status = pthread_create(&thread_id, NULL, echo_server, server_tag);
        if ( status != 0 ) {
            print_errno_message("Error creating thread");
            return EXIT_FAILURE;
        }
    }
}

