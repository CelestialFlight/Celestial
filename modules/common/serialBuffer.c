//
#include "serialBuffer.h"

// Returns 0 if succesful. If 1 is returned, the device ran out of memory to
// allocate.
// Note: This should only be used during initialization stages of the program.
int SerialBufferInit(volatile struct SerialBuffer* buf, int bufferSize)
{
    // Defensive check.
    if (buf == 0) return -1;

    buf->end = 0;
    buf->start = 0;
    buf->sendChar = 0;
    buf->sendPrintf = 0;
    buf->forceSend = 0;
    buf->lock = 0;

    // Initiate memory for the buffer. Add one extra space which is used for
    // determining when the buffer is full.
    buf->bufferSize = (bufferSize+1);
    buf->buffer = (uint8_t*)malloc(sizeof(uint8_t)*(bufferSize+1));

    // Return an error if malloc ran out of memory.
    if (buf->buffer == 0) return 1;

    return 0;
}

// Frees up memory that the serial buffer used.
void SerialBufferDeInit(volatile struct SerialBuffer* buf)
{
    if (buf == 0) return; // Defensive check;
    if (buf->buffer == 0) return;

    buf->bufferSize = 0;
    free(buf->buffer);
}

int SerialBufferIsEmpty(volatile struct SerialBuffer* buf)
{
    if (buf == 0) return -1; // Defensive check;
    if (buf->buffer == 0) return -1;

    return buf->start == buf->end ? 1 : 0;
}

int SerialBufferIsFull(volatile struct SerialBuffer* buf)
{
    if (buf == 0) return -1; // Defensive check;
    if (buf->buffer == 0) return -1;

    return (buf->end+1) % buf->bufferSize == buf->start ? 1 : 0;
}

// Returns the amount of buffer space in bytes being used.
int SerialBufferSize(volatile struct SerialBuffer* buf)
{
    if (buf == 0) return -1; // Defensive check;
    if (buf->buffer == 0) return -1;

    // If the end index hasn't overflowed from max to 0
    if (buf->end >= buf->start)
        return buf->end - buf->start;

    // If the end index is less then the start index
    else
        return buf->bufferSize - buf->start + buf->end;
}

// Returns the maximum capacity in bytes for the buffer.
int SerialBufferMaxCapacity(volatile struct SerialBuffer* buf)
{
    if (buf == 0) return -1; // Defensive check;
    if (buf->buffer == 0) return -1;

    return buf->bufferSize - 1;
}

// Adds a byte to the buffer.
int SerialBufferPush(volatile struct SerialBuffer* buf, uint8_t value)
{
    if (buf == 0) return -1; // Defensive check;
    if (buf->buffer == 0) return -1;

    if (SerialBufferIsFull(buf)) return 1;

    // Lock writing to buffer so that there aren't any race issues.
    while (buf->lock == 1);
    buf->lock = 1;

    buf->buffer[buf->end] = value;
    buf->end++;

    if (buf->end >= buf->bufferSize) buf->end = 0;

    // Allow other threads to use serial buffer now.
    // NOTE: A deadlock can occur if the lock is reset after
    //       sendChar callback as that callback is likely
    //       to call push/pop and require the lock.
    buf->lock = 0;

    if (buf->sendChar != 0)
    {
        void (*callback)(volatile struct SerialBuffer*) = buf->sendChar;
        callback(buf);
    }

    return 0;
}

// Removes a byte from the buffer.
int16_t SerialBufferPop(volatile struct SerialBuffer* buf)
{
    if (buf == 0) return -1; // Defensive check;
    if (buf->buffer == 0) return -1;

    if (SerialBufferIsEmpty(buf)) return -1;

    // Lock writing to buffer so that there aren't any race issues.
    while (buf->lock == 1);
    buf->lock = 0;

    char returnResult = buf->buffer[buf->start];
    buf->start++;

    if (buf->start >= buf->bufferSize) buf->start = 0;

    buf->lock = 0;
    return returnResult;
}

// Looks at, without removing, a byte from the buffer.
int SerialBufferPeek(volatile struct SerialBuffer* buf, uint16_t amount)
{
    if (buf == 0) return -1; // Defensive check;
    if (buf->buffer == 0) return -1;

    if (SerialBufferIsEmpty(buf)) return -1;

    return buf->buffer[(buf->start+amount) % buf->bufferSize];
}

// Copies `amount` number of bytes from the serial buffer to the `data` array.
int SerialBufferCopy(volatile struct SerialBuffer* buf, char* data, uint16_t amount)
{
    if (buf == 0) return -1; // Defensive check;
    if (buf->buffer == 0) return -1;

    if (amount <= 0) amount = SerialBufferSize(buf);

    int i;
    for (i = 0; i < amount; i++)
    {
        data[i] = SerialBufferPeek(buf, i);
    }

    return 1;
}

// Resets the buffer without allocating/freeing memory.
void SerialBufferReset(volatile struct SerialBuffer* buf)
{
    if (buf == 0) return; // Defensive check;

    buf->start = 0;
    buf->end = 0;
}

