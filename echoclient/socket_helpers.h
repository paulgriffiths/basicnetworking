/*!
 * \file            socket_helpers.h
 * \brief           Interface to socket helper functions.
 * \author          Paul Griffiths
 * \copyright       Copyright 2013 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */


#ifndef PG_SOCKET_HELPERS_H
#define PG_SOCKET_HELPERS_H

#include <sys/time.h>
#include <inttypes.h>


/*  Function prototypes  */

ssize_t socket_readline(const int l_socket, char * buffer,
        const size_t max_len, char ** error_msg);
ssize_t socket_readline_timeout(const int l_socket, char * buffer,
        const size_t max_len, struct timeval * time_out,
        char ** error_msg);
ssize_t socket_writeline(const int l_socket, const char * buffer,
        const size_t max_len, char ** error_msg);
uint16_t port_from_string(const char * port_str);
int conn_socket_from_string(const char * host, const char * port,
                            char ** error_msg);
void ignore_sigpipe(void);


#endif          /*  PG_SOCKET_HELPERS_H  */
