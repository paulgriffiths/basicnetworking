/*!
 * \file            socket_helpers_main.c
 * \brief           Implementation of main socket helper functions.
 * \author          Paul Griffiths
 * \copyright       Copyright 2013 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <inttypes.h>
#include <unistd.h>
#include <signal.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>
#include <paulgrif/chelpers.h>
#include "socket_helpers.h"


/*!
 * \brief           Maximum character buffer size.
 */

#define MAX_BUFFER_SIZE 1024


/*!
 * \brief           Reads an `\r\n` terminated line from a socket.
 * \details         The function will not overwrite the buffer, so
 * `max_len` should be the size of the whole buffer, and function will
 * at most write `max_len - 1` characters plus the terminating `\0`.
 * Any terminating CR or LF characters will be stripped.
 * \param socket File description of the socket
 * \param buffer The buffer into which to read
 * \param max_len The maximum number of characters to read, including
 * the terminating `\0`.
 * \returns         The number of characters read, or -1 on encountering
 * an error.
 */

ssize_t socket_readline(const int socket, char * buffer, const size_t max_len) {
    size_t index;
    ssize_t num_read;

    /*  Fill buffer with 0, to avoid having to add terminating NUL  */

    memset(buffer, 0, max_len);

    for ( index = 0; index < (max_len - 1); ++index ) {

        /*  Attempt to read one character  */

        num_read = read(socket, &buffer[index], 1);

        if ( num_read == 1 ) {

            /*  Successfully read a character  */

            if ( index > 0 &&
                 buffer[index] == '\n' &&
                 buffer[index - 1] == '\r' ) {

                /*  End of line, so break  */

                break;
            }
        } else if ( num_read == 0 ) {

            /*  No characters read, but we haven't reached end
                of line so break                                */

            break;
        } else if ( errno == EINTR ) {

            /*  read() got interrupted by a signal, so try again  */

            continue;
        } else {

            /*  Some other error, so set error message and return  */

            set_errno_errmsg("error reading from socket");
            return ERROR_RETURN;
        }
    }

    trim_line_ending(buffer);
    return (ssize_t) index;
}


