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
