#include "MPU6500.h"

// Initalizes the sensor.
int MPU6500Init(struct MPU6500* mpu, void* pinData)
{
    // Defensive check.
    if (mpu == 0) return SENSOR_NULL_POINTER;
    if (pinData == 0) return SENSOR_NULL_POINTER;

    // Max angular rate: 250 deg/sec (4.36332 rad/sec)
    mpu->gyroScale = 250 * 0.0174533;

    // Max acceleration: 2 g's.
    mpu->accelScale = 2;

    // Set default biases
    MPU6500SetAccelBias(mpu, 0, 0, 0);
    MPU6500SetGyroBias(mpu, 0, 0, 0);

    // Initalize the I2C pins.
    I2CDeviceInit(&mpu->i2c, 0x68, 100000, pinData);

    // Read the WHOAMI register off the chip.
    uint8_t whoAmIByte = 0;
    const uint8_t whoAmIRegister = 0x75;
    I2CDeviceReadRegister(&mpu->i2c, whoAmIRegister, &whoAmIByte, 1);

    // Ensure we have good communication.  The WHOAMI register on
    // the sensor should return 0x70
    if (whoAmIByte == 112) return SENSOR_CONNECTED;

    // Return an error (device couldn't be found!)
    return SENSOR_NOT_DETECTED;
}

int MPU6500Enable(struct MPU6500* mpu)
{
    // TODO: Initialize registers
    /*int16_t data = -32;
    uint8_t lowData = (data) & 0xFF;
    uint8_t highData = (data >> 8) & 0xFF;

    printf("L %d H %d\n", lowData, highData);
    I2CDeviceWriteRegister(&mpu->i2c, YG_OFFSET_L, &lowData, 1);
    I2CDeviceWriteRegister(&mpu->i2c, YG_OFFSET_H, &highData, 1);

    uint8_t tmpdata = 3;
    I2CDeviceReadRegister(&mpu->i2c, YG_OFFSET_L, &tmpdata, 1);
    printf("W %d\n", tmpdata);
    I2CDeviceReadRegister(&mpu->i2c, YG_OFFSET_H, &tmpdata, 1);
    printf("W %d\n", tmpdata);*/

    return 0;
}

void MPU6500SetAccelBias(struct MPU6500* mpu,
    double xBias, double yBias, double zBias)
{
    mpu->accelBias.x = xBias;
    mpu->accelBias.y = yBias;
    mpu->accelBias.z = zBias;
}

void MPU6500SetGyroBias(struct MPU6500* mpu,
    double xBias, double yBias, double zBias)
{
    mpu->gyroBias.x = xBias;
    mpu->gyroBias.y = yBias;
    mpu->gyroBias.z = zBias;
}

int MPU6500Sample(struct MPU6500* mpu)
{
    // Sample Accelerometer
    uint8_t data[6];
    uint8_t accelReg = 0x3B;
    I2CDeviceReadRegister(&mpu->i2c, accelReg, data, 6);

    int16_t rawAccelData[3];
    rawAccelData[0] = (int16_t)((data[0]) << 8 | data[1]);
    rawAccelData[1] = (int16_t)((data[2]) << 8 | data[3]);
    rawAccelData[2] = (int16_t)((data[4]) << 8 | data[5]);

    // Sample Gyroscope
    uint8_t gyroReg = 0x43;
    I2CDeviceReadRegister(&mpu->i2c, gyroReg, data, 6);

    int16_t rawGyroData[3];
    rawGyroData[0] = (int16_t)((data[0]) << 8 | data[1]);
    rawGyroData[1] = (int16_t)((data[2]) << 8 | data[3]);
    rawGyroData[2] = (int16_t)((data[4]) << 8 | data[5]);

    // Converts the raw data to g's
    double accelConversion = mpu->accelScale / 32767.0;
    mpu->accelData.x = rawAccelData[0] * accelConversion - mpu->accelBias.x;
    mpu->accelData.y = rawAccelData[1] * accelConversion - mpu->accelBias.y;
    mpu->accelData.z = rawAccelData[2] * accelConversion - mpu->accelBias.z;

    // Converts the raw data to rad/sec
    double gyroConversion = mpu->gyroScale / 32767.0;
    mpu->gyroData.x = rawGyroData[0] * gyroConversion - mpu->gyroBias.x;
    mpu->gyroData.y = rawGyroData[1] * gyroConversion - mpu->gyroBias.y;
    mpu->gyroData.z = rawGyroData[2] * gyroConversion - mpu->gyroBias.z;

    return 0;
}
