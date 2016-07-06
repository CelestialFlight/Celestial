#ifndef __DATA_LOGGER_H__
#define __DATA_LOGGER_H__

#include "common/serialBuffer.h"

// A datalogger is any perihperal which can save large amounts of data.
// An example would be an SD Card.  This is merely an extension to the
// serial buffer structure.
struct DataLogger
{
    volatile struct SerialBuffer* buf;
};

// Initializes the datalogger default values.
int DataLoggerInit(volatile struct DataLogger* dl,
    volatile struct SerialBuffer* buf);

// Send a printf to the datalogger.
int DataLoggerPrintf(volatile struct DataLogger* dl,
    char* c,
    ...);

// Prints raw data to the datalogger.  `
int DataLoggerPrintRaw(volatile struct DataLogger* dl,
    void* data,
    uint32_t bytes);

// Tells the datalogger to do a force save.  This is important for
// peripherals where there can be a high overhead cost for setup
// and data will be buffered (only being saved periodically).
// As example, when you reached your last byte of information, it
// would be required to tell this buffer to save it's contents as
// no more data would arrive.
int DataLoggerForceSave(volatile struct DataLogger* dl);

#endif
