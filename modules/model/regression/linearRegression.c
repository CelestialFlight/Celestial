#include "linearRegression.h"

int LinearRegressionInit(struct LinearRegression* reg, double m, double b)
{
    if (reg == 0) return 1;

    reg->m = m;
    reg->b = b;

    return 0;
}

double LinearRegressionCalc(struct LinearRegression* reg, double x)
{
    return reg->m * x + reg->b;
}
