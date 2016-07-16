#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

// Available Unit Tests:
//
// Serial Buffer Tests
void BasicSerialBufferTest(void** state);
void UseCallbacksSerialBufferTests(void** state);
// Motor Mixer Tests
void BasicMotorMixerTest(void** state);

// Execute cmocka unit tests.
int main()
{
    const struct CMUnitTest tests[] =
    {
        cmocka_unit_test(BasicSerialBufferTest),
        cmocka_unit_test(UseCallbacksSerialBufferTests),
        cmocka_unit_test(BasicMotorMixerTest),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
