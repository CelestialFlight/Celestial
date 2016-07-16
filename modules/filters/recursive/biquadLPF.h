#ifndef __BIQUAD_LPF_H__
#define __BIQUAD_LPF_H__

struct BiquadLPF
{
    double a0, a1, a2, b1, b2;

    double old_input[2];
    double old_output[2];
};

void BiquadLPFInit(struct BiquadLPF* lpf, double sampleFreq, double cutoffFreq);
double BiquadLPFSample(struct BiquadLPF* lpf, double data);

#endif
