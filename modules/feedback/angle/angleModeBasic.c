// A set of feedback controllers that control the attitude of the quadcopter.
#include "angleModeBasic.h"
#include "common/error.h"

int AngleModeBasicInit(
    struct AngleModeBasic* angle,
    double pitchP, double pitchI, double pitchD,
    double rollP, double rollI, double rollD,
    double yawP, double yawI, double yawD)
{
    // Defensive check.
    if (error(angle != 0)) return -1;

    // Initialize PID controllers to default values.
    int result = 0;
    result |= PIDBasicInit(&angle->pitchPID, pitchP, pitchI, pitchD);
    result |= PIDBasicInit(&angle->rollPID, rollP, rollI, rollD);
    result |= PIDBasicInit(&angle->yawPID, yawP, yawI, yawD);

    // Report any errors that may of occured.
    return error(result == 0);
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
    // Defensive check.
    if (error(angle != 0)) return;

    // Calculate errors.
    double pitchError = targetPitchForwardAngle - pitchForwardAngle;
    double rollError = targetRollRightAngle - rollRightAngle;
    double yawError = targetYawRightAngle - yawRightAngle;

    // PID calculations.
    angle->pitchResult = PIDBasicUpdate(&angle->pitchPID, pitchError, dT);
    angle->rollResult = PIDBasicUpdate(&angle->rollPID, rollError, dT);
    angle->yawResult = PIDBasicUpdate(&angle->yawPID, yawError, dT);
}
