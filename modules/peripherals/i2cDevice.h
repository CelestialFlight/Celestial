#ifndef __I2C_DEVICE_H__
#define __I2C_DEVICE_H__

#include <stdint.h>

struct I2CDevice
{
    // Holds info about which pins are SCL and SDA.
    // Note: Each implementation will have a different
    // way of storing this info.
    void* pinData;

    // Clock frequency in Hz.  Usually 100Khz or 400Khz.
    uint32_t frequency;

    // Address of the I2C Device
    uint8_t deviceAddress;
};

// Initializes the I2C Bus structure.
int I2CDeviceInit(
    struct I2CDevice* device, uint8_t deviceAddress, 
    uint32_t frequency, void* pinData);

// Reads a register off the i2c device.
int I2CDeviceReadRegister(
    struct I2CDevice* device, uint8_t reg, uint8_t data[], int nBytes);

// Writes to a register on an i2c device.
int I2CDeviceWriteRegister(
    struct I2CDevice* device, uint8_t reg, uint8_t data[], int nBytes);

#endif
