#ifndef PG_CHAT_CLIENT_TEST_MESSAGES_H
#define PG_CHAT_CLIENT_TEST_MESSAGES_H

#include <stdbool.h>
#include "messages.h"

void test_messages(void);
bool test_message_id(const char * str,
                     const enum chatc_msg id,
                     const bool reverse);
bool test_message_param(const char * str,
                        const int param,
                        const char * expected,
                        const bool reverse);

#endif      /*  PG_CHAT_CLIENT_TEST_MESSAGES_H  */
