#include "ultraTrackers.h"

void UltraTrackersInit(struct UltraTrackers* ut,
    double range12, double range13, double range14)
{
    ut->index = 0;
    ut->mode = _UT_MODE_WAITING;
    ut->output = 0;
    ut->range12 = range12;
    ut->range13 = range13;
}

void UltraTrackersSetOutput(struct UltraTrackers* ut,
    struct SerialBuffer* output)
{
    ut->output = output;
}

// Helper functions.
static int _findStartOfSignal(uint8_t* buffer, int bufSize);
static int _getPulseWidth(uint8_t* buffer, int pulseStart, int bufSize);

// This should be updated at exactly 1us (or whichever sampling
// rate is desired)
// value1 - 4 are the current sensors values for each tracker.
// High = 1, Low = 0.  This will keep checking for pulses until
// one goes high (1) before capturing.
int UltraTrackersUpdate(struct UltraTrackers* ut,
    int value1, int value2, int value3, int value4)
{
    switch (ut->mode)
    {
        // Wait for high pulse before capturing data.
        case _UT_MODE_WAITING:

            // Check for high pulse in any of the channels
            if (value1 == 1 || value2 == 1 || value3 == 1 || value4 == 1)
            {
                ut->mode = _UT_MODE_CAPTURING;
                ut->index = 0;

                // Intentionally don't break here so to continue to capturing
                // section.
            }
            else
            {
                break;
            }

        // Capture incoming data.
        case _UT_MODE_CAPTURING:

            ut->buffer1[ut->index] = value1;
            ut->buffer2[ut->index] = value2;
            ut->buffer3[ut->index] = value3;
            ut->buffer4[ut->index] = value4;
            ut->index++;

            if (ut->index >= _UT_BUFFER_SIZE)
            {
                ut->mode = _UT_MODE_PRINT;

                // Intentionally don't break here so to continue to printing
                // section.
            }
            else
            {
                break;
            }
            break;


        // Print captured data to stream if configured.
        case _UT_MODE_PRINT:

            // If there is a datastream to save data to, print the captured data.
            if (ut->output != 0 && 0)
            {
                int i = 0;
                for (i = 0; i < _UT_BUFFER_SIZE; i++)
                {
                    // Print data to serial buffer.
                    SerialBufferPrintf(ut->output, "%d, %d, %d, %d, %d\n",
                        i,
                        ut->buffer1[i],
                        ut->buffer2[i],
                        ut->buffer3[i],
                        ut->buffer4[i]);

                    while (SerialBufferIsEmpty(ut->output) == 0);
                }
            }

            // Intentionally don't break and go onto the next state.
            ut->mode = _UT_MODE_COMPUTE;

        // Compute angles and
        case _UT_MODE_COMPUTE:
        {
            int time1 = _findStartOfSignal(ut->buffer1, _UT_BUFFER_SIZE);
            int time2 = _findStartOfSignal(ut->buffer2, _UT_BUFFER_SIZE);
            int time3 = _findStartOfSignal(ut->buffer3, _UT_BUFFER_SIZE);
            int pWidth1 = _getPulseWidth(ut->buffer1, time1, _UT_BUFFER_SIZE);
            int pWidth2 = _getPulseWidth(ut->buffer2, time2, _UT_BUFFER_SIZE);
            int pWidth3 = _getPulseWidth(ut->buffer3, time3, _UT_BUFFER_SIZE);
            //_filterResults(&time1, &time2, &time3, pWidth1, pWidth2, pWidth3);


            // If there was missing data, we can't determine
            // the position at this point.
            if (time1 == -1 || time2 == -1 || time3 == -1)
            {
                ut->mode = _UT_MODE_WAITING;
                break;
            }

            // Adjacent sensors
            int diff12 = time1 - time2;
            int diff13 = time3 - time1;

            // Diagonal sensors
            int diff23 = time3 - time2;

            double range23 =
                sqrt(ut->range12 * ut->range12 + ut->range13 * ut->range13);

            // 0.34029mm / usec * timeDiff usec / distance bewteen sensors in mm.
            double yaw12 = asin(0.34029 * diff12 / ut->range12);
            double yaw13 = asin(0.34029 * diff13 / ut->range13);
            double yaw23 = asin(0.34029 * diff23 / range23);

            double pt12x1, pt12y1;
            pt12x1 = 0;
            pt12y1 = ut->range12/2;
            double pt12x2, pt12y2;
            pt12x2 = sin(3.1415/2 - yaw12);
            pt12y2 = cos(3.1415/2 - yaw12) + ut->range12/2;

            double pt13x1, pt13y1;
            pt13x1 = ut->range13/2;
            pt13y1 = 0;
            double pt13x2, pt13y2;
            pt13x2 = sin(3.1415 - yaw13) + ut->range13/2;
            pt13y2 = cos(3.1415 - yaw13);

            double pt23x1, pt23y1;
            pt23x1 = ut->range13/2;
            pt23y1 = ut->range12/2;

            double pt23x2, pt23y2;
            pt23x2 = sin(3.1415*3.0/4.0 - yaw23) + ut->range13/2;
            pt23y2 = cos(3.1415*3.0/4.0 - yaw23) + ut->range12/2;

            double x1 = pt12x1;
            double x2 = pt12x2;
            double x3 = pt23x1;
            double x4 = pt23x2;
            double y1 = pt12y1;
            double y2 = pt12y2;
            double y3 = pt23y1;
            double y4 = pt23y2;

            double xtop = (x1*y2 - y1*x2) * (x3 - x4) - (x1 - x2) * (x3*y4 - y3*x4);
            double xbottom = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

            double ytop = (x1*y2 - y1*x2) * (y3 - y4) - (y1 - y2) * (x3*y4 - y3*x4);
            double ybottom = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

            double newX = xtop / xbottom;
            double newY = ytop / ybottom;

            /*SerialBufferPrintf(ut->output,
                "%d %d %d %d %d %d %f %f\n",
                time1, time2, time3, pWidth1, pWidth2, pWidth3, newX, newY);*/

            SerialBufferPrintf(ut->output,
                "%f %f\n",
                newX, newY);

            ut->mode = _UT_MODE_WAITING;
            return 1;
        }
    }

    return 0;
}

// Returns the index of the first non-zero.
static int _findStartOfSignal(uint8_t* buffer, int bufSize)
{
    int i;
    for (i = 0; i < bufSize; i++)
    {
        if (buffer[i] != 0) return i;
    }

    return -1;
}

static int _getPulseWidth(uint8_t* buffer, int pulseStart, int bufSize)
{
    int count = 0;
    int i;
    for (i = pulseStart; i < bufSize; i++)
    {
        if (buffer[i] == 0) return count;

        count++;
    }

    return count;
}
