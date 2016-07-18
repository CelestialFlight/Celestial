#include "biquadLPF.h"
#include <math.h>
#include "common/error.h"

int BiquadLPFInit(struct BiquadLPF* lpf, double sampleFreq, double cutoffFreq)
{
    if (error(lpf != 0)) return -1;

    // Calculate all necessary constants for the biquad lpf.
    double w0 = 2 * 3.1415 * cutoffFreq / sampleFreq;
    double cosw0 = cos(w0);
    double sinw0 = sin(w0);

    double Q = 0.7;
    double alpha = sinw0 / (2 * Q);

    double b0 = (1 - cosw0)/2;
    double b1 = 1 - cosw0;
    double b2 = (1 - cosw0)/2;
    double a0 = 1 + alpha;
    double a1 = -2 * cosw0;
    double a2 = 1 - alpha;

    // a0 should never be 0.
    if (error(a0 != 0)) return 1;

    lpf->a0 = b0/a0;
    lpf->a1 = b1/a0;
    lpf->a2 = b2/a0;
    lpf->b1 = a1/a0;
    lpf->b2 = a2/a0;

    lpf->old_input[0] = lpf->old_input[1] = 0;
    lpf->old_output[0] = lpf->old_output[1] = 0;

    return 0;
}

double BiquadLPFSample(struct BiquadLPF* lpf, double data)
{
    if (error(lpf != 0)) return 0;

    double output =
        lpf->a0 * data +
        lpf->a1 * lpf->old_input[0] +
        lpf->a2 * lpf->old_input[1] -
        lpf->b1 * lpf->old_output[0] -
        lpf->b2 * lpf->old_output[1];

    lpf->old_input[1] = lpf->old_input[0];
    lpf->old_input[0] = data;

    lpf->old_output[1] = lpf->old_output[0];
    lpf->old_output[0] = output;

    return output;
}
