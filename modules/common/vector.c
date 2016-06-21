#include "vector.h"

double Vector3DGetValue(struct Vector3D* v3d, int axis)
{
    if (axis == VECTOR_X) return v3d->x;
    else if (axis == VECTOR_Y) return v3d->y;
    else if (axis == VECTOR_Z) return v3d->z;

    return 0;
}
