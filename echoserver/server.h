/*!
 * \file            server.h
 * \brief           Interface to listening server functions.
 * \author          Paul Griffiths
 * \copyright       Copyright 2013 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */


#ifndef PG_LISTENING_SERVER_H
#define PG_LISTENING_SERVER_H

#include <inttypes.h>


/*!
 * \brief           Struct for passing to server threads.
 * \details         Contains a file descriptor for the connected socket,
 * as the server obviously needs to know this.
 */

typedef struct ServerTag {
    int c_socket;       /*!< File descriptor for the connected socket */
} ServerTag;


/*  Function prototypes  */

int create_server_socket(const uint16_t listening_port);
int start_server(const int listening_socket);


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

#endif          /*  PG_LISTENING_SERVER_H  */
