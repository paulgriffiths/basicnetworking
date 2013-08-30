/*!
 * \file            main.c
 * \brief           Main function for echoserver.
 * \details         Main function for echoserver.
 * \author          Paul Griffiths
 * \copyright       Copyright 2013 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */


#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "server.h"


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

