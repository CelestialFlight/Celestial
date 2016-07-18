// Mixer maps an array of motors to 4 commands (throt, pitch, roll, yaw).
#include "mixer.h"


int MixMotorValues(struct MotorMixer* mix, int numOfMotors,
    double throtValue, double pitchValue, double rollValue, double yawValue)
{
    int i;
    for (i = 0; i < numOfMotors; i++)
    {
        double result = mix[i].throttle*throtValue
            + mix[i].pitch*pitchValue
            + mix[i].roll*rollValue
            + mix[i].yaw*yawValue;

        if (result > 1000)
            result = 1000;
        else if (result < 0)
            result = 0;

        mix[i].result = result;
    }

    return 0;
}
