#include "serialBuffer.h"

void SerialBufferInit(struct SerialBuffer* buf)
{
	buf->end = 0;
	buf->start = 0;
	buf->sendChar = 0;
}

int SerialBufferIsEmpty(struct SerialBuffer* buf)
{
	return buf->start == buf->end ? 1 : 0;
}

int SerialBufferIsFull(struct SerialBuffer* buf)
{
	return (buf->end+1) % _SB_BUFFER_SIZE == buf->start ? 1 : 0;
}

int SerialBufferSize(struct SerialBuffer* buf)
{
	// If the end index hasn't overflowed from max to 0
	if (buf->end >= buf->start)
		return buf->end - buf->start;

	// If the end index is less then the start index
	else
		return _SB_BUFFER_SIZE - buf->start + buf->end;
}

int SerialBufferPush(struct SerialBuffer* buf, uint8_t value)
{
	if (SerialBufferIsFull(buf)) return 0;

	buf->buffer[buf->end] = value;
	buf->end++;

	if (buf->end >= _SB_BUFFER_SIZE) buf->end = 0;

	if (buf->sendChar != 0)
	{
	    void (*callback)(struct SerialBuffer*) = buf->sendChar;
	    callback(buf);
	}

	return 1;
}

int16_t SerialBufferPop(struct SerialBuffer* buf)
{
	if (SerialBufferIsEmpty(buf)) return -1;

	char returnResult = buf->buffer[buf->start];
	buf->start++;

	if (buf->start >= _SB_BUFFER_SIZE) buf->start = 0;

	return returnResult;
}

int SerialBufferPeek(struct SerialBuffer* buf, uint16_t amount)
{
	if (SerialBufferIsEmpty(buf)) return -1;

	return buf->buffer[(buf->start+amount) % _SB_BUFFER_SIZE];
}

int SerialBufferCopy(struct SerialBuffer* buf, char* data, uint16_t amount)
{
	if (amount <= 0) amount = SerialBufferSize(buf);

	for (int i = 0; i < amount; i++)
	{
		data[i] = SerialBufferPeek(buf, i);
	}

	return 1;
}

void SerialBufferReset(struct SerialBuffer* buf)
{
	buf->start = 0;
	buf->end = 0;
}

int SerialBufferSaveChar(struct SerialBuffer* buf, char c)
{
    return SerialBufferPush(buf, c);
}

int SerialBufferSaveString(struct SerialBuffer* buf, char* c)
{
    while (*c != 0)
    {
        SerialBufferPush(buf, *c);
        c++;
    }

    return 0;
}

int SerialBufferSaveInt(struct SerialBuffer* buf, int n)
{
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

int SerialBufferSaveDouble(struct SerialBuffer* buf, double n)
{
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

int SerialBufferPrintfVargs(struct SerialBuffer* buf, char* s, va_list ap)
{
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
    return 0;
}

int SerialBufferPrintf(struct SerialBuffer* buf, char* s, ...)
{
    va_list ap;
    va_start(ap, s);
    return SerialBufferPrintfVargs(buf, s, ap);
    va_end(ap);
}
