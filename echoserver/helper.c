/*!
 * \file            helper.c
 * \brief           Implementation of helper functions.
 * \details         Implementation of helper functions.
 * \author          Paul Griffiths
 * \copyright       Copyright 2013 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "helper.h"


void print_errno_message(const char * message) {
    char buffer[1024];

    if ( strerror_r(errno, buffer, sizeof(buffer)) != 0 ) {
        exit(EXIT_FAILURE);
    }

    fprintf(stderr, "Error: %s [(%d) %s]\n", message, errno, buffer);
}
