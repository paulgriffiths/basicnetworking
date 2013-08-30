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
#include <errno.h>
#include "helper.h"


/*!
 * \brief           Maximum character buffer size.
 */

#define MAX_BUFFER_SIZE 1024


/*!
 * \brief           Prints an errno error message to stderr.
 * \details         This function accesses the standard global variable
 * `errno` and related function `strerror_r` (a POSIX extension) and
 * can be called after returning from a function which sets `errno`
 * and returns with an error code.
 * \param message The error message to show.
 */

void print_errno_message(const char * message) {
    char buffer[MAX_BUFFER_SIZE];

    if ( strerror_r(errno, buffer, sizeof(buffer)) != 0 ) {
        strcpy(buffer, "Undetermined error");
    }

    fprintf(stderr, "Error: %s [(%d) %s]\n", message, errno, buffer);
}
