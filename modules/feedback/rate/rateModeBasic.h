#ifndef __RATE_MODE_BASIC_H__
#define __RATE_MODE_BASIC_H__

#include "feedback/PIDBasic.h"

struct RateModeBasic
{
    struct PIDBasic pitchPID;
    struct PIDBasic rollPID;
    struct PIDBasic yawPID;
};

struct RateModeResults
{
    double pitchResult, rollResult, yawResult;
};

int RateModeBasicInit(
    struct RateModeBasic* rate,
    double pitchP, double pitchI, double pitchD,
    double rollP, double rollI, double rollD,
    double yawP, double yawI, double yawD);

struct RateModeResults RateModeBasicUpdate(
        struct RateModeBasic* rate,
        double targetPitchForwardRate,
        double targetRollRightRate,
        double targetYawRightRate,
        double pitchForwardRate,
        double rollRightRate,
        double yawRightRate,
        double dT);
#endif
