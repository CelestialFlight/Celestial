#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "celestial.h"

// Tests the basic functionality of the asserts used in Celestial.
void BasicErrorTest(void** state)
{
    (void)state; // Unused.

    int result;

    // Error should return 0 if there was no error.
    result = error(1==1);
    assert_int_equal(result, 0);

    // Error should return 1 if there was an error.
    result = error(1==0);
    assert_int_equal(result, 1);

    // Warning should return 0 if there was no warnings.
    result = warning(1==1);
    assert_int_equal(result, 0);

    // Warning should return 1 if there was an warning.
    result = warning(1==0);
    assert_int_equal(result, 1);
}
