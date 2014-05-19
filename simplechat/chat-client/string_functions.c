/*!
 * \file            string_functions.c
 * \brief           Implementation of string helper functions.
 * \author          Paul Griffiths
 * \copyright       Copyright 2013 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */


/*!
 * \brief           Enables POSIX library functions
 */

#define _POSIX_C_SOURCE 200809L


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "string_functions.h"

/*!
 * \brief           Trims CR and LF characters from the end of a string.
 * \param buffer    The string to trim.
 * \returns         A pointer to the passed buffer.
 */

char * trim_line_ending(char * buffer) {
    if ( *buffer != '\0' ) {
        int last;
        for ( last = (int) strlen(buffer) - 1;
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
        for ( last = (int) strlen(buffer) - 1;
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
    char * orig_head = buffer;

    while ( *leading != '\0' && isspace(*leading) ) {
        ++leading;
    }

    while ( *leading != '\0' ) {
        *orig_head++ = *leading++;
    }
    *orig_head = '\0';

    return buffer;
}


/*!
 * \brief           Trims leading and trailing whitespace from a string
 * \param buffer    The string to trim.
 * \returns         A pointer to the passed buffer.
 */

char * trim(char * buffer) {
    (void) trim_left(buffer);
    (void) trim_right(buffer);
    return buffer;
}

