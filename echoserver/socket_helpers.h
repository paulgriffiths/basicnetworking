/*!
 * \file            socket_helpers.h
 * \brief           Interface to socket helper functions.
 * \details         Interface to socket helper functions.
 * \author          Paul Griffiths
 * \copyright       Copyright 2013 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */


#ifndef PG_SOCKET_HELPERS_H
#define PG_SOCKET_HELPERS_H

int wait_for_single_socket_input(const int l_socket, struct timeval timeout);
ssize_t socket_readline(const int l_socket, char * buffer,
        const size_t maxlen);
ssize_t socket_writeline(const int l_socket, const char * buffer,
        const size_t maxlen);


#endif          /*  PG_SOCKET_HELPERS_H  */
