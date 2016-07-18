#ifndef __ERROR_H__
#define __ERROR_H__

#include "serialBuffer.h"

// Sets the SerialBuffer output.
void SetErrorOutput(struct SerialBuffer* output);

// Returns 1 if an error occurs, 0 if no warning.
int _error(int x, const char* file, int line);

// Returns 1 if a warning was thrown.  0 if no warning.
int _warning(int x, const char* file, int line);

// Prints a message to the error log.
void _verbose(const char* x, const char* file, int line);

// Macro for error asserts.
#define error(x) \
    _error(x, __FILE__, __LINE__)

// Macro for warning asserts.
#define warning(x) \
    _warning(x, __FILE__, __LINE__);

// Macro for verbose messages on the error log.
#define verbose(str) \
    _verbose(str, __FILE__, __LINE__)

#endif