/*!
 * \brief           Reads an `\r\n` terminated line from a socket with timeout.
 * \details         Behaves the same as socket_readline(), except it
 * will time out if no input is available on the socket after the
 * specified time. Any terminating CR or LF characters will be stripped.
 * \param socket File description of the socket
 * \param buffer The buffer into which to read
 * \param max_len The maximum number of characters to read, including
 * the terminating `\0`.
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
    ssize_t num_read;
    size_t index;
    int status;
    fd_set socket_set;

    /*  Zero and set fd_set  */

    FD_ZERO(&socket_set);
    FD_SET(socket, &socket_set);

    /*  Fill buffer with 0, to avoid having to add terminating NUL  */

    memset(buffer, 0, max_len);

    for ( index = 0; index < (max_len - 1); ++index ) {

        /*  Wait for input for timeout period  */

        status = select(socket + 1, &socket_set, NULL, NULL, time_out);
        if ( status == -1 ) {
            set_errno_errmsg("error calling select()");
            return ERROR_RETURN;
        } else if ( status == 0 ) {

            /*  No data ready after timeout period  */

            break;
        }

        /*  Try to read a single character  */

        num_read = read(socket, &buffer[index], 1);
        if ( num_read == 1 ) {

            /*  Successfully read a character  */

            if ( index > 0 &&
                 buffer[index] == '\n' &&
                 buffer[index - 1] == '\r' ) {

                /*  End of line, so add break  */

                break;
            }
        } else if ( num_read == 0 ) {

            /*  No characters read, but we haven't reached end
                of line so break                                */

            break;
        } else if ( errno == EINTR ) {

            /*  read() got interrupted by a signal, so try again  */

            continue;
        } else {
            
            /*  Some other error, so set error message and return  */

            set_errno_errmsg("error reading from socket");
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
 * \returns         The number of characters written, or -1 on encountering
 * an error.
 */

ssize_t socket_writeline(const int socket, const char * buffer,
        const size_t max_len) {
    size_t num_left = max_len + 2;
    ssize_t num_written, total_written = 0;
    const char * buf_ptr;

    /*  Allocate new buffer with enough room to add \r\n  */

    char * eol_buf = malloc(strlen(buffer) + 3);
    if ( eol_buf == NULL ) {
        set_errno_errmsg("couldn't allocate memory");
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
                set_errno_errmsg("error writing to socket");
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


/*!
 * \brief           Extracts a valid TCP/UDP port from a string.
 * \param port_str  The string from which to extract
 * \returns         The port number on success, or zero if `port_str` does
 * not contain a valid TCP/UDP port (port 0 is reserved and cannot be used).
 */

uint16_t port_from_string(const char * port_str) {
    unsigned long port_value;
    char * endptr;

    port_value = strtoul(port_str, &endptr, 10);
    if ( *endptr != '\0' || port_value < 1 || port_value > 65535 ) {
        port_value = 0;
    }

    return (uint16_t) port_value;
}


/*!
 * \brief           Creates a connected sock from a hostname and port.
 * \param host      A string containing the hostname to which to connect.
 * \param port      A string containing the port to which to connect.
 * \returns         The file descriptor of the connected socket on success,
 * or -1 on failure.
 */

int conn_socket_from_string(const char * host, const char * port) {
    struct addrinfo *result_info, *cri;
    struct addrinfo hints;
    int status, c_sock, connected = 0;

    /*  Set address info hints prior to calling getaddrinfo()  */

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = 0;
    hints.ai_flags = AI_CANONNAME;

    /*  Try to get address info  */

    status = getaddrinfo(host, port, &hints, &result_info);
    if ( status != 0 ) {
        if ( status == EAI_SYSTEM ) {
            set_errno_errmsg("error getting address info");
        } else {

            /*  All other getaddrinfo() errors obtainable
                through gai_strerror()                     */

            char buffer[MAX_BUFFER_SIZE];
            snprintf(buffer, sizeof(buffer),
                    "error getting address info: %d (%s)",
                    status, gai_strerror(status));
            set_errmsg(buffer);
        }
        return ERROR_RETURN;
    }

    /*  Create socket and try to connect  */

    for ( cri = result_info; !connected && cri; cri = cri->ai_next ) {
        struct sockaddr * s_addr = cri->ai_addr;

        if ( (c_sock = socket(s_addr->sa_family, SOCK_STREAM, 0)) != -1 ) {
            if ( connect(c_sock, s_addr, cri->ai_addrlen) == -1) {
                if ( close(c_sock) == -1 ) {
                    set_errno_errmsg("couldn't close socket");
                    return ERROR_RETURN;
                }
                c_sock = ERROR_RETURN;
            } else {
                connected = 1;
            }
        }
    }

    /*  Remember to free the address info we obtained  */

    freeaddrinfo(result_info);

    /*  Set an error message if we couldn't connect to anything  */

    if ( c_sock == ERROR_RETURN ) {
        set_errmsg("unable to connect to service");
    }

    return c_sock;
}


/*!
 * \brief           Ignores the SIGPIPE signal.
 * \details         The `write()` system call will, when writing to a closed
 * socket, elicit an RST (reset) flag. A second `write()` system call will
 * trigger a `SIGPIPE` signal to be raised. The default action of `SIGPIPE`
 * is to terminate the program, with no error message, which is not
 * desirable. If we want to do anything special when `SIGPIPE` is triggered,
 * we could set up a handler, but if we don't, then ignoring `SIGPIPE` is
 * fine, provided our socket functions respond appropriately to the
 * condition (`write()` will return `EPIPE` after an ignored `SIGPIPE` signal).
 */

void ignore_sigpipe(void) {
    struct sigaction sa;

    sa.sa_handler = SIG_IGN;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    sigaction(SIGPIPE, &sa, NULL);
}
