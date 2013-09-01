/*!
 * \file            main.c
 * \brief           Entry point to echoclient.
 * \author          Paul Griffiths
 * \copyright       Copyright 2013 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>
#include <paulgrif/chelpers.h>
#include <paulgrif/socket_helpers.h>


/*!
 * \brief           Maximum character buffer size.
 */

#define MAX_BUFFER_LEN 1024


/*  Function prototypes  */

void run_echo_client(const int c_sock);
int connect_with_command_line_args(int argc, char ** argv);


/*!
 * \brief       Main function.
 * \details     Connects to an echo server and runs the echo client.
 * \returns     Exit status.
 */

int main(int argc, char ** argv) {
    int c_sock;

    ignore_sigpipe();

    if ( (c_sock = connect_with_command_line_args(argc, argv)) == -1 ) {
        return EXIT_FAILURE;
    }

    run_echo_client(c_sock);

    if ( close(c_sock) == -1 ) {
        set_errmsg("error closing socket");
        fprintf(stderr, "echoclient: %s\n", get_errmsg());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}


/*!
 * \brief           Runs the echo client.
 * \param c_sock    File descriptor of the connected socket to use.
 */

void run_echo_client(const int c_sock) {
    char input_buf[MAX_BUFFER_LEN];
    char echo_buf[MAX_BUFFER_LEN];
    int num_bytes;

    while ( TRUE ) {
        printf("Enter the string to echo ('q' to quit): ");
        fflush(stdout);

        if ( fgets(input_buf, MAX_BUFFER_LEN, stdin) == NULL ||
             strcmp(input_buf, "q\n") == 0 ||
             strcmp(input_buf, "Q\n") == 0 ) {
            break;
        }

        num_bytes = socket_writeline(c_sock, input_buf, strlen(input_buf));
        if ( num_bytes == 0 ) {
            fprintf(stderr, "echoclient: no bytes could be written.\n");
            break;
        } else if ( num_bytes < 0 ) {
            fprintf(stderr, "echoclient: %s\n", get_errmsg());
            break;
        }

        num_bytes = socket_readline(c_sock, echo_buf, sizeof(echo_buf));
        if ( num_bytes == 0 ) {
            fprintf(stderr, "echoclient: no bytes could be read.\n");
            break;
        } else if ( num_bytes < 0 ) {
            fprintf(stderr, "echoclient: %s\n", get_errmsg());
            break;
        }

        printf("Echo response: %s\n", echo_buf);
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
    int c_sock;

    /*  Check for correct number of command line arguments  */

    if ( argc != 3 ) {
        fprintf(stdout, "Usage: echoclient [IP/Hostname] [port]\n");
        return ERROR_RETURN;
    }

    /*  Check valid port  */

    if ( (port = port_from_string(argv[2])) == 0 ) {
        fprintf(stdout, "Usage: echoclient [IP/Hostname] [port]\n");
        fprintf(stdout, "Invalid port specified.\n");
        return ERROR_RETURN;
    }

    /*  Try to connect to service  */

    if ( (c_sock = conn_socket_from_string(argv[1], argv[2])) == -1 ) {
        fprintf(stderr, "echoclient: %s\n", get_errmsg());
        return ERROR_RETURN;
    }

    return c_sock;
}
