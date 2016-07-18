#ifndef __COMPLEMENTARY_FILTER_H__
#define __COMPLEMENTARY_FILTER_H__

#include "common/vector.h"
#include <math.h>

struct ComplementaryFilter
{
    // Filter outputs in degrees.
    double angle;

    // Constant that values the gyroscope over the accelerometer.
    double alpha;
};

int ComplementaryFilterInit(
    struct ComplementaryFilter* cf, double alpha);

int ComplementaryFilterUpdate(
    struct ComplementaryFilter* cf,
    double angle,
    double rate,
    double dT);

#endif
