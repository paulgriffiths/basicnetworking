#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string_functions.h"
#include "test_string_functions.h"
#include "test_logging.h"

void test_string_functions(void) {
    test_trim_left("hello", "hello", true);
    test_trim_left("  world", "world", true);
    test_trim_left("  dingo  ", "dingo  ", true);
    test_trim_left("\t  dingo  ", "dingo  ", true);
    test_trim_left(" \n dingo  ", "dingo  ", true);
    test_trim_left("  rock and roll  ", "rock and roll  ", true);
    test_trim_left("hello  ", "hello", false);
    test_trim_left("\thello  ", "hello  ", true);
    test_trim_left("  world", " world", false);
    test_trim_left("  dingo  ", "dingo", false);

    test_trim_right("hello", "hello", true);
    test_trim_right("world  ", "world", true);
    test_trim_right("fish and chips  ", "fish and chips", true);
    test_trim_right("  hyena\t", "  hyena", true);
    test_trim_right("  \npython\t\n", "  \npython", true);
    test_trim_right("pork  ", "pork  ", false);
    test_trim_right("bacon\t", "bacon\t", false);
    test_trim_right("  sausage  ", "sausage", false);

    test_trim("   donkey  ", "donkey", true);
    test_trim("   moon unit  ", "moon unit", true);
    test_trim("\tmule\n", "mule", true);
    test_trim("fish  ", "fish", true);
    test_trim("    space-hopper", "space-hopper", true);
    test_trim("   headache  ", "headache  ", false);
    test_trim("  filigree   ", "  filigree", false);
    test_trim("\nmartian\t", "\nmartian", false);
    test_trim("\nmartian\t", "martian\t", false);

    test_trim_line_ending("offal\r\n", "offal", true);
    test_trim_line_ending("offal\n\r", "offal", true);
    test_trim_line_ending("offal\n", "offal", true);
    test_trim_line_ending("offal\r", "offal", true);
    test_trim_line_ending("big \roffal\r\n", "big \roffal", true);
    test_trim_line_ending("big \noffal\n\r", "big \noffal", true);
    test_trim_line_ending("big \r\noffal\n", "big \r\noffal", true);
    test_trim_line_ending("big \n\roffal\r", "big \n\roffal", true);
    test_trim_line_ending("  offal\r\n", "offal", false);
    test_trim_line_ending("  offal\n\r", "offal", false);
    test_trim_line_ending("  offal\n", "offal", false);
    test_trim_line_ending("  offal\r", "offal", false);
    test_trim_line_ending("  offal  ", "  offal", false);
}

bool test_trim_left(const char * str,
                    const char * expected,
                    const bool reverse) {
    char * test_case = strdup(str);
    if ( !test_case ) {
        perror("test_trim_left: couldn't allocate memory");
        exit(EXIT_FAILURE);
    }

    trim_left(test_case);

    bool test_result;
    if ( !strcmp(test_case, expected) ) {
        test_result = reverse ? true : false;
    }
    else {
        test_result = reverse ? false : true;
    }

    tests_log_test(test_result, "test_trim_left: [%s] and [%s]", str, expected);

    free(test_case);
    return test_result;
}

bool test_trim_right(const char * str,
                     const char * expected,
                     const bool reverse) {
    char * test_case = strdup(str);
    if ( !test_case ) {
        perror("test_trim_right: couldn't allocate memory");
        exit(EXIT_FAILURE);
    }

    trim_right(test_case);

    bool test_result;
    if ( !strcmp(test_case, expected) ) {
        test_result = reverse ? true : false;
    }
    else {
        test_result = reverse ? false : true;
    }

    tests_log_test(test_result,
                   "test_trim_right: [%s] and [%s]", str, expected);

    free(test_case);
    return test_result;
}

bool test_trim(const char * str,
               const char * expected,
               const bool reverse) {
    char * test_case = strdup(str);
    if ( !test_case ) {
        perror("test_trim: couldn't allocate memory");
        exit(EXIT_FAILURE);
    }

    trim(test_case);

    bool test_result;
    if ( !strcmp(test_case, expected) ) {
        test_result = reverse ? true : false;
    }
    else {
        test_result = reverse ? false : true;
    }

    tests_log_test(test_result,
                   "test_trim: [%s] and [%s]", str, expected);

    free(test_case);
    return test_result;
}

bool test_trim_line_ending(const char * str,
                           const char * expected,
                           const bool reverse) {
    char * test_case = strdup(str);
    if ( !test_case ) {
        perror("test_trim_line_ending: couldn't allocate memory");
        exit(EXIT_FAILURE);
    }

    trim_line_ending(test_case);

    bool test_result;
    if ( !strcmp(test_case, expected) ) {
        test_result = reverse ? true : false;
    }
    else {
        test_result = reverse ? false : true;
    }

    tests_log_test(test_result,
                   "test_trim_line_ending: [%s] and [%s]", str, expected);

    free(test_case);
    return test_result;
}

