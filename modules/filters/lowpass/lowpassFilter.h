#ifndef __LOW_PASS_FILTER_H__
#define __LOW_PASS_FILTER_H__

#define BUFFER_SIZE 1

struct LowPassFilter
{
    double buf[BUFFER_SIZE];
    double _sum;
    int _index;
};

void LowPassFilterInit(struct LowPassFilter* filter);
double LowPassFilterSample(struct LowPassFilter* filter, double sample);
#endif
