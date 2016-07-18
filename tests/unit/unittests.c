#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

// Serial Buffer Tests
void BasicSerialBufferTest(void** state);
void UseCallbacksSerialBufferTests(void** state);

// Motor Mixer Tests
void BasicMotorMixerTest(void** state);

// Error Tests
void BasicErrorTest(void** state);

// Feedback Tests
void BasicAngleModeTest(void** state);
void BasicPIDTest(void** state);

// Execute cmocka unit tests.
int main()
{
    const struct CMUnitTest tests[] =
    {
        cmocka_unit_test(BasicSerialBufferTest),
        cmocka_unit_test(UseCallbacksSerialBufferTests),
        cmocka_unit_test(BasicMotorMixerTest),
        cmocka_unit_test(BasicErrorTest),
        cmocka_unit_test(BasicAngleModeTest),
        cmocka_unit_test(BasicPIDTest),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