int SerialBufferSaveChar(volatile struct SerialBuffer* buf, char c)
{
    if (buf == 0) return -1; // Defensive check;
    if (buf->buffer == 0) return -1;

    return SerialBufferPush(buf, c);
}

int SerialBufferSaveString(volatile struct SerialBuffer* buf, char* c)
{
    if (buf == 0) return -1; // Defensive check;
    if (buf->buffer == 0) return -1;

    while (*c != 0)
    {
        SerialBufferPush(buf, *c);
        c++;
    }

    return 0;
}

int SerialBufferSaveInt(volatile struct SerialBuffer* buf, int n)
{
    if (buf == 0) return -1; // Defensive check;
    if (buf->buffer == 0) return -1;

    char value[30];
    int index = 0;

    if (n < 0)
    {
        SerialBufferPush(buf, '-');
        n = -n;
    }
    else if (n == 0)
    {
        SerialBufferPush(buf, '0');
        return 0;
    }

    while (n != 0)
    {
        // Convert integer to character.
        value[index] = (char)(n % 10) + '0';
        index++;
        n /= 10;
    }

    while (index != 0)
    {
        index--;
        SerialBufferPush(buf, value[index]);
    }

    return 0;
}

int SerialBufferSaveDouble(volatile struct SerialBuffer* buf, double n)
{
    if (buf == 0) return -1; // Defensive check;
    if (buf->buffer == 0) return -1;

    char value[30];
    int index = 0;

    if (n < 0)
    {
        SerialBufferPush(buf, '-');
        n = -n;
    }
    else if (n == 0)
    {
        SerialBufferPush(buf, '0');
        return 0;
    }

    // Strip the 5 most significant decimals
    int numOfDecimals = 5;
    uint64_t noDecimals = (uint64_t)n;
    double onlyDecimals = n - noDecimals;

    int i;
    for (i = 0; i < numOfDecimals; i++)
    {
        noDecimals *= 10;
        onlyDecimals *= 10;
    }

    noDecimals += onlyDecimals;

    // Treat the number as a decimal.
    while (noDecimals != 0 || index < numOfDecimals)
    {
        value[index] = (char)(noDecimals % 10) + '0';
        index++;
        if (index == numOfDecimals) value[index++] = '.';
        noDecimals /= 10;
    }

    // Write all the characters to the uart buffer.
    while (index != 0)
    {
        index--;
        SerialBufferPush(buf, value[index]);
    }

    return 0;
}

int SerialBufferPrintfVargs(volatile struct SerialBuffer* buf, char* s, va_list ap)
{
    if (buf == 0) return -1; // Defensive check;
    if (buf->buffer == 0) return -1;

    while (*s != 0)
    {
       if (*s == '%')
       {
           s++;

           // Print decimal
           if (*s == 'd')
           {
               SerialBufferSaveInt(buf, va_arg(ap, int));
           }
           // Print double
           else if (*s == 'f')
           {
               SerialBufferSaveDouble(buf, va_arg(ap, double));
           }
           // Print string
           else if (*s == 's')
           {
               SerialBufferSaveString(buf, va_arg(ap, char*));
           }
       }
       // Print regular char
       else
       {
           SerialBufferSaveChar(buf, *s);
       }

       s++;
    }

    // Callback for after a printf statement is used.
    if (buf->sendPrintf != 0)
    {
        void (*callback)(volatile struct SerialBuffer*) = buf->sendPrintf;
        callback(buf);
    }

    return 0;
}

// Tells the peripheral using this serial buffer to try to send all the data
// on the buffer.
void SerialBufferForceSend(volatile struct SerialBuffer* buf)
{
    if (buf == 0) return; // Defensive check;

    if (buf->forceSend != 0)
    {
        void (*callback)(volatile struct SerialBuffer*) = buf->forceSend;
        callback(buf);
    }
}

int SerialBufferPrintf(volatile struct SerialBuffer* buf, char* s, ...)
{
    if (buf == 0) return -1; // Defensive check;
    if (buf->buffer == 0) return -1;

    va_list ap;
    va_start(ap, s);
    return SerialBufferPrintfVargs(buf, s, ap);
    va_end(ap);
}

void SerialBufferSetPrintfCallback(volatile struct SerialBuffer* buf, void* cb)
{
    if (buf == 0) return; // Defensive check;
    buf->sendPrintf = cb;
}

void* SerialBufferGetPrintfCallback(volatile struct SerialBuffer* buf)
{
    if (buf == 0) return 0; // Defensive check;
    return buf->sendPrintf;
}

void SerialBufferSetForceSendCallback(volatile struct SerialBuffer* buf, void* cb)
{
    if (buf == 0) return; // Defensive check;
    buf->forceSend = cb;
}

void* SerialBufferGetForceSendCallback(volatile struct SerialBuffer* buf)
{
    if (buf == 0) return 0; // Defensive check;
    return buf->forceSend;
}

void SerialBufferSetSendCharCallback(volatile struct SerialBuffer* buf, void* cb)
{
    if (buf == 0) return; // Defensive check;
    buf->sendChar = cb;
}

void* SerialBufferGetSendCharCallback(volatile struct SerialBuffer* buf)
{
    if (buf == 0) return 0; // Defensive check;
    return buf->sendChar;
}
