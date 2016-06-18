#include "uartOutput.h"

void UartOutputSendString(struct UartOutput* output, char* c)
{
    SerialBufferSaveString(&output->buf, c);
}

void UartOutputSendChar(struct UartOutput* output, char c)
{
    SerialBufferSaveChar(&output->buf, c);
}

void UartOutputSendInt(struct UartOutput* output, int n)
{
    SerialBufferSaveInt(&output->buf, n);
}

void UartOutputSendDouble(struct UartOutput* output, double n)
{
    SerialBufferSaveDouble(&output->buf, n);
}

void UartOutputPrintf(struct UartOutput* output, char* c, ...)
{
    va_list ap;
    va_start(ap, c);
    SerialBufferPrintfVargs(&output->buf, c, ap);
    va_end(ap);
}
