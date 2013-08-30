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


/*!
 * \brief           Reads a '\n' terminated line from a socket.
 * \details         The function will not overwrite the buffer, so
 * `max_len` should be the size of the whole buffer, and function will
 * at most write `max_len - 1` characters plus the terminating `'\0'`.
 * \param socket File description of the socket
 * \param buffer The buffer into which to read
 * \param max_len The maximum number of characters to read, including
 * the terminating `'\0'`.
 * \returns         The number of characters read, or -1 on encountering
 * an error.
 */

ssize_t socket_readline(const int socket, char * buffer,
        const size_t max_len) {
    size_t n;
    ssize_t num_read;

    for ( n = 0; n < (max_len - 1); ++n ) {
        num_read = read(socket, &buffer[n], 1);

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
        } else {
            print_errno_message("Error reading from socket");
            return -1;
        }
    }

    num_read = (ssize_t) n;
    return num_read;
}


/*!
 * \brief           Reads a '\n' terminated line from a socket with timeout.
 * \details         Behaves the same as socket_readline(), except it
 * will time out if no input is available on the socket after the
 * specified time.
 * \param socket File description of the socket
 * \param buffer The buffer into which to read
 * \param max_len The maximum number of characters to read, including
 * the terminating `'\0'`.
 * \param time_out A pointer to a `timeval` struct containing the timeout
 * period. Note that some implementations of `select()` may alter this
 * variable, so the calling function should consider it unusable after
 * return. In addition, on such an implementation, the value will
 * specify the cumulative timeout period over the entire read line
 * operation, rather than resetting after reading each character.
 * \returns         The number of characters read, or -1 on encountering
 * an error.
 */

ssize_t socket_readline_timeout(const int socket, char * buffer,
        const size_t max_len, struct timeval * time_out) {
    size_t n;
    ssize_t num_read;
    fd_set socket_set;
    int status;

    FD_ZERO(&socket_set);
    FD_SET(socket, &socket_set);

    for ( n = 0; n < (max_len - 1); ++n ) {
        status = select(socket + 1, &socket_set, NULL, NULL, time_out);
        if ( status == -1 ) {
            print_errno_message("Error calling select()");
            exit(EXIT_FAILURE);
        } else if ( status == 0 ) {
            buffer[n] = '\0';
            return -1;
        }

        num_read = read(socket, &buffer[n], 1);
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


/*!
 * \brief           Writes a line to a socket.
 * \param socket File description of the socket
 * \param buffer The buffer from which to write.
 * \param max_len The maximum number of characters to read from the buffer.
 * \returns         The number of characters written, or -1 on encountering
 * an error.
 */

ssize_t socket_writeline(const int socket, const char * buffer,
        const size_t max_len) {
    size_t num_left = max_len;
    ssize_t num_written;
    const char * buf_ptr = buffer;

    while ( num_left > 0 ) {
        num_written = write(socket, buf_ptr, num_left);

        if ( num_written <= 0 ) {
            if ( errno == EINTR ) {
                num_written = 0;
            } else {
                print_errno_message("Error writing to socket");
                return -1;
            }
        }

        num_left -= num_written;
        buffer += num_written;
    }

    return num_written;
}
