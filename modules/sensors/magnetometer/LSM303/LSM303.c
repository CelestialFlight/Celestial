#include "LSM303.h"
#include <stdio.h>

int LSM303Init(struct LSM303* mag, void* pinData)
{
    if (mag == 0) return SENSOR_NULL_POINTER;
    if (pinData == 0) return SENSOR_NULL_POINTER;

    // Initialize I2C pins
    I2CDeviceInit(&mag->i2c, 0x1D, 100000, pinData);

    // Read the WHOAMI register off the chip.
    uint8_t whoAmIByte = 0;
    I2CDeviceReadRegister(&mag->i2c, LSM303_REG_WHO_AM_I, &whoAmIByte, 1);

    // Ensure the whoami value returned was correct.
    if (whoAmIByte == 73) return SENSOR_CONNECTED;

    // The sensor wasn't detected, return an error.
    return SENSOR_NOT_DETECTED;
}

int LSM303Enable(struct LSM303* mag)
{
    LSM303SetCtrl5(mag,
        LSM303_TEMP_OFF,
        LSM303_MAG_HIGH_RESOLUTION,
        LSM303_MAGDR_100HZ,
        LSM303_DONT_LATCH_INT1,
        LSM303_DONT_LATCH_INT2);

    LSM303SetCtrl6(mag,
        LSM303_4GAUSS);

    LSM303SetCtrl7(mag,
        LSM303_HIGHPASS_A_NORMAL,
        LSM303_HIGHPASS_A_OFF,
        LSM303_NOT_ONLY_TEMP,
        LSM303_MAG_NORMAL_POWER,
        LSM303_MAG_CONTINUOUS_MODE);

    return 0;
}

int LSM303Sample(struct LSM303* mag)
{
    // Sample Accelerometer
    uint8_t data[6];
    I2CDeviceReadRegister(&mag->i2c, LSM303_REG_OUT_X_L_M | LSM303_OUT_ALL, data, 6);

    int16_t xData = ((int16_t)data[1] << 8) | data[0];
    int16_t yData = ((int16_t)data[3] << 8) | data[2];
    int16_t zData = ((int16_t)data[5] << 8) | data[4];

    mag->magData.x = xData;
    mag->magData.y = yData;
    mag->magData.z = zData;

    return 0;
}

int LSM303SetCtrl5(struct LSM303* mag,
    uint8_t TEMP_EN, uint8_t M_RES, uint8_t M_ODR, uint8_t LIR2, uint8_t LIR1)
{
    uint8_t data =
        (TEMP_EN & 0b1) << 7 |
        (M_RES & 0b11) << 5 |
        (M_ODR & 0b111) << 2 |
        (LIR2 & 0b1) << 1 |
        (LIR1 & 0b1);

    I2CDeviceWriteRegister(&mag->i2c, LSM303_REG_CTRL5, &data, 1);

    return 0;
}

int LSM303SetCtrl6(struct LSM303* mag, uint8_t MFS)
{
    uint8_t data = (MFS&0b11) << 5;

    I2CDeviceWriteRegister(&mag->i2c, LSM303_REG_CTRL6, &data, 1);

    return 0;
}

int LSM303SetCtrl7(struct LSM303* mag,
    uint8_t AHPM, uint8_t AFDS, uint8_t T_ONLY, uint8_t MLP, uint8_t MD)
{
    uint8_t data =
       (AHPM & 0b11) << 6 |
       (AFDS & 0b1) << 5 |
       (T_ONLY & 0b1) << 4 |
       (MLP & 0b1) << 2 |
       (MD & 0b11);

    I2CDeviceWriteRegister(&mag->i2c, LSM303_REG_CTRL7, &data, 1);

    return 0;
}
