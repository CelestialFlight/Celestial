#ifndef __MIXER_H__
#define __MIXER_H__

#include <stdint.h>

// Predefined array indicies for QuadX props.
#define MIXER_PROP_BR   0
#define MIXER_PROP_FR   1
#define MIXER_PROP_BL   2
#define MIXER_PROP_FL   3

// This records how much of each command should be applied to each motor.
// Values should range from -1 to 1
struct MotorMixer
{
    double throttle;
    double pitch;
    double roll;
    double yaw;
    double result;
};

struct Mixer
{
    // Number of motors for this UAV
    uint8_t numMotors;

    // Pointer to an array of all the motor mixers.
    struct MotorMixer* motors;
};

// `mix` is an array of MotorMixer structs. The array size should be the same
//   as the number of motors on the drone.
// `numOfMotors` is the size of the array `mix`.
int MixMotorValues(struct MotorMixer* mix, int numOfMotors,
    double throtValue, double rollValue, double pitchValue, double yawValue);

#endif
