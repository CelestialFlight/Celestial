#ifndef __UART_OUTPUT_H__
#define __UART_OUTPUT_H__

#include "serialBuffer.h"
#include <stdarg.h>

struct UartOutput
{
    struct SerialBuffer buf;

    // Baud rate to send data at.
    uint32_t baud;
};

// Initializes uart output.
void UartOutputInit(struct UartOutput* output, uint32_t baud);

// Cleans up the uart port for use later.
void UartOutputDeinit(struct UartOutput* output);

// Sets the baud rate for the uart output.
// Note: If the uart port is already running, it
// is not defined when the baud rate will change.
// This will be depend on implementation.
void UartOutputSetBaud(struct UartOutput* output, uint32_t baud);

// Sends a string over a UART port.
void UartOutputSendString(struct UartOutput* output, char* c);

// Sends a char over a UART port.
// Note: Depending on the implementation, this may
// be blocking or non-blocking!
void UartOutputSendChar(struct UartOutput* output, char c);

// Sends an integer over a UART port.
void UartOutputSendInt(struct UartOutput* output, int n);

// Sends a double over a UART port.
// Note: This only sends 5 decimal places atm.
void UartOutputSendDouble(struct UartOutput* output, double n);

// Sends a standard printf statement over the UART port.
void UartOutputPrintf(struct UartOutput* output, char* c, ...);

#endif
