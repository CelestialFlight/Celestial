#ifndef __PID_BASIC_H__
#define __PID_BASIC_H__

// A PID controller is the fundemnetal feedback controller used on most
// quadcopter systems.  In simply takes in an sensor input and computes
// a value back that will be used to control some other device.
// For example, sensors can tell a wheel is moving too slow, so
// the PID controller will give a value back telling a motor to speed up.
struct PIDBasic
{
    // Proportional, Integral, Derivative constants.
    double kP, kI, kD;

    //
    double error, integral, derivative, previousError;

    // Max integral is how large the `integral` value can become. If left
    // as 0, the integral will not be restricted.
    double maxIntegral;

    // Max output is the maximum output value the controller can return.
    // If left as 0, the output will be restricted.
    double maxOutput;
};

// Initalizes the PID controller and constants to default states.
// Returns 0 if succesful. -1 if NULL pointer.
int PIDBasicInit(struct PIDBasic* pid, double kP, double kI, double kD);

// Computes the feedback value for the PID.  dT should be the
// amount of time since the last update in seconds.
double PIDBasicUpdate(struct PIDBasic* pid, double error, double dT);

// Changes the kP, kI, and kD constants.
// Returns 0 if succesful, -1 if null pointer.
int PIDBasicSetConstants(struct PIDBasic* pid,
    double kP, double kI, double kD);

// Sets the maximum integral value to prevent windup.
// Returns 0 if succesful, -1 if null pointer.
int PIDBasicSetMaxIntegral(struct PIDBasic* pid, double maxIntegral);

// Resets the maximum integral value to none.
// Returns 0 if succesful, -1 if null pointer.
int PIDBasicResetMaxIntegral(struct PIDBasic* pid);

// Sets the maximum output value.
// Returns 0 if succesful, -1 if null pointer.
int PIDBasicSetMaxOutput(struct PIDBasic* pid, double maxOutput);

// Resets the maximum output value.
// Returns 0 if succesful, -1 if null pointer.
int PIDBasicResetMaxOutput(struct PIDBasic* pid);

#endif
