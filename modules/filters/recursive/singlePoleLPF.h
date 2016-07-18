#ifndef __SINGLE_POLE_LPF_H__
#define __SINGLE_POLE_LPF_H__

struct SinglePoleLPF
{
    double a0, b1;
    double old_output;
};

// Initializes a single pole LPF.
// Sample Freq and cutoff Freq is in hertz.
// Ex. 400 Hz sample freq with a 20 Hz -3db cutoff freq.
// Returns 0 if succesful, -1 if null pointer.
int SinglePoleLPFInit(
    struct SinglePoleLPF* filter,
    double sampleFreq, double cutoffFreq);

// Samples and returns a filter result.
double SinglePoleLPFSample(struct SinglePoleLPF* filter, double data);

#endif
