#include "complementaryFilter.h"

void ComplementaryFilterInit(
	struct ComplementaryFilter* cf, double alpha)
{
	cf->angle = 0;
	cf->alpha = alpha;
}

// X = pitch, Y = roll, Z = yaw
void ComplementaryFilterUpdate(
    struct ComplementaryFilter* cf,
    double newAngle,
    double rate,
    double dT)
{
	cf->angle += rate * dT;
	cf->angle = cf->angle * cf->alpha + newAngle * (1 - cf->alpha);

	//cf->roll += gyroData->y * dT;

	/*double ax = accelData->x;
	double ay = accelData->y;
	double az = accelData->z;

	double pitchAccel =
		a0tan2(ay, sqrt(ax*ax + az*az)) * 57.2958;

	double rollAccel =
		atan2(ax, sqrt(ay*ay + az*az)) * 57.2958;

	//printf("IMU %f %f ROLL %f %f %f %f %f\n", cf->roll, cf->pitch, ax, ay, az, rollAccel, pitchAccel);
*/
	// Combine gyroscope and accelerometer data.
	//cf->pitch = cf->pitch * cf->alpha + pitchAccel * (1 - cf->alpha);
	//cf->roll = cf->roll * cf->alpha + rollAccel * (1 - cf->alpha);
}
