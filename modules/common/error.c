#include "error.h"

struct SerialBuffer* errorOutput;

void SetErrorOutput(struct SerialBuffer* output)
{
    errorOutput = output;
}

struct SerialBuffer* GetErrorOutput(void)
{
    return errorOutput;
}

// Returns 1 if there was an error.
int _error(int x, const char* file, const char* function, int line)
{
    if (x == 0)
    {
        if (GetErrorOutput() != 0)
        {
            SerialBufferPrintf(
                GetErrorOutput(), "ERROR in %s func:%s line:%d", file, function, line);
        }

        return 1;
    }

    return 0;
}
