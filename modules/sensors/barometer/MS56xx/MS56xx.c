#include "MS56xx.h"

int MS5611Init(struct MS5611* ms5, void* pinData)
{
    // Defensive check.
    if (ms5 == 0) return -1;
    if (pinData == 0) return -1;    

       // Initalize the I2C pins.
    I2CDeviceInit(&ms5->i2c, 118, 100000, pinData);

    return 0;
}

int MS5611Sample(struct MS5611* ms5)
{
    uint8_t data[3];

    // Pressure data
    I2CDeviceWriteRegister(&ms5->i2c, 0b01001000, data, 0);

    // ADC Read Sequence
    I2CDeviceWriteRegister(&ms5->i2c, 0, data, 0);    

    // TODO: This won't work on the real device as 
    //  no data should be sent!
    I2CDeviceReadRegister(&ms5->i2c, 0, data, 3);

    // TODO: This needs to be actually calculated and whatnot
    // from the datasheet.
    uint32_t pressure = 
        ((data[0]&0xFF) << 16) | 
        ((data[1]&0xFF) << 8) | 
        (data[2] & 0xFF);

    // Convert it from the sensor value to millibars
    ms5->pressure = pressure / 100.0;

    return 0;
}
