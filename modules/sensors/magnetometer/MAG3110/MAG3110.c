#include "MAG3110.h"
#include <stdio.h>

int MAG3110Init(struct MAG3110* mag, void* pinData)
{
    if (mag == 0) return SENSOR_NULL_POINTER;
    if (pinData == 0) return SENSOR_NULL_POINTER;

    // Initialize I2C pins
    I2CDeviceInit(&mag->i2c, 0x0E, 100000, pinData);

    // Read the WHOAMI register off the chip.
    uint8_t whoAmIByte = 0;
    I2CDeviceReadRegister(&mag->i2c, MAG3110_REG_WHOAMI, &whoAmIByte, 1);

    // Ensure the whoami value returned was correct.
    if (whoAmIByte == 196) return SENSOR_CONNECTED;

    // The sensor wasn't detected, return an error.
    return SENSOR_NOT_DETECTED;
}

int MAG3110Sample(struct MAG3110* mag)
{
    // Sample Accelerometer
    uint8_t data[6];
    I2CDeviceReadRegister(&mag->i2c, MAG3110_REG_OUT_X_MSB, data, 6);

    int16_t xData = ((int16_t)data[0] << 8) | data[1];
    int16_t yData = ((int16_t)data[2] << 8) | data[3];
    int16_t zData = ((int16_t)data[4] << 8) | data[5];

    // TODO: Convert data to uT
    mag->magData.x = xData;
    mag->magData.y = yData;
    mag->magData.z = zData;

    return 0;
}

int MAG3110Enable(struct MAG3110* mag)
{
    MAG3110SetCtrl_Reg1(mag,
        MAG3110_DR_000,
        MAG3110_OS_00,
        MAG3110_FAST_READ_OFF,
        MAG3110_TM_NORMAL,
        MAG3110_ACTIVE);

    return 0;
}

// DR = dataRate
// OS = Over sampling ratio
// FR = Fast Read selection
// TM = Trigger immediate measurement
// AC = operating mode selection.
int MAG3110SetCtrl_Reg1(struct MAG3110* mag,
    uint8_t DR, uint8_t OS, uint8_t FR, uint8_t TM, uint8_t AC)
{
    uint8_t data =
        (DR & 0b111) << 5 |
        (OS & 0b11) << 3 |
        (FR & 0b1) << 2 |
        (TM & 0b1) << 1 |
        (AC & 0b1);

    I2CDeviceWriteRegister(&mag->i2c, MAG3110_REG_CTRL_REG1, &data, 1);

    return 0;
}
