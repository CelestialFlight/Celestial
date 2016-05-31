#include "complementaryFilter.h"

void ComplementaryFilterInit(
	struct ComplementaryFilter* cf, double alpha)
{
	cf->roll = 0;
	cf->pitch = 0;
	cf->alpha = alpha;
}

// X = pitch, Y = roll, Z = yaw
void ComplementaryFilterUpdate(
	struct ComplementaryFilter* cf,
	struct Vector3D* accelData,
	struct Vector3D* gyroData,
	double dT)
{
	cf->pitch += gyroData->x * dT;
	cf->roll += gyroData->y * dT;

	double ax = accelData->x;
	double ay = accelData->y;
	double az = accelData->z;

	double pitchAccel =
		atan2(ay, sqrt(ax*ax + az*az));

	double rollAccel =
		atan2(ax, sqrt(ay*ay + az*az));

	//printf("IMU %f %f ROLL %f %f %f %f %f\n", cf->roll, cf->pitch, ax, ay, az, rollAccel, pitchAccel);

	// Combine gyroscope and accelerometer data.
	cf->pitch = cf->pitch * cf->alpha + pitchAccel * (1 - cf->alpha);
	cf->roll = cf->roll * cf->alpha + rollAccel * (1 - cf->alpha);
}
