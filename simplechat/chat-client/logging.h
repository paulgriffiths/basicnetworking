/*!
 * \file            logging.h
 * \brief           Interface to logging functionality.
 * \details         Interface to logging functionality. Enables debugging
 * and other system messages to be recorded to a log file.
 * \author          Paul Griffiths
 * \copyright       Copyright 2014 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */

#ifndef PG_CHAT_CLIENT_LOGGING_H
#define PG_CHAT_CLIENT_LOGGING_H

/*!
 * \brief           Turns logging on or off.
 * \details         Turns logging on or off.
 * \param status    `true` to turn logging on, `false` to turn logging off.
 */
void set_logging(const bool status);

/*!
 * \brief           Logs a message to the log file.
 * \details         Logs a message to the log file.
 * \param format    Format string, in same format as `printf()`.
 * \param ...       Variable arguments as specified by format string.
 */
void log_msg(const char * format, ...);

#endif      /*  PG_CHAT_CLIENT_LOGGING_H  */
