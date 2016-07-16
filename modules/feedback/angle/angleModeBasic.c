#include "angleModeBasic.h"

int AngleModeBasicInit(
    struct AngleModeBasic* angle,
    double pitchP, double pitchI, double pitchD,
    double rollP, double rollI, double rollD,
    double yawP, double yawI, double yawD)
{
    PIDBasicInit(&angle->pitchPID, pitchP, pitchI, pitchD);
    PIDBasicInit(&angle->rollPID, rollP, rollI, rollD);
    PIDBasicInit(&angle->yawPID, yawP, yawI, yawD);

    return 0;
}

void AngleModeBasicUpdate(
        struct AngleModeBasic* angle,
        double targetPitchForwardAngle,
        double targetRollRightAngle,
        double targetYawRightAngle,
        double pitchForwardAngle,
        double rollRightAngle,
        double yawRightAngle,
        double dT)
{
    // Calculate errors.
    double pitchError = targetPitchForwardAngle - pitchForwardAngle;
    double rollError = targetRollRightAngle - rollRightAngle;
    double yawError = targetYawRightAngle - yawRightAngle;

    // PID calculations.
    angle->pitchResult = PIDBasicUpdate(&angle->pitchPID, pitchError, dT);
    angle->rollResult = PIDBasicUpdate(&angle->rollPID, rollError, dT);
    angle->yawResult = PIDBasicUpdate(&angle->yawPID, yawError, dT);
}
