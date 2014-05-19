#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include "test_logging.h"

static int test_successes = 0;
static int test_failures = 0;
static int total_tests = 0;
static bool show_failures = true;

void tests_log_test(const bool success, const char * fmt, ...) {
    ++total_tests;
    if ( success ) {
        ++test_successes;
    }
    else {
        ++test_failures;
    }

    if ( show_failures && !success ) {
        fprintf(stderr, "Failure (%d): ", total_tests);
        va_list ap;
        va_start(ap, fmt);
        vfprintf(stderr, fmt, ap);
        va_end(ap);
        fprintf(stderr, "\n");
    }
}

int tests_get_total_tests(void) {
    return total_tests;
}

int tests_get_successes(void) {
    return test_successes;
}

int tests_get_failures(void) {
    return test_failures;
}

