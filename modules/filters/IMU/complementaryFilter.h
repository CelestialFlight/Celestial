#ifndef __COMPLEMENTARY_FILTER_H__
#define __COMPLEMENTARY_FILTER_H__

#include "common/vector3d.h"
#include <math.h>

struct ComplementaryFilter
{
	// Filter outputs in degrees.
	double roll, pitch;

	// Constant that values the gyroscope over the accelerometer.
	double alpha;
};

void ComplementaryFilterInit(
	struct ComplementaryFilter* cf, double alpha);

void ComplementaryFilterUpdate(
	struct ComplementaryFilter* cf,
	struct Vector3D* accelData,
	struct Vector3D* gyroData,
	double dT);

#endif
