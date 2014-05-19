/*!
 * \file            logging.c
 * \brief           Implementation of logging functionality.
 * \details         Implementation of logging functionality. Enables debugging
 * and other system messages to be recorded to a log file.
 * \author          Paul Griffiths
 * \copyright       Copyright 2014 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include "logging.h"

/*!  File scope variable for log file name  */
static const char log_file_name[] = "chat-client.log";

/*!  File scope variable for log file pointer  */
static FILE * log_file = NULL;

/*!  File scope variable for current status of logging functionality  */
static bool logging_on = false;

void set_logging(const bool status) {
    if ( status && !logging_on ) {
        log_file = fopen(log_file_name, "w");
        if ( !log_file ) {
            perror("chat-client: couldn't open log file");
            exit(EXIT_FAILURE);
        }
        logging_on = true;
        fprintf(log_file, "Starting to log...\n");
    }
    else if ( !status && logging_on ) {
        fprintf(log_file, "Ending logging...\n");
        fclose(log_file);
        logging_on = false;
    }
}

void log_msg(const char * format, ...) {
    if ( logging_on ) {
        va_list ap;
        va_start(ap, format);
        vfprintf(log_file, format, ap);
        va_end(ap);
    }
}
