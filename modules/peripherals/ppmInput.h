#ifndef __PPM_INPUT_H__
#define __PPM_INPUT_H__

#include <stdint.h>

#define MAX_CHANNELS 8
#define MIN_PPM_SYNC_PULSE 2700

struct PPMInput
{
    uint16_t channels[MAX_CHANNELS];
    uint16_t updateCount;

    uint32_t _edgeTimes[MAX_CHANNELS*2];
    uint64_t _lastEdgeTime;
    uint16_t _index;

    // Signed so that numbers can go negative on accident
    // without overflowing.
    int8_t _dataCorrupt;

    // Keeps track how many signals are being dropped (hopefully 0!)
    uint32_t _corruptDataCounter;
};

#endif
