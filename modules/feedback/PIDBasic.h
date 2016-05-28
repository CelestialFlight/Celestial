#ifndef __PID_BASIC_H__
#define __PID_BASIC_H__

struct PIDBasic
{
    double kP, kI, kD;

    double error, integral, derivative, previousError;

    double maxIntegral, maxOutput;
};

int PIDBasicInit(struct PIDBasic* pid, double kP, double kI, double kD);
double PIDBasicUpdate(struct PIDBasic* pid, double error, double dT);

#endif
