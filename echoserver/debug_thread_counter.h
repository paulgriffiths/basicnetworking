/*!
 * \file            debug_thread_counter.h
 * \brief           Interface to debug thread counter.
 * \details         A utility for counting active threads for debugging
 * purposes.
 * \author          Paul Griffiths
 * \copyright       Copyright 2013 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */


#ifndef PG_DEBUG_THREAD_COUNTER_H
#define PG_DEBUG_THREAD_COUNTER_H


/*  Debugging function prototypes and macros  */

#ifdef DEBUG

void increment_thread_count(void);
void decrement_thread_count(void);
int get_thread_count(void);


/*!
 * \brief           Calls increment_thread_count() only if DEBUG is defined.
 */

# define DINCREMENT_THREAD_COUNT(arg) increment_thread_count()


/*!
 * \brief           Calls decrement_thread_count() only if DEBUG is defined.
 */

# define DDECREMENT_THREAD_COUNT(arg) decrement_thread_count()

#else

# define DINCREMENT_THREAD_COUNT(arg)
# define DDECREMENT_THREAD_COUNT(arg)

#endif          /*  DEBUG  */

#endif          /*  PG_DEBUG_THREAD_COUNTER_H  */
