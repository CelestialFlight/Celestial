#ifndef __LINEAR_REGRESSION_H__
#define __LINEAR_REGRESSION_H__

struct LinearRegression
{
    // y = m * x + b
    double m, b;
};

int LinearRegressionInit(struct LinearRegression* reg, double m, double b);
double LinearRegressionCalc(struct LinearRegression* reg, double x);

#endif
