#ifndef __SERIAL_BUFFER_H__
#define __SERIAL_BUFFER_H__

#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>

#define _SB_BUFFER_SIZE 1024

struct SerialBuffer
{
	uint8_t buffer[1024];

	uint16_t start;
	uint16_t end;

    // Callback function everytime a char is saved.
    // Depending on the implementation, this may be sent
    // to zero and no callback will be made.
	// Note: This is set as a void pointer type as function pointers
	//       aren't allowed on xc files.  I don't like this solution.
	//       FP's are of course allowed in c files, a cast will have
	//       to be made between void* to void(*)(struct SerialBuffer*)
    void* sendChar;
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

int SerialBufferSaveChar(struct SerialBuffer* buf, char c);
int SerialBufferSaveString(struct SerialBuffer* buf, char* c);
int SerialBufferSaveInt(struct SerialBuffer* buf, int n);
int SerialBufferSaveDouble(struct SerialBuffer* buffer, double n);
int SerialBufferPrintfVargs(struct SerialBuffer* buf, char* s, va_list ap);
int SerialBufferPrintf(struct SerialBuffer* buf, char* s, ...);
#endif
