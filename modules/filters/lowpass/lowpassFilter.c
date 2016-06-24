#include "lowpassFilter.h"

void LowPassFilterInit(struct LowPassFilter* filter)
{
    int i;
    for (i = 0; i < BUFFER_SIZE; i++)
        filter->buf[i] = 0;

    filter->_sum = 0;
    filter->_index = 0;
}

double LowPassFilterSample(struct LowPassFilter* filter, double sample)
{
    filter->_sum = filter->_sum - filter->buf[filter->_index];
    filter->buf[filter->_index] = sample;
    filter->_sum += sample;
    filter->_index++;
    if (filter->_index >= BUFFER_SIZE) filter->_index = 0;

    return filter->_sum / BUFFER_SIZE;
}
