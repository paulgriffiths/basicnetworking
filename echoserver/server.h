/*!
 * \file            server.h
 * \brief           Interface to listening server functions.
 * \details         Interface to listening server functions.
 * \author          Paul Griffiths
 * \copyright       Copyright 2013 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */


#ifndef PG_LISTENING_SERVER_H
#define PG_LISTENING_SERVER_H

#include <inttypes.h>

typedef struct ServerTag {
    int c_socket;
} ServerTag;

uint16_t get_port_from_commandline(const int argc, char ** argv);
int create_server_socket(const uint16_t listening_port);
int start_server(const int listening_socket);

#endif          /*  PG_ECHOSERVER_H  */
