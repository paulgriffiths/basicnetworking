#ifndef PG_CHAT_CLIENT_TEST_LOGGING_H
#define PG_CHAT_CLIENT_TEST_LOGGING_H

#include <stdbool.h>

void tests_log_test(const bool success, const char * fmt, ...);
int tests_get_total_tests(void);
int tests_get_successes(void);
int tests_get_failures(void);

#endif      /*  PG_CHAT_CLIENT_TEST_LOGGING_H  */

