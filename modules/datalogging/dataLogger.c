#include "dataLogger.h"

int DataLoggerInit(volatile struct DataLogger* dl, volatile struct SerialBuffer* buf)
{
    dl->buf = buf;

    return 0;
}

int DataLoggerPrintf(volatile struct DataLogger* dl, char* c, ...)
{
    if (dl == 0) return 1;
    else if (dl->buf == 0) return 2;

    va_list ap;
    va_start(ap, c);
    SerialBufferPrintfVargs(dl->buf, c, ap);
    va_end(ap);

    return 0;
}

int DataLoggerPrintRaw(volatile struct DataLogger* dl, void* data, uint32_t bytes)
{
    if (dl == 0) return 1;
    else if (dl->buf == 0) return 2;

    char* array = data;

    int i;
    for (i = 0; i < bytes; i++)
    {
        SerialBufferSaveChar(dl->buf, array[i]);
    }

    return 0;
}

int DataLoggerForceSave(volatile struct DataLogger* dl)
{
    if (dl == 0) return 1;
    else if (dl->buf == 0) return 2;

    void* cb = SerialBufferGetForceSendCallback(dl->buf);

    if (cb != 0)
    {
        void (*callback)(volatile struct SerialBuffer*) = dl->buf->forceSend;
        callback(dl->buf);
    }

    return 0;
}
