#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "celestial.h"

void BasicMotorMixerTest(void** state)
{
    (void)state; // Unused.

    struct MotorMixer mix[] = {
    // Throttle, pitch, roll, yaw
       {1,    1,    -1,    -1}, // BR prop
       {1,   -1,    -1,     1}, // FR prop
       {1,    1,     1,     1}, // BL prop
       {1,   -1,     1,    -1}  // FL prop
    };

    MixMotorValues(mix, 4, 10, 10, 10, 10);

    // Ensure the motors were mixed correctly.
    assert_int_equal(mix[0].result, 0);
    assert_int_equal(mix[1].result, 0);
    assert_int_equal(mix[2].result, 40);
    assert_int_equal(mix[3].result, 0);

    MixMotorValues(mix, 4, 0, 10, 20, 30);

    // Ensure the motors were mixed correctly.
    assert_int_equal(mix[0].result, 0); // -40 is cropped to 0
    assert_int_equal(mix[1].result, 0);
    assert_int_equal(mix[2].result, 60);
    assert_int_equal(mix[3].result, 0); // -20 is cropped to 0

    MixMotorValues(mix, 4, 0, 2000, 0, 0);

    // Ensure the motors were mixed correctly.
    // 2000 -> 1000, -2000 -> 0
    assert_int_equal(mix[0].result, 1000);
    assert_int_equal(mix[1].result, 0);
    assert_int_equal(mix[2].result, 1000);
    assert_int_equal(mix[3].result, 0);

}
