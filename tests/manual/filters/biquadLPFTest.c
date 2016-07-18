#include <stdio.h>
#include <stdlib.h>
#include "celestial.h"

// Takes in a data file and applys a single pole recursive LPF to it.
int main()
{
    FILE* fp;
    char* line = NULL;
    size_t len = 0;
    size_t read = 0;

    // Open file
    fp = fopen("../manual/filters/data.txt", "r");
    if (fp == NULL)
    {
        printf("Error!\n");
        return 1;
    }

    struct BiquadLPF lpf;
    BiquadLPFInit(&lpf, 333.33, 40.0);

    // Read in each data point and apply the filter.
    while ((read = getline(&line, &len, fp)) != -1)
    {
        float originalData = strtof(line, NULL);
        double newData = BiquadLPFSample(&lpf, originalData);
        printf("%f, %f\n", originalData, newData);
    }

    return 0;
}
