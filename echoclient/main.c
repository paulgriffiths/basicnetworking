/*!
 * \file            main.c
 * \brief           Main function for echoclient.
 * \details         Main function for echoclient.
 * \author          Paul Griffiths
 * \copyright       Copyright 2013 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <inttypes.h>
#include <unistd.h>
#include "socket_helpers.h"
#include "helper.h"


/*!
 * \brief           Maximum character buffer size.
 */

#define MAX_BUFFER_LEN 1024


/*  Function prototypes  */

void run_echo_client(const int conn_socket);
int connect_with_command_line_args(int argc, char ** argv);


/*!
 * \brief       Main function.
 * \details     Connects to an echo server and runs the echo client.
 * \returns     Exit status.
 */

int main(int argc, char ** argv) {
    int conn_socket;

    ignore_sigpipe();

    if ( (conn_socket = connect_with_command_line_args(argc, argv)) == -1 ) {
        return EXIT_FAILURE;
    }

    run_echo_client(conn_socket);

    if ( close(conn_socket) == -1 ) {
        fprintf(stderr, "Error closing socket.\n");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}


/*!
 * \brief           Runs the echo client.
 * \param conn_socket File descriptor of the connected socket to use.
 */

void run_echo_client(const int conn_socket) {
    char buffer[MAX_BUFFER_LEN];
    char echo_buffer[MAX_BUFFER_LEN];
    char * error_msg;
    int num_bytes;

    while ( 1 ) {
        printf("Enter the string to echo ('q' to quit): ");
        fflush(stdout);

        if ( fgets(buffer, MAX_BUFFER_LEN, stdin) == NULL ||
             strcmp(buffer, "q\n") == 0 ||
             strcmp(buffer, "Q\n") == 0 ) {
            break;
        }

        num_bytes = socket_writeline(conn_socket, buffer,
                strlen(buffer), &error_msg);
        if ( num_bytes == 0 ) {
            fprintf(stderr, "No bytes could be written.\n");
            break;
        } else if ( num_bytes < 0 ) {
            fprintf(stderr, "%s\n", error_msg);
            free(error_msg);
            break;
        }

        if ( socket_readline(conn_socket, echo_buffer,
                sizeof(echo_buffer), &error_msg) < 0 ) {
            fprintf(stderr, "%s\n", error_msg);
            free(error_msg);
            break;
        }

        printf("Echo response: %s", echo_buffer);
    }
}


/*!
 * \brief           Attempts to connect to a service specified in cmdline args.
 * \details         Expects `argc` to be `3`, with `argv[1]` specifying a
 * hostname or IP address, and `argv[2]` specifying a valid port.
 * \param argc      Number of command line arguments, passed from main()
 * \param argv      Command line arguments, passed from main()
 * \returns         The file descriptor of the connected socket on success,
 * or -1 on failure.
 */

int connect_with_command_line_args(int argc, char ** argv) {
    uint16_t port;
    int conn_socket;
    char * error_msg;

    /*  Check for correct number of command line arguments  */

    if ( argc != 3 ) {
        fprintf(stderr, "Usage: %s [IP/Hostname] [port]\n", argv[0]);
        return -1;
    }

    /*  Check valid port  */

    if ( (port = port_from_string(argv[2])) == 0 ) {
        fprintf(stderr, "Usage: %s [IP/Hostname] [port]\n", argv[0]);
        fprintf(stderr, "Invalid port specified.\n");
        return -1;
    }

    /*  Try to connect to service  */

    if ( (conn_socket = conn_socket_from_string(argv[1], argv[2],
                                                &error_msg)) == -1 ) {
        fprintf(stderr, "%s\n", error_msg);
        free(error_msg);
        return -1;
    }

    return conn_socket;
}
