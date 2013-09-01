/*!
 * \file            socket_helpers_server.h
 * \brief           Interface to server helper functions.
 * \details         Interface to server helper functions.
 * \author          Paul Griffiths
 * \copyright       Copyright 2013 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */


#ifndef PG_SOCKET_HELPERS_SERVER_H
#define PG_SOCKET_HELPERS_SERVER_H


#include <inttypes.h>


/*!
 * \brief           Struct for passing to server threads.
 * \details         Contains a file descriptor for the connected socket,
 * as the server obviously needs to know this.
 */

typedef struct ServerTag {
    int c_socket;       /*!< File descriptor for the connected socket */
} ServerTag;


/*  Function prototypes  */

#ifdef __cplusplus
extern "C" {
#endif

int create_tcp_server_socket(const uint16_t listening_port);
int start_threaded_tcp_server(const int listening_socket,
                              void * (*sfunc)(void *));

#ifdef __cplusplus
}
#endif

#endif          /*  PG_SOCKET_HELPERS_SERVER_H  */
