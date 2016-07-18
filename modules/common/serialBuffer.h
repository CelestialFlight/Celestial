#ifndef __SERIAL_BUFFER_H__
#define __SERIAL_BUFFER_H__

#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>

// Error messages.
#define SB_ERROR_NULL_POINTER  -1
#define SB_NO_ERROR             0
#define SB_FULL_BUFFER          1
#define SB_EMPTY_BUFFER         2

// A SerialBuffer is a core datastructure in celestial which is
// implemented as a circular buffer.  It is used for saving a stream
// of data to and from peripherals.  For instance, a datalogger or
// UART port.  Several callbacks are implemented so that individual
// peripherals can execute specific tasks.  For instance, a UART port
// may want to check to see if the UART DMA peripherals is currently
// being used, and if not, activate it to send the data.
struct SerialBuffer
{
    uint8_t* buffer;
    uint32_t bufferSize;

    // Points to the start and the end of the circular buffer.
    uint16_t start;
    uint16_t end;

    // A lock used when pushign and pulling from the buffer.  This
    // ensures that situations where threading is used, there aren't
    // any race issues.
    uint8_t lock;

    // Callback function everytime a char is saved.
    // Depending on the implementation, this may be sent
    // to zero and no callback will be made.
    // Note: This is set as a void pointer type as function pointers
    //       aren't allowed on xc files.  I don't like this solution.
    //       FP's are of course allowed in c files, a cast will have
    //       to be made between void* to void(*)(struct SerialBuffer*)
    // Another Note: Function pointers are heavily discouraged for
    //       safety-critical code, so this should be used lightly and
    //       avoided when possibole.  If a better solution comes up
    //       that avoids FP's, it should be used.
    void* sendChar;

    // Callback function everytime a printf statement is used. This is
    // better to use then sendChar when large bulk of information needs
    // to be sent.
    void* sendPrintf;

    // Callback function everytime the user wants to force to send the
    // entire buffer.
    void* forceSend;
};

// Initializes the serial buffer to it's default state.  Returns 0 if succesful.
int SerialBufferInit(volatile struct SerialBuffer* buf, int size);

// De-initializes the serial buffer.
void SerialBufferDeInit(volatile struct SerialBuffer* buf);

// Returns 1 if the buffer contains no data.
int SerialBufferIsEmpty(volatile struct SerialBuffer* buf);

// Returns 1 if the buffer is completely full.
int SerialBufferIsFull(volatile struct SerialBuffer* buf);

// Returns the amount of data stored in the buffer.
int SerialBufferSize(volatile struct SerialBuffer* buf);

// Returns the maximum amount of bytes this buffer can store.
int SerialBufferMaxCapacity(volatile struct SerialBuffer* buf);

// Adds a 8bit character onto the buffer.
int SerialBufferPush(volatile struct SerialBuffer* buf, uint8_t value);

// Removes a 8bit character off the buffer and returns it's value.
int16_t SerialBufferPop(volatile struct SerialBuffer* buf);

// Returns the a 8bit character off the buffer, but does not remove it.
// To look at the top-most byte, set `amount` to zero.  To offset a certain
// amounts of bytes from the top of the buffer, specificy that by setting
// `amount`
int SerialBufferPeek(volatile struct SerialBuffer* buf, uint16_t amount);

// Copies the buffer contents to the data array, without removing any of the data.
int SerialBufferCopy(volatile struct SerialBuffer* buf, char* data, uint16_t size);

// Resets the buffer to a default state, all data will be erased.
void SerialBufferReset(volatile struct SerialBuffer* buf);

// Saves a char to the buffer.
int SerialBufferSaveChar(volatile struct SerialBuffer* buf, char c);

// Saves a null-terminated string to the buffer.
int SerialBufferSaveString(volatile struct SerialBuffer* buf, char* c);

// Saves an integer (converted to ASCII) to the buffer.
// Ex. if n = 53, '5' and '3' will be saved to the buffer.
int SerialBufferSaveInt(volatile struct SerialBuffer* buf, int n);

// Saves a double (converted to ASCII) to the buffer.
// Ex. if n = '3.1415', '3', '.', '1', '4', '1', '5', will be
//     saved to the buffer.
int SerialBufferSaveDouble(volatile struct SerialBuffer* buffer, double n);

// Executes a printf statement with a va_list of arguements.
// Ex. if s = "Hi %d\n" and va_list contains the integer 53,
//     'H', 'i', ' ', '5', '3', '\n', will be saved to the buffer.
int SerialBufferPrintfVargs(volatile struct SerialBuffer* buf, char* s, va_list ap);

// Executes a printf statement.  See example above.
int SerialBufferPrintf(volatile struct SerialBuffer* buf, char* s, ...);

// Returns the current forceSend callback.
void* SerialBufferGetForceSendCallback(volatile struct SerialBuffer* buf);

// Sets the forceSend callback.
void SerialBufferSetForceSendCallback(volatile struct SerialBuffer* buf, void* cb);

// Returns the current printf callback.
void* SerialBufferGetPrintfCallback(volatile struct SerialBuffer* buf);

// Sets the printf callback.
void SerialBufferSetPrintfCallback(volatile struct SerialBuffer* buf, void* cb);

// Sets the sendChar callback.
void SerialBufferSetSendCharCallback(volatile struct SerialBuffer* buf, void* cb);

// Returns the sendChar callback.
void* SerialBufferGetSendCharCallback(volatile struct SerialBuffer* buf);

// Tell the process using this serial buffer to force save.
void SerialBufferForceSend(volatile struct SerialBuffer* buf);

#endif
