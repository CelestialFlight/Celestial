#include "rateModeBasic.h"

int RateModeBasicInit(
    struct RateModeBasic* rate,
    double pitchP, double pitchI, double pitchD,
    double rollP, double rollI, double rollD,
    double yawP, double yawI, double yawD)
{
    PIDBasicInit(&rate->pitchPID, pitchP, pitchI, pitchD);
    PIDBasicInit(&rate->rollPID, rollP, rollI, rollD);
    PIDBasicInit(&rate->yawPID, yawP, yawI, yawD);

    return 0;
}

struct RateModeResults RateModeBasicUpdate(
    struct RateModeBasic* rate,
    double targetPitchForwardRate,
    double targetRollRightRate,
    double targetYawRightRate,
    double pitchForwardRate,
    double rollRightRate,
    double yawRightRate,
    double dT)
{
    // Calculate errors.
    double pitchError = targetPitchForwardRate - pitchForwardRate;
    double rollError = targetRollRightRate - rollRightRate;
    double yawError = targetYawRightRate - yawRightRate;

    // PID calculations.
    struct RateModeResults x;
    x.pitchResult = PIDBasicUpdate(&rate->pitchPID, pitchError, dT);
    x.rollResult = PIDBasicUpdate(&rate->rollPID, rollError, dT);
    x.yawResult = PIDBasicUpdate(&rate->yawPID, yawError, dT);

    return x;
}
