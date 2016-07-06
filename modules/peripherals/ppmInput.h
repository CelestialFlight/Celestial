#ifndef __PPM_INPUT_H__
#define __PPM_INPUT_H__

#include <stdint.h>

#define MAX_CHANNELS 8
#define MIN_PPM_SYNC_PULSE 2700

// PPM is a protocol mainly used for controllers.  It is a seris of
// pulses that represent the values of several channels.
struct PPMInput
{
    // Current channel value.
    uint16_t channels[MAX_CHANNELS];

    // This is subtracted from the input signal value so that if
    // the "zero" value is offset from exactly 1500, it can be
    // calibrated easily.  By default, all zero.
    uint16_t channelsBias[MAX_CHANNELS];
    uint16_t updateCount;

    uint32_t _edgeTimes[MAX_CHANNELS*2];
    uint64_t _lastEdgeTime;
    uint16_t _index;

    // Signed so that numbers can go negative on accident
    // without overflowing.
    int8_t _dataCorrupt;

    // Keeps track how many signals are being dropped (hopefully 0!)
    uint32_t _corruptDataCounter;

    // Pointer to a datastructure that contains information concerning
    // the pin being used.  This will be different for every processor.
    void* pinData;
};

// Initializes the PPM hardware and structure.
int PPMInputInit(struct PPMInput* ppm, void* pinData);

// Sets the PPWMInput Bias value.  This value will be subtracted from
// the value calculated.
// Ex. If the bias is set to 1000, and the PPM value is 1500, the
// resuting value is 1500 - 1000 = 500
void PPMInputSetBias(struct PPMInput* ppm, int channel, uint16_t bias);

// Everytime a pulse is detected, update the structure.
int PPMInputUpdate(struct PPMInput* ppm);

// Get a channel value.
int32_t PPMInputGetValue(struct PPMInput* ppm, int channel);


#endif
