#ifndef __ANGLE_MODE_BASIC_H__
#define __ANGLE_MODE_BASIC_H__

#include "feedback/PIDBasic.h"

struct AngleModeBasic
{
    struct PIDBasic pitchPID;
    struct PIDBasic rollPID;
    struct PIDBasic yawPID;
};

struct AngleModeResults
{
    double pitchResult, rollResult, yawResult;
};

int AngleModeBasicInit(
    struct AngleModeBasic* angle,
    double pitchP, double pitchI, double pitchD,
    double rollP, double rollI, double rollD,
    double yawP, double yawI, double yawD);

struct AngleModeResults AngleModeBasicUpdate(
        struct AngleModeBasic* angle,
        double targetPitchForwardAngle,
        double targetRollRightAngle,
        double targetYawRightAngle,
        double pitchForwardAngle,
        double rollRightAngle,
        double yawRightAngle,
        double dT);

#endif
