#ifndef __SERIAL_BUFFER_H__
#define __SERIAL_BUFFER_H__

#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>

#define _SB_BUFFER_SIZE 2048

struct SerialBuffer
{
	uint8_t buffer[_SB_BUFFER_SIZE];

	uint16_t start;
	uint16_t end;

	uint8_t lock;

    // Callback function everytime a char is saved.
    // Depending on the implementation, this may be sent
    // to zero and no callback will be made.
	// Note: This is set as a void pointer type as function pointers
	//       aren't allowed on xc files.  I don't like this solution.
	//       FP's are of course allowed in c files, a cast will have
	//       to be made between void* to void(*)(struct SerialBuffer*)
    void* sendChar;

    // Callback function everytime a printf statement is used. This is
    // better to use then sendChar when large bulk of information needs
    // to be sent.
    void* sendPrintf;

    // Callback function everytime the user wants to force to send the
    // entire buffer.
    void* forceSend;
};

void SerialBufferInit(volatile struct SerialBuffer* buf);
int SerialBufferIsEmpty(volatile struct SerialBuffer* buf);
int SerialBufferIsFull(volatile struct SerialBuffer* buf);
int SerialBufferSize(volatile struct SerialBuffer* buf);
int SerialBufferPush(volatile struct SerialBuffer* buf, uint8_t value);
int16_t SerialBufferPop(volatile struct SerialBuffer* buf);
int SerialBufferPeek(volatile struct SerialBuffer* buf, uint16_t amount);
int SerialBufferCopy(volatile struct SerialBuffer* buf, char* data, uint16_t size);
void SerialBufferReset(volatile struct SerialBuffer* buf);

int SerialBufferSaveChar(volatile struct SerialBuffer* buf, char c);
int SerialBufferSaveString(volatile struct SerialBuffer* buf, char* c);
int SerialBufferSaveInt(volatile struct SerialBuffer* buf, int n);
int SerialBufferSaveDouble(volatile struct SerialBuffer* buffer, double n);
int SerialBufferPrintfVargs(volatile struct SerialBuffer* buf, char* s, va_list ap);
int SerialBufferPrintf(volatile struct SerialBuffer* buf, char* s, ...);

void* SerialBufferGetForceSendCallback(volatile struct SerialBuffer* buf);
void SerialBufferSetForceSendCallback(volatile struct SerialBuffer* buf, void* cb);
void* SerialBufferGetPrintfCallback(volatile struct SerialBuffer* buf);
void SerialBufferSetPrintfCallback(volatile struct SerialBuffer* buf, void* cb);

#endif
