#ifndef __SERIAL_BUFFER_H__
#define __SERIAL_BUFFER_H__

#include <stdint.h>

#define BUFFER_SIZE 1024

struct SerialBuffer
{
	uint8_t buffer[1024];

	uint16_t start;
	uint16_t end;
};

void SerialBufferInit(struct SerialBuffer* buf);
int SerialBufferIsEmpty(struct SerialBuffer* buf);
int SerialBufferIsFull(struct SerialBuffer* buf);
int SerialBufferSize(struct SerialBuffer* buf);
int SerialBufferPush(struct SerialBuffer* buf, uint8_t value);
int16_t SerialBufferPop(struct SerialBuffer* buf);
int SerialBufferPeek(struct SerialBuffer* buf, uint16_t amount);
int SerialBufferCopy(struct SerialBuffer* buf, char* data, uint16_t size);
void SerialBufferReset(struct SerialBuffer* buf);

#endif
