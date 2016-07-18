#ifndef __ANGLE_MODE_BASIC_H__
#define __ANGLE_MODE_BASIC_H__

#include "feedback/PIDBasic.h"

struct AngleModeBasic
{
    struct PIDBasic pitchPID;
    struct PIDBasic rollPID;
    struct PIDBasic yawPID;
    double pitchResult, rollResult, yawResult;
};

// Initializes PID controllers.  Returns 0 if succesful.  -1 for NULL pointer.
int AngleModeBasicInit(
    struct AngleModeBasic* angle,
    double pitchP, double pitchI, double pitchD,
    double rollP, double rollI, double rollD,
    double yawP, double yawI, double yawD);

// Given pitch/roll/yaw information, updates the feedback controllers.
// Results are stored in angle->pitchResult, angle->rollResult, angle->yawResult
void AngleModeBasicUpdate(
        struct AngleModeBasic* angle,
        double targetPitchForwardAngle,
        double targetRollRightAngle,
        double targetYawRightAngle,
        double pitchForwardAngle,
        double rollRightAngle,
        double yawRightAngle,
        double dT);

#endif
