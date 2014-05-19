#ifndef PG_CHAT_CLIENT_TEST_STRING_FUNCTIONS_H
#define PG_CHAT_CLIENT_TEST_STRING_FUNCTIONS_H

#include <stdbool.h>

void test_string_functions(void);
bool test_trim_left(const char * str,
                    const char * expected,
                    const bool reverse);
bool test_trim_right(const char * str,
                     const char * expected,
                     const bool reverse);
bool test_trim(const char * str,
               const char * expected,
               const bool reverse);
bool test_trim_line_ending(const char * str,
                           const char * expected,
                           const bool reverse);

#endif      /*  PG_CHAT_CLIENT_TEST_STRING_FUNCTIONS_H  */
