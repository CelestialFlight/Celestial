#ifndef __HCSR04_H__
#define __HCSR04_H__

struct HCSR04
{
    // Distance in mm.
    double distance;

    // Data depends on the hardware celestial is implemented in.
    void* pinData;
};

// Function prototypes (need to implemented in bindings).
void HCSR04Init(struct HCSR04* ultra, void* pinData);
void HCSR04Trigger(struct HCSR04* ultra);

// Returns distance in millimeters.
double HCSR04GetDistanceMM(struct HCSR04* ultra);


#endif
