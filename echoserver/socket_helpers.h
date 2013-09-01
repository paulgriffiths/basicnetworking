/*!
 * \file            socket_helpers.h
 * \brief           Interface to socket helper functions.
 * \author          Paul Griffiths
 * \copyright       Copyright 2013 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */


#ifndef PG_ECHOSERVER_SOCKET_HELPERS_H
#define PG_ECHOSERVER_SOCKET_HELPERS_H

#include <sys/time.h>
#include <inttypes.h>


/*  Function prototypes  */

ssize_t socket_readline_r(const int l_socket, char * buffer,
        const size_t max_len, char ** error_msg);
ssize_t socket_readline_timeout_r(const int l_socket, char * buffer,
        const size_t max_len, struct timeval * time_out,
        char ** error_msg);
ssize_t socket_writeline_r(const int l_socket, const char * buffer,
        const size_t max_len, char ** error_msg);


#endif          /*  PG_ECHOSERVER_SOCKET_HELPERS_H  */
