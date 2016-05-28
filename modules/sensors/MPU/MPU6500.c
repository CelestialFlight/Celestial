#include "MPU6500.h"

// Initalizes the sensor.
int MPU6500Init(struct MPU6500* mpu, void* pinData)
{
    // Defensive check.
    if (mpu == 0) return -1;
    if (pinData == 0) return -1;

    // Max angular rate: 250 deg/sec
    mpu->gyroScale = 250;

    // Max acceleration: 2 g's.
    mpu->accelScale = 2;

    // Initalize the I2C pins.
    I2CDeviceInit(&mpu->i2c, 0x68, 100000, pinData);

    // Read the WHOAMI register off the chip.
    uint8_t whoAmIByte = 0;
    const uint8_t whoAmIRegister = 0x75;
    I2CDeviceReadRegister(&mpu->i2c, whoAmIRegister, &whoAmIByte, 1);

    // Ensure we have good communication.  The WHOAMI register on
    // the sensor should return 0x70
    if (whoAmIByte == 112) return 0;

    // Return an error (device couldn't be found!)
    return 1;
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
    mpu->accelData.x = rawAccelData[0] * accelConversion;
    mpu->accelData.y = rawAccelData[1] * accelConversion;
    mpu->accelData.z = rawAccelData[2] * accelConversion;

    // Converts the raw data to deg/sec
    double gyroConversion = mpu->gyroScale / 32767.0;
    mpu->gyroData.x = rawGyroData[0] * gyroConversion;
    mpu->gyroData.y = rawGyroData[1] * gyroConversion;
    mpu->gyroData.z = rawGyroData[2] * gyroConversion;

    return 0;
}
