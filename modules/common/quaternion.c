#include "quaternion.h"

void QuaternionFromEuler(
	struct Quaternion* out, double pitch, double roll, double yaw)
{
	// Convert Euler to Quaternion
	double c1 = cos(roll / 2);// Roll
	double s1 = sin(roll / 2);
	double c2 = cos(pitch / 2);//Pitch
	double s2 = sin(pitch / 2);
	double c3 = cos(yaw / 2);// Yaw
	double s3 = sin(yaw / 2);

	// Rotate the previous quaternion with the new rotation
	// applied by the torque on each motor.
	out->x = s1 * c2 * c3 + c1 * s2 * s3;
	out->y = c1 * s2 * c3 - s1 * c2 * s3;
	out->z = c1 * c2 * s3 + s1 * s2 * c3;
	out->w = c1 * c2 * c3 - s1 * s2 * s3;	
}

void QuaternionMultiplyByQuaternion(
	struct Quaternion* A, struct Quaternion* B, struct Quaternion* out)
{
	double x1 = A->x;	
	double y1 = A->y;
	double z1 = A->z;
	double w1 = A->w;		

	double x2 = B->x;	
	double y2 = B->y;
	double z2 = B->z;
	double w2 = B->w;		

	out->w = (w1*w2 - x1*x2 - y1*y2 - z1*z2);
 	out->x = (w1*x2 + x1*w2 + y1*z2 - z1*y2);
	out->y = (w1*y2 - x1*z2 + y1*w2 + z1*x2);
	out->z = (w1*z2 + x1*y2 - y1*x2 + z1*w2);
}

void QuaternionMultiplyByVector(
	struct Quaternion* A, struct Vector3D* B, struct Vector3D* out)
{
	double wx = A->x;
	double wy = A->y;
	double wz = A->z;
	double ww = A->w;

	double vx = B->x;
	double vy = B->y;
	double vz = B->z;

	// Multiply rotational matrix by vector
	out->x = (1 - 2*wy*wy - 2*wz*wz)*vx + (2*wx*wy - 2*wz*ww)*vy     + (2*wx*wz + 2*wy*ww)*vz;
	out->y = (2*wx*wy + 2*wz*ww)*vx     + (1 - 2*wx*wx - 2*wz*wz)*vy + (2*wy*wz - 2*wx*ww)*vz;
	out->z = (2*wx*wz - 2*wy*ww)*vx     + (2*wy*wz + 2*wx*ww)*vy     + (1 - 2*wx*wx - 2*wy*wy)*vz;
}