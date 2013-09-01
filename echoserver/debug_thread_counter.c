/*!
 * \file            debug_thread_counter.c
 * \brief           Implementation of debug thread counter.
 * \author          Paul Griffiths
 * \copyright       Copyright 2013 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <netinet/in.h>
#include <paulgrif/chelpers.h>
#include "debug_thread_counter.h"


#ifdef DEBUG

/*!
 * \brief           Struct to synchronize access to the active thread count.
 */

typedef struct ThreadCount {
    pthread_mutex_t mutex;      /*!< Mutex for synchronized access */
    int count;                  /*!< Active thread count variable */
} ThreadCount;


/*!
 * \brief           File scope variable holding the active thread count.
 * \details         This variable is accessed and manipulated solely
 * through the get_thread_count(), increment_thread_count(), and
 * decrement_thread_count() functions.
 */

static ThreadCount thread_count = {PTHREAD_MUTEX_INITIALIZER, 0};


/*!
 * \brief           Gets the active thread count.
 * \details         Used for debugging purposes to check that threads
 * are exiting and being destroyed when expected.
 * \returns         The number of active threads (excluding the main
 * thread).
 */

int get_thread_count(void) {
    int num_threads;
    char * error_msg;

    if ( pthread_mutex_lock(&thread_count.mutex) != 0 ) {
        mk_errmsg("Couldn't lock thread count mutex", &error_msg);
        fprintf(stderr, "%s\n", error_msg);
        free(error_msg);
        exit(EXIT_FAILURE);
    }

    num_threads = thread_count.count;

    if ( pthread_mutex_unlock(&thread_count.mutex) != 0 ) {
        mk_errmsg("Couldn't unlock thread count mutex", &error_msg);
        fprintf(stderr, "%s\n", error_msg);
        free(error_msg);
        exit(EXIT_FAILURE);
    }

    return num_threads;
}


/*!
 * \brief           Increments the active thread count.
 * \details         Used for debugging purposes to check that threads
 * are exiting and being destroyed when expected.
 */

void increment_thread_count(void) {
    char * error_msg;

    if ( pthread_mutex_lock(&thread_count.mutex) != 0 ) {
        mk_errmsg("Couldn't lock thread count mutex", &error_msg);
        fprintf(stderr, "%s\n", error_msg);
        free(error_msg);
        exit(EXIT_FAILURE);
    }

    ++thread_count.count;

    if ( pthread_mutex_unlock(&thread_count.mutex) != 0 ) {
        mk_errmsg("Couldn't lock thread count mutex", &error_msg);
        fprintf(stderr, "%s\n", error_msg);
        free(error_msg);
        exit(EXIT_FAILURE);
    }
}


/*!
 * \brief           Decrements the active thread count.
 * \details         Used for debugging purposes to check that threads
 * are exiting and being destroyed when expected.
 */

void decrement_thread_count(void) {
    char * error_msg;

    if ( pthread_mutex_lock(&thread_count.mutex) != 0 ) {
        mk_errmsg("Couldn't lock thread count mutex", &error_msg);
        fprintf(stderr, "%s\n", error_msg);
        free(error_msg);
        exit(EXIT_FAILURE);
    }

    --thread_count.count;

    if ( pthread_mutex_unlock(&thread_count.mutex) != 0 ) {
        mk_errmsg("Couldn't lock thread count mutex", &error_msg);
        fprintf(stderr, "%s\n", error_msg);
        free(error_msg);
        exit(EXIT_FAILURE);
    }
}

#endif
