/*!
 * \file            helper.h
 * \brief           Interface to helper functions.
 * \details         Interface to helper functions.
 * \author          Paul Griffiths
 * \copyright       Copyright 2013 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */


#ifndef PG_HELPER_H
#define PG_HELPER_H


/*  Function prototypes  */

void mk_errmsg(const char * buffer, char ** error_msg);
void mk_errno_errmsg(const char * buffer, char ** error_msg);


/*  Debug macros  */

#ifdef DEBUG

/*!
 * \brief           Calls printf() only when DEBUG is defined.
 * \param arg       The normal parameters to printf()
 */

# define DPRINTF(arg) printf arg


/*!
 * \brief           Calls fprintf() only when DEBUG is defined.
 * \param arg       The normal parameters to fprintf()
 */

# define DFPRINTF(arg) fprintf arg

#else

# define DPRINTF(arg)
# define DFPRINTF(arg)

#endif          /*  DEBUG  */

#endif          /*  PG_HELPER_H  */
