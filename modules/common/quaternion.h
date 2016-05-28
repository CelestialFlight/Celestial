#ifndef __QUATERNION_H__
#define __QUATERNION_H__

// Math.h for cos and sin functions.
#include <math.h>
#include "vector3d.h"

struct Quaternion
{
	double x, y, z, w;
};

void QuaternionFromEuler(
	struct Quaternion* out, double pitch, double roll, double yaw);

void QuaternionMultiplyByQuaternion(
	struct Quaternion* A, struct Quaternion* B, struct Quaternion* out);

void QuaternionMultiplyByVector(
	struct Quaternion* A, struct Vector3D* B, struct Vector3D* out);

#endif
