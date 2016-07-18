#ifndef __MS56XX_H__
#define __MS56XX_H__

#include "peripherals/i2cDevice.h"

struct MS5611
{
    double pressure;

    struct I2CDevice i2c;
};

// Initializes the MS5611 barometer.  
int MS5611Init(struct MS5611* ms5, void* pinData);

// Samples the sensor for data.
int MS5611Sample(struct MS5611* ms5);


#endif
