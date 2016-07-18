#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "celestial.h"

void BasicAngleModeTest(void** state)
{
    (void)state;

    // Initialize controller.
    struct AngleModeBasic angle;
    int result = AngleModeBasicInit(&angle,
        10, 0, 0,
        5, 0, 0,
        1, 0, 0);

    assert_int_equal(result, 0);

    // Update AngleMode P-Controllers
    AngleModeBasicUpdate(&angle,
        1, 1, 1, 0, 0, 0, 1);

    // Error is 1, so the p controllers should return their kP values.
    assert_int_equal(angle.pitchResult, 10);
    assert_int_equal(angle.rollResult, 5);
    assert_int_equal(angle.yawResult, 1);
}
