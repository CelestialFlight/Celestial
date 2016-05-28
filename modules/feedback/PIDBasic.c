#include "PIDBasic.h"

// Initializes the PID
int PIDBasicInit(struct PIDBasic* pid, double kP, double kI, double kD)
{
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
