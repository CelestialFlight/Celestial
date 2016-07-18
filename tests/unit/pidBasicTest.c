#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "celestial.h"

void BasicPIDTest(void** state)
{
    (void)state; // Unused.

    // Initialize PID controller.
    struct PIDBasic pid;
    int result = PIDBasicInit(&pid, 2.0, 1.0, 0.5);
    assert_int_equal(result, 0);

    // First Update.
    double feedback = PIDBasicUpdate(&pid, 1.0, 1.0);

    // Error = 1.0 * 2.0 = 2.0
    // Integral = 1.0 * 1.0 = 1.0
    // Derivative = 1.0 * 0.5 = 0.5
    // Feedback = 3.5
    assert_true(feedback == 3.5);

    // Second update
    feedback = PIDBasicUpdate(&pid, 1.0, 1.0);

    // Error = 1.0 * 2.0 = 2.0
    // Integral = 2.0 * 1.0 = 2.0
    // Derivative = 0 * 0.5  = 0
    // Feedback = 4.0
    assert_true(feedback == 4.0);

    // Set maximum output value to be 1.0
    result = PIDBasicSetMaxOutput(&pid, 1.1);
    assert_int_equal(result, 0);

    // Third update, with output limit.
    feedback = PIDBasicUpdate(&pid, 1.0, 1.0);

    // Error = 1.0 * 2.0 = 2.0
    // Integral = 3.0 * 1.0 = 3.0
    // Derivative = 0 * 0.5 = 0
    // Feedback = 5.0 -> Limited to 1.1
    assert_true(feedback == 1.1);

    // Change constants.
    result = PIDBasicSetConstants(&pid, 0, 1, 0);
    assert_int_equal(result, 0);

    // Reset output to no max value.
    result = PIDBasicResetMaxOutput(&pid);
    assert_int_equal(result, 0);

    // Change max integral value.
    result = PIDBasicSetMaxIntegral(&pid, 0.3);
    assert_int_equal(result, 0);

    // Fourth update, with integral limit.
    feedback = PIDBasicUpdate(&pid, 1.0, 1.0);

    // Error =  1.0 * 0.0 = 0.0
    // Integral = 4.0 * 1.0 = 4.0 -> Limit to 0.3
    // Derivative = 0 * 0 = 0.0
    // Feddback = 0.3
    assert_true(feedback == 0.3);
}
