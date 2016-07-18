#include "singlePoleLPF.h"
#include <math.h>
#include "common/error.h"

int SinglePoleLPFInit(
    struct SinglePoleLPF* filter,
    double sampleFreq, double cutoffFreq)
{
    if (error(filter != 0)) return -1;

    double x = exp(-2.0 * 3.1415 * cutoffFreq/sampleFreq);

    filter->a0 = 1 - x;
    filter->b1 = x;
    filter->old_output = 0;

    return 0;
}

double SinglePoleLPFSample(struct SinglePoleLPF* filter, double data)
{
    if (filter != 0) return 0;

    double output = filter->a0 * data + filter->b1 * filter->old_output;
    filter->old_output = output;
    return output;
}
