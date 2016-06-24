#ifndef __DATA_LOGGER_H__
#define __DATA_LOGGER_H__

#include "serialBuffer.h"

struct DataLogger
{
    volatile struct SerialBuffer* buf;
};

int DataLoggerInit(volatile struct DataLogger* dl, volatile struct SerialBuffer* buf);
int DataLoggerPrintf(volatile struct DataLogger* dl, char* c, ...);
int DataLoggerPrintRaw(volatile struct DataLogger* dl, void* data, uint32_t bytes);
int DataLoggerForceSave(volatile struct DataLogger* dl);

#endif
