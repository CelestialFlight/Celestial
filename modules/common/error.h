#ifndef __ERROR_H__
#define __ERROR_H__

#include "serialBuffer.h"

void SetErrorOutput(struct SerialBuffer* output);
struct SerialBuffer* GetErrorOutput(void);

#define error(x) \
    if (x == 0 && GetErrorOutput() != 0) \
        SerialBufferPrintf( \
            GetErrorOutput(), "ERROR in %s func:%s line:%d", \
            __FILE__, __FUNCTION__, __LINE__);

#define warning(x) \
    if (x == 0 && GetErrorOutput() != 0) \
        SerialBufferPrintf( \
            GetErrorOutput(), "WARNING in %s func:%s line:%d", \
            __FILE__, __FUNCTION__, __LINE__);

#endif
