/*!
 * \file            socket_helpers.c
 * \brief           Implementation of socket helper functions.
 * \author          Paul Griffiths
 * \copyright       Copyright 2013 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/time.h>
#include <sys/select.h>
#include <errno.h>
#include <inttypes.h>
#include <signal.h>
#include <paulgrif/chelpers.h>
#include "socket_helpers.h"


/*!
 * \brief           Maximum character buffer size.
 */

#define MAX_BUFFER_SIZE 1024


/*!
 * \brief           Reads a \\n terminated line from a socket.
 * \details         The function will not overwrite the buffer, so
 * `max_len` should be the size of the whole buffer, and function will
 * at most write `max_len - 1` characters plus the terminating \\0.
 * Any terminating CR or LF characters will be stripped.
 * \param socket File description of the socket
 * \param buffer The buffer into which to read
 * \param max_len The maximum number of characters to read, including
 * the terminating \\0.
 * \param error_msg A pointer to a char pointer which may point to an
 * error message on failure. Set this to NULL to avoid setting an error
 * message.
 * \returns         The number of characters read, or -1 on encountering
 * an error.
 */

ssize_t socket_readline_r(const int socket, char * buffer,
        const size_t max_len, char ** error_msg) {
    size_t index;
    ssize_t num_read;

    for ( index = 0; index < (max_len - 1); ++index ) {

        /*  Attempt to read one character  */

        num_read = read(socket, &buffer[index], 1);

        if ( num_read == 1 ) {

            /*  Successfully read a character  */

            if ( index > 0 &&
                 buffer[index] == '\n' &&
                 buffer[index - 1] == '\r' ) {

                /*  End of line, so add terminating NUL and break  */

                buffer[++index] = '\0';
                break;
            }
        } else if ( num_read == 0 ) {

            /*  No characters read, but we haven't reached end
                of line so add terminating NUL and return error  */

            buffer[index] = '\0';
            mk_errmsg("No bytes to read", error_msg);
            return ERROR_RETURN;
        } else if ( errno == EINTR ) {

            /*  read() got interrupted by a signal, so try again  */

            continue;
        } else {

            /*  Some other error, so set error message and return  */

            mk_errno_errmsg("Error reading from socket", error_msg);
            return ERROR_RETURN;
        }
    }

    trim_line_ending(buffer);

    return (ssize_t) index;
}


/*!
 * \brief           Reads a \\n terminated line from a socket with timeout.
 * \details         Behaves the same as socket_readline(), except it
 * will time out if no input is available on the socket after the
 * specified time. Any terminating CR or LF characters will be stripped.
 * \param socket File description of the socket
 * \param buffer The buffer into which to read
 * \param max_len The maximum number of characters to read, including
 * the terminating \\0.
 * \param time_out A pointer to a `timeval` struct containing the timeout
 * period. Note that some implementations of `select()` may alter this
 * variable, so the calling function should consider it unusable after
 * return. In addition, on such an implementation, the value will
 * specify the cumulative timeout period over the entire read line
 * operation, rather than resetting after reading each character.
 * \param error_msg A pointer to a char pointer which may point to an
 * error message on failure. Set this to NULL to avoid setting an error
 * message.
 * \returns         The number of characters read, or -1 on encountering
 * an error.
 */

ssize_t socket_readline_timeout_r(const int socket, char * buffer,
        const size_t max_len, struct timeval * time_out,
        char ** error_msg) {
    ssize_t num_read;
    size_t index;
    int status;
    fd_set socket_set;

    /*  Zero and set fd_set  */

    FD_ZERO(&socket_set);
    FD_SET(socket, &socket_set);

    for ( index = 0; index < (max_len - 1); ++index ) {

        /*  Wait for input for timeout period  */

        status = select(socket + 1, &socket_set, NULL, NULL, time_out);
        if ( status == -1 ) {
            mk_errno_errmsg("Error calling select()", error_msg);
            return ERROR_RETURN;
        } else if ( status == 0 ) {

            /*  No data ready after timeout period  */

            buffer[index] = '\0';
            break;
        }

        /*  Try to read a single character  */

        num_read = read(socket, &buffer[index], 1);
        if ( num_read == 1 ) {

            /*  Successfully read a character  */

            if ( index > 0 &&
                 buffer[index] == '\n' &&
                 buffer[index - 1] == '\r' ) {

                /*  End of line, so add terminating NUL and break  */

                buffer[++index] = '\0';
                break;
            }
        } else if ( num_read == 0 ) {

            /*  No characters read, but we haven't reached end
                of line so add terminating NUL and return error  */

            buffer[index] = '\0';
            break;
        } else if ( errno == EINTR ) {

            /*  read() got interrupted by a signal, so try again  */

            continue;
        } else {
            
            /*  Some other error, so set error message and return  */

            mk_errno_errmsg("Error reading from socket", error_msg);
            return ERROR_RETURN;
        }
    }

    trim_line_ending(buffer);

    return (ssize_t) index;
}


/*!
 * \brief           Writes a line to a socket.
 * \details         The function adds a network-standard terminating
 * CRLF, so the provided string should not normally end in any newline
 * characters.
 * \param socket File description of the socket
 * \param buffer The buffer from which to write.
 * \param max_len The maximum number of characters to write to the buffer.
 * Due to the addition of CRLF, `max_len + 2` characters may actually
 * be written.
 * \param error_msg A pointer to a char pointer which may point to an
 * error message on failure. Set this to NULL to avoid setting an error
 * message.
 * \returns         The number of characters written, or -1 on encountering
 * an error.
 */

ssize_t socket_writeline_r(const int socket, const char * buffer,
        const size_t max_len, char ** error_msg) {
    size_t num_left = max_len + 2;
    ssize_t num_written, total_written = 0;
    const char * buf_ptr;

    /*  Allocate new buffer with enough room to add \r\n  */

    char * eol_buf = malloc(strlen(buffer) + 3);
    if ( eol_buf == NULL ) {
        mk_errno_errmsg("Couldn't allocate memory", error_msg);
        return ERROR_RETURN;
    }

    sprintf(eol_buf, "%s\r\n", buffer);
    buf_ptr = eol_buf;

    /*  Write characters  */

    while ( num_left > 0 ) {
        num_written = write(socket, buf_ptr, num_left);

        if ( num_written <= 0 ) {
            if ( errno == EINTR ) {
                num_written = 0;
            } else {
                mk_errno_errmsg("Error writing to socket", error_msg);
                free(eol_buf);
                return ERROR_RETURN;
            }
        }

        num_left -= num_written;
        buf_ptr += num_written;
        total_written += num_written;
    }

    free(eol_buf);
    return total_written;
}
