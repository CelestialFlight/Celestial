#include "PIDBasic.h"
#include "common/error.h"

// Initializes the controller to default values.
int PIDBasicInit(struct PIDBasic* pid, double kP, double kI, double kD)
{
    // Defensive check.
    if (error(pid != 0)) return -1;

    pid->kP = kP;
    pid->kI = kI;
    pid->kD = kD;

    pid->previousError = 0;
    pid->integral = 0;
    pid->derivative = 0;
    pid->error = 0;

    // If max = 0, do no limit to any value.
    pid->maxIntegral = 0;
    pid->maxOutput = 0;

    return 0;
}

// Calcultes the feedback value for the PID controller.
double PIDBasicUpdate(struct PIDBasic* pid, double error, double dT)
{
    // Defensive check.
    if (error(pid != 0)) return 0;

    pid->error = error;
    pid->integral += error * dT;
    pid->derivative = (error - pid->previousError) / dT;
    pid->previousError = error;

    // Ensure the integral doesn't get too large.
    if (pid->maxIntegral != 0)
    {
        if (pid->integral > pid->maxIntegral)
            pid->integral = pid->maxIntegral;

        if (pid->integral < -pid->maxIntegral)
            pid->integral = -pid->maxIntegral;
    }

    double returnValue = pid->error * pid->kP
        + pid->integral * pid->kI
        + pid->derivative * pid->kD;

    if (pid->maxOutput != 0)
    {
        if (returnValue > pid->maxOutput)
            returnValue = pid->maxOutput;

        if (returnValue < -pid->maxOutput)
            returnValue = -pid->maxOutput;
    }

    return returnValue;
}

int PIDBasicSetConstants(struct PIDBasic* pid,
    double kP, double kI, double kD)
{
    // Defensive check.
    if (error(pid != 0)) return 0;

    pid->kP = kP;
    pid->kI = kI;
    pid->kD = kD;

    return 0;
}

int PIDBasicSetMaxIntegral(struct PIDBasic* pid, double maxIntegral)
{
    // Defensive check.
    if (error(pid != 0)) return 0;

    pid->maxIntegral = maxIntegral;

    return 0;
}

int PIDBasicResetMaxIntegral(struct PIDBasic* pid)
{
    // Defensive check.
    if (error(pid != 0)) return 0;

    pid->maxIntegral = 0;

    return 0;
}

int PIDBasicSetMaxOutput(struct PIDBasic* pid, double maxOutput)
{
    // Defensive check.
    if (error(pid != 0)) return 0;

    pid->maxOutput = maxOutput;

    return 0;
}

int PIDBasicResetMaxOutput(struct PIDBasic* pid)
{
    // Defensive check.
    if (error(pid != 0)) return 0;

    pid->maxOutput = 0;

    return 0;
}
