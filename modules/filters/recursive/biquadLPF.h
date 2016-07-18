#ifndef __BIQUAD_LPF_H__
#define __BIQUAD_LPF_H__

struct BiquadLPF
{
    double a0, a1, a2, b1, b2;

    double old_input[2];
    double old_output[2];
};

// Initializes the biquad filter.
// Sample Freq and cutoff Freq is in hertz.
// Ex. 400 Hz sample freq with a 20 Hz -3db cutoff freq.
// Returns 0 if succesful, -1 if null pointer.
int BiquadLPFInit(struct BiquadLPF* lpf, double sampleFreq, double cutoffFreq);

// Takes a sample at the given sampling frequency.
double BiquadLPFSample(struct BiquadLPF* lpf, double data);

#endif
