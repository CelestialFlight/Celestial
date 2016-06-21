#ifndef __VECTOR_3D_H__
#define __VECTOR_3D_H__

#define VECTOR_X    0
#define VECTOR_Y    1
#define VECTOR_Z    2

// Represents a three dimensional object.
struct Vector3D
{
	double x, y, z;
};

struct Vector4D
{
	double w, x, y, z;
};

double Vector3DGetValue(struct Vector3D* v3d, int axis);

#endif
