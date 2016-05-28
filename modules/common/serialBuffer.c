#include "serialBuffer.h"

void SerialBufferInit(struct SerialBuffer* buf)
{
	buf->end = 0;
	buf->start = 0;
}

int SerialBufferIsEmpty(struct SerialBuffer* buf)
{
	return buf->start == buf->end ? 1 : 0;
}

int SerialBufferIsFull(struct SerialBuffer* buf)
{
	return (buf->end+1) % BUFFER_SIZE == buf->start ? 1 : 0;
}

int SerialBufferSize(struct SerialBuffer* buf)
{
	// If the end index hasn't overflowed from max to 0
	if (buf->end >= buf->start)
		return buf->end - buf->start;

	// If the end index is less then the start index
	else
		return BUFFER_SIZE - buf->start + buf->end;
}

int SerialBufferPush(struct SerialBuffer* buf, uint8_t value)
{
	if (SerialBufferIsFull(buf)) return 0;

	buf->buffer[buf->end] = value;
	buf->end++;

	if (buf->end >= BUFFER_SIZE) buf->end = 0;

	return 1;
}

int16_t SerialBufferPop(struct SerialBuffer* buf)
{
	if (SerialBufferIsEmpty(buf)) return -1;

	char returnResult = buf->buffer[buf->start];
	buf->start++;

	if (buf->start >= BUFFER_SIZE) buf->start = 0;

	return returnResult;
}

int SerialBufferPeek(struct SerialBuffer* buf, uint16_t amount)
{
	if (SerialBufferIsEmpty(buf)) return -1;

	return buf->buffer[(buf->start+amount) % BUFFER_SIZE];
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
