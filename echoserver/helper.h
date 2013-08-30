/*!
 * \file            helper.h
 * \brief           Interface to helper functions.
 * \details         Interface to helper functions.
 * \author          Paul Griffiths
 * \copyright       Copyright 2013 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */


#ifndef PG_ECHOSERVER_HELPER_H
#define PG_ECHOSERVER_HELPER_H

#ifdef DEBUG
# define DPRINTF(arg) printf arg
#else
# define DPRINTF(arg)
#endif

#ifdef DEBUG
# define DFPRINTF(arg) fprintf arg
#else
# define DFPRINTF(arg)
#endif

void print_errno_message(const char * message);

#endif          /*  PG_ECHOSERVER_HELPER_H  */
