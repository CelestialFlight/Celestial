#ifndef __SINGLE_POLE_LPF_H__
#define __SINGLE_POLE_LPF_H__

struct SinglePoleLPF
{
    double a0, b1;
    double old_output;
};

void SinglePoleLPFInit(
    struct SinglePoleLPF* filter,
    double sampleFreq, double cutoffFreq);
double SinglePoleLPFSample(struct SinglePoleLPF* filter, double data);

#endif
