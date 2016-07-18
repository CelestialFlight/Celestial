#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "celestial.h"

// Tests the basic functionality of push/pop/peek/size/etc. functions.
void BasicSerialBufferTest(void** state)
{
    (void)state; // Unused.

    // Create a serial buffer of size 32.
    struct SerialBuffer buf;
    SerialBufferInit(&buf, 32);

    // Check the capacity and that it should be empty by default.
    assert_int_equal(SerialBufferMaxCapacity(&buf), 32);
    assert_int_equal(SerialBufferIsEmpty(&buf), 1);

    // Save formatted text to the buffer.
    SerialBufferPrintf(&buf, "Hello World! %f\n", 3.14152);

    // Ensure the correct number of bytes were saved to the buffer.
    assert_int_equal(SerialBufferSize(&buf), 21);

    // Peek into the buffer.
    assert_int_equal(SerialBufferPeek(&buf, 0), 'H');
    assert_int_equal(SerialBufferPeek(&buf, 7), 'o');

    // Fill the buffer to it's max.
    SerialBufferPrintf(&buf, "Foo! Foo! Foo! Foo!\n");
    assert_int_equal(SerialBufferIsFull(&buf), 1);

    uint8_t i = 0;

    // Keep removing items from the buffer until it's empty.
    while (!SerialBufferIsEmpty(&buf))
    {
        SerialBufferPop(&buf);
        i++;
    }

    // Ensure the correct number of items were removed.
    assert_int_equal(i, 32);

    // Fill the buffer again. (0 = no error).
    for (i = 0; i < 32; i++)
        assert_int_equal(SerialBufferPush(&buf, i), 0);

    // Ensure it's full again.
    assert_int_equal(SerialBufferIsFull(&buf), 1);
    assert_int_equal(SerialBufferSize(&buf), 32);

    // Empty the buffer, ensuring all the data was correct.
    for (i = 0; i < 32; i++)
        assert_int_equal(SerialBufferPop(&buf), i);

    assert_int_equal(SerialBufferIsEmpty(&buf), 1);
}

// Callbacks used in the callback test.
int printfCount = 0;
int charCount = 0;
int forceSendCount = 0;
static void callback_printf(void) { printfCount++; }
static void callback_char(void) { charCount++; }
static void callback_forcesend(void) { forceSendCount++; }

// Tests that the callbacks are called correctly.
void UseCallbacksSerialBufferTests(void** state)
{
    (void)state; // Unused.

    // Create a serial buffer of size 32.
    struct SerialBuffer buf;
    SerialBufferInit(&buf, 32);

    // Set the printf callback.  This will be called after every
    // buffer to this printf.
    SerialBufferSetPrintfCallback(&buf, &callback_printf);

    // Set the force save callback.  This is used when the buffer
    // should be sent over whichever medium it's set for.
    SerialBufferSetForceSendCallback(&buf, &callback_forcesend);

    // Set the send char callback.  This is called everytime a
    // char is added to the buffer.
    SerialBufferSetSendCharCallback(&buf, &callback_char);

    // Save data to the buffer.
    SerialBufferPrintf(&buf, "Test1234");

    // Ensure the callbacks were called correctly.
    assert_int_equal(printfCount, 1);
    assert_int_equal(charCount, 8);
    assert_int_equal(forceSendCount, 0);

    // Call force save..
    SerialBufferForceSend(&buf);
    assert_int_equal(printfCount, 1);
    assert_int_equal(charCount, 8);
    assert_int_equal(forceSendCount, 1);
}
