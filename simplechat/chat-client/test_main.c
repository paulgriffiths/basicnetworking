#include <stdio.h>
#include <stdlib.h>
#include "test_logging.h"
#include "test_string_functions.h"
#include "test_messages.h"

int main(void) {
    test_string_functions();
    test_messages();

    printf("%d successes and %d failures from %d tests.\n",
           tests_get_successes(), tests_get_failures(),
           tests_get_total_tests());
    return 0;
}

