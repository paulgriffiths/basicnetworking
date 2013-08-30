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


/*  Function prototypes  */

void print_errno_message(const char * message);


/*  Debug macros  */

#ifdef DEBUG

# define DPRINTF(arg) printf arg
# define DFPRINTF(arg) fprintf arg

#else

# define DPRINTF(arg)
# define DFPRINTF(arg)

#endif          /*  DEBUG  */

#endif          /*  PG_ECHOSERVER_HELPER_H  */
