#ifndef __ERROR_H__
#define __ERROR_H__

#include "serialBuffer.h"

void SetErrorOutput(struct SerialBuffer* output);
struct SerialBuffer* GetErrorOutput(void);
int _error(int x, const char* file, const char* function, int line);

#define error(x) \
    _error(x, __FILE__, __FUNCTION__, __LINE__)

#define warning(x) \
    if (x == 0 && GetErrorOutput() != 0){ \
        SerialBufferPrintf( \
            GetErrorOutput(), "WARNING in %s func:%s line:%d", \
            __FILE__, __FUNCTION__, __LINE__); }

#endif
