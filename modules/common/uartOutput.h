#ifndef __UART_OUTPUT_H__
#define __UART_OUTPUT_H__

#include "serialBuffer.h"

struct UartOutput
{
    struct SerialBuffer buf;

    // Baud rate to send data at.
    uint32_t baud;
};

// Sets the baud rate for the uart output.
// Note: If the uart port is already running, it
// is not defined when the baud rate will change.
// This will be depend on implementation.
void UartOutputSetBaud(struct UartOutput* output, uint32_t baud);

// Sends a char over a UART port.
// Note: Depending on the implementation, this may
// be blocking or non-blocking!
void UartOutputSendChar(struct UartOutput* output, char c);

// Sends an integer over a UART port.
void UartOutputSendInt(struct UartOutput* output, int n);

// Sends a double over a UART port.
// Note: This only sends 5 decimal places atm.
void UartOutputSendDouble(struct UartOutput* output, double n);

#endif
