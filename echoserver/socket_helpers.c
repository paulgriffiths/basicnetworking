/*!
 * \file            socket_helpers.c
 * \brief           Implementation of socket helper functions.
 * \details         Implementation of socket helper functions.
 * \author          Paul Griffiths
 * \copyright       Copyright 2013 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */


#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>
#include <errno.h>
#include "socket_helpers.h"
#include "helper.h"

int wait_for_single_socket_input(const int l_socket, struct timeval timeout) {
    fd_set socket_set;
    int status;

    FD_ZERO(&socket_set);
    FD_SET(l_socket, &socket_set);

    status = select(l_socket + 1, &socket_set, NULL, NULL, &timeout);
    if ( status == -1 ) {
        print_errno_message("Error calling select()");
        exit(EXIT_FAILURE);
    }

    return status;
}


ssize_t socket_readline(const int l_socket, char * buffer,
        const size_t max_len) {
    size_t n;
    ssize_t num_read;

    for ( n = 0; n < (max_len - 1); ++n ) {
        num_read = read(l_socket, &buffer[n], 1);

        if ( num_read == 1 ) {
            if ( buffer[n] == '\n' ) {
                buffer[++n] = '\0';
                break;
            }
        } else if ( num_read == 0 ) {
            buffer[n] = '\0';
            break;
        } else if ( errno == EINTR ) {
            continue;
            break;
        } else {
            print_errno_message("Error reading from socket");
            return 0;
        }
    }

    num_read = (ssize_t) n;
    return num_read;
}


ssize_t socket_readline_timeout(const int l_socket, char * buffer,
        const size_t max_len, struct timeval * time_out) {
    size_t n;
    ssize_t num_read;
    fd_set socket_set;
    int status;

    FD_ZERO(&socket_set);
    FD_SET(l_socket, &socket_set);

    for ( n = 0; n < (max_len - 1); ++n ) {
        status = select(l_socket + 1, &socket_set, NULL, NULL, time_out);
        if ( status == -1 ) {
            print_errno_message("Error calling select()");
            exit(EXIT_FAILURE);
        } else if ( status == 0 ) {
            buffer[n] = '\0';
            return -1;
        }

        num_read = read(l_socket, &buffer[n], 1);
        if ( num_read == 1 ) {
            if ( buffer[n] == '\n' ) {
                buffer[++n] = '\0';
                break;
            }
        } else if ( num_read == 0 ) {
            buffer[n] = '\0';
            return -1;
        } else if ( errno == EINTR ) {
            continue;
        } else {
            print_errno_message("Error reading from socket");
            exit(EXIT_FAILURE);
        }
    }

    num_read = (ssize_t) n;
    return num_read;
}


ssize_t socket_writeline(const int l_socket, const char * buffer,
        const size_t max_len) {
    size_t num_left = max_len;
    ssize_t num_written;
    const char * buf_ptr = buffer;

    while ( num_left > 0 ) {
        num_written = write(l_socket, buf_ptr, num_left);

        if ( num_written <= 0 ) {
            if ( errno == EINTR ) {
                num_written = 0;
            } else {
                print_errno_message("Error writing to socket");
                return 0;
            }
        }

        num_left -= num_written;
        buffer += num_written;
    }

    return num_written;
}
