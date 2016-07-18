// Assert statements used throughout Celestial.
#include "error.h"

// Singleton serialBuffer for saving errors to a output stream.
struct SerialBuffer* errorOutput = 0;

// Sets the serialbuffer stream that should be used for all errors messages.
void SetErrorOutput(struct SerialBuffer* output)
{
    errorOutput = output;
}

// Returns the current serialBuffer being used for the serial log.
static struct SerialBuffer* GetErrorOutput(void)
{
    return errorOutput;
}

// Prints an error to an error log.
int _error(int x, const char* file, int line)
{
    if (x == 0)
    {
        // Check that an error log has been set.
        if (GetErrorOutput() != 0)
        {
            SerialBufferPrintf(
                GetErrorOutput(), "ERROR in %s line:%d", file, line);
        }

        printf("ERROR in %s line:%d\n", file, line);

        return 1;
    }

    return 0;
}

// Prints a warning to an error log.
int _warning(int x, const char* file, int line)
{
    if (x == 0)
    {
        // Check that an error log has been set.
        if (GetErrorOutput() != 0)
        {
            SerialBufferPrintf(
                GetErrorOutput(), "WARNING in %s line:%d", file, line);
        }

        printf("WARNING in %s line:%d\n", file, line);

        return 1;
    }

    return 0;
}

// Prints a verbose statement to an error log.
void _verbose(const char* x, const char* file, int line)
{
    if (x != 0)
    {
        // Check that an error log has been set.
        if (GetErrorOutput() != 0)
        {
            SerialBufferPrintf(
                GetErrorOutput(), "%s (in %s line:%d)\n", x, file, line);
        }

        printf("%s (in %s line:%d)\n", x, file, line);
    }
}
