/*!
 * \file            helper.c
 * \brief           Implementation of helper functions.
 * \author          Paul Griffiths
 * \copyright       Copyright 2013 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include "helper.h"


/*!
 * \brief           Maximum character buffer size.
 */

#define MAX_BUFFER_SIZE 1024


/*!
 * \brief           Sets an error message.
 * \details         This function provides a thread-safe way for a
 * function to set an error message.
 * \param buffer    A buffer containing the error message.
 * \param error_msg A pointer to a char pointer which will refer to
 * the error message. This should be free()d by the called.
 */

void mk_errmsg(const char * buffer, char ** error_msg) {
    static const char error_prefix[] = "echoserver error";

    if ( error_msg ) {
        if ( (*error_msg = malloc(strlen(buffer) +
                                  strlen(error_prefix) + 2)) == NULL ) {
            fprintf(stderr, "Couldn't allocate memory for error message.\n");
            exit(EXIT_FAILURE);
        }

        sprintf(*error_msg, "%s: %s", error_prefix, buffer);
    }
}


/*!
 * \brief           Sets an error message based on errno.
 * \details         This function provides a thread-safe way for a
 * function to set an error message, with the usual caveat that
 * errno itself is not threadsafe.
 * \param buffer    A buffer containing the error message.
 * \param error_msg A pointer to a char pointer which will refer to
 * the error message. This should be free()d by the called.
 */

void mk_errno_errmsg(const char * buffer, char ** error_msg) {
    static const char error_prefix[] = "echoserver error";
    char error_buffer[MAX_BUFFER_SIZE];
    char msg_buffer[MAX_BUFFER_SIZE];

    if ( error_msg ) {
        if ( strerror_r(errno, error_buffer, sizeof(error_buffer)) != 0 ) {
            strcpy(error_buffer, "Undetermined error");
        }

        snprintf(msg_buffer, MAX_BUFFER_SIZE, "%s: %s [(%d) %s]",
                 error_prefix, buffer, errno, error_buffer);

        if ( (*error_msg = malloc(strlen(msg_buffer) + 1)) == NULL ) {
            fprintf(stderr, "Couldn't allocate memory for error message.\n");
            exit(EXIT_FAILURE);
        }

        strcpy(*error_msg, msg_buffer);
    }
}


/*!
 * \brief           Trims CR and LF characters from the end of a string.
 * \param buffer    The string to trim.
 * \returns         A pointer to the passed buffer.
 */

char * trim_line_ending(char * buffer) {
    if ( *buffer != '\0' ) {
        int last;
        for ( last = strlen(buffer) - 1;
              last >= 0 && ( buffer[last] == '\n' || buffer[last] == '\r' );
              --last ) {
            buffer[last] = '\0';
        }
    }

    return buffer;
}


/*!
 * \brief           Trims trailing whitespace from a string.
 * \param buffer    The string to trim.
 * \returns         A pointer to the passed buffer.
 */

char * trim_right(char * buffer) {
    if ( *buffer != '\0' ) {
        int last;
        for ( last = strlen(buffer) - 1;
              last >= 0 && isspace(buffer[last]);
              --last ) {
            buffer[last] = '\0';
        }
    }

    return buffer;
}


/*!
 * \brief           Trims leading whitespace from a string.
 * \param buffer    The string to trim.
 * \returns         A pointer to the passed buffer.
 */

char * trim_left(char * buffer) {
    char * leading = buffer;

    while ( *leading != '\0' && isspace(*leading) ) {
        ++leading;
    }

    strcpy(buffer, leading);

    return buffer;
}


/*!
 * \brief           Trims leading and trailing whitespace from a string
 * \param buffer    The string to trim.
 * \returns         A pointer to the passed buffer.
 */

char * trim(char * buffer) {
    trim_left(buffer);
    trim_right(buffer);
    return buffer;
}
