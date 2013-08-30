/*!
 * \file            server.h
 * \brief           Interface to listening server functions.
 * \details         Interface to listening server functions.
 * \author          Paul Griffiths
 * \copyright       Copyright 2013 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */


#ifndef PG_LISTENING_SERVER_H
#define PG_LISTENING_SERVER_H

#include <inttypes.h>
#include <pthread.h>

#ifdef DEBUG

typedef struct ThreadCount {
    pthread_mutex_t mutex;
    int count;
} ThreadCount;

void increment_thread_count(void);
void decrement_thread_count(void);
int get_thread_count(void);

#endif

#ifdef DEBUG
# define DINCREMENT_THREAD_COUNT(arg) increment_thread_count()
# define DDECREMENT_THREAD_COUNT(arg) decrement_thread_count()
#else
# define DINCREMENT_THREAD_COUNT(arg)
# define DDECREMENT_THREAD_COUNT(arg)
#endif

typedef struct ServerTag {
    int c_socket;
} ServerTag;

uint16_t get_port_from_commandline(const int argc, char ** argv);
int create_server_socket(const uint16_t listening_port);
int start_server(const int listening_socket);

#endif          /*  PG_ECHOSERVER_H  */
