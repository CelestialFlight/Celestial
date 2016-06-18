#ifndef __ULTRA_TRACKERS_H__
#define __ULTRA_TRACKERS_H__

#include <stdint.h>
#include <math.h>

#define _UT_MODE_WAITING 0x00
#define _UT_MODE_CAPTURING 0x01
#define _UT_MODE_PRINT 0x02
#define _UT_MODE_COMPUTE 0x03

// Save for 2000us
#define _UT_BUFFER_SIZE 2000

#include "serialBuffer.h"

struct UltraTrackers
{
    uint8_t buffer1[_UT_BUFFER_SIZE];
    uint8_t buffer2[_UT_BUFFER_SIZE];
    uint8_t buffer3[_UT_BUFFER_SIZE];
    uint8_t buffer4[_UT_BUFFER_SIZE];
    int index;
    int mode;

    // Distance between mic 1 and mic 2/3 in mm.
    double range12;
    double range13;

    // This is the output stream for printing data.
    struct SerialBuffer* output;
};

void UltraTrackersInit(struct UltraTrackers* ut,
    double range12, double range13, double range14);

void UltraTrackersSetOutput(struct UltraTrackers* ut,
    struct SerialBuffer* output);

int UltraTrackersUpdate(struct UltraTrackers* ut,
    int value1, int value2, int value3, int value4);

#endif
