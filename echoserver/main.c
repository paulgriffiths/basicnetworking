/*!
 * \file            main.c
 * \brief           Main function for echoserver.
 * \author          Paul Griffiths
 * \copyright       Copyright 2013 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */


#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "server.h"


/*  Function prototypes  */

uint16_t get_port_from_commandline(const int argc, char ** argv);


/*!
 * \brief       Main function.
 * \details     Main function.
 * \returns     Exit status.
 */

int main(int argc, char ** argv) {
    uint16_t l_port;
    int l_socket;
    int exit_status;

    if ( (l_port = get_port_from_commandline(argc, argv)) == 0 ) {
        return EXIT_FAILURE;
    }

    if ( (l_socket = create_server_socket(l_port)) == -1 ) {
        return EXIT_FAILURE;
    }

    exit_status = start_server(l_socket);

    return exit_status;
}


/*!
 * \brief       Parses the command line for a specified TCP port.
 * \details     Checks for the existence of a single command line
 * argument, and if one and only one is present, attempts to interpret
 * it as a TCP listening port, between 1 and 49151 (ports above
 * 49151 are ephemeral ports).
 * \param argc The number of command line arguments, passed from main()
 * \param argv The command line arguments, passed from main()
 * \returns The specified TCP port if successful, or 0 on error.
 */

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
