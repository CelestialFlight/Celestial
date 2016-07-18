#ifndef __MPU_6500_H__
#define __MPU_6500_H__

#include <stdint.h>
#include "common/vector.h"
#include "peripherals/i2cDevice.h"
#include "sensors/sensors.h"

struct MPU6500
{
    // Accelerometer and gyroscope data from the sensor.
    struct Vector3D gyroData;
    struct Vector3D accelData;

    // Biases that are subtracted from the sensor data.
    // By default, these are 0.
    struct Vector3D gyroBias;
    struct Vector3D accelBias;

    // Highest rotational velocity that be detected.
    // Can be: 250, 500, 1000, 2000 deg/sec.
    double gyroScale;

    // Highest acceleration that can be detected.
    // Can be 2, 4, 8, or 16g's
    double accelScale;

    // Increments everytime a new piece of data is saved.
    uint32_t updateCount;

    // Information about how to communicate to the sensor
    // over I2C.
    struct I2CDevice i2c;
};

// Initializes the MPU6500 sensor.  PinData is dependent on
// which device this code is running on, see I2C examples.
// This function call will set up the I2C pins given by
// pinData.
int MPU6500Init(struct MPU6500* mpu, void* pinData);

// Enables the sensor to the correct state.
int MPU6500Enable(struct MPU6500* mpu);

// Samples the sensor for data.
int MPU6500Sample(struct MPU6500* mpu);

// Accelerometer Bias
void MPU6500SetAccelBias(struct MPU6500* mpu,
    double xBias, double yBias, double zBias);

// Gyroscope Bias
void MPU6500SetGyroBias(struct MPU6500* mpu,
    double xBias, double yBias, double zBias);

// Gyroscope Offset Registers
#define XG_OFFSET_H 0x13
#define XG_OFFSET_L 0x14
#define YG_OFFSET_H 0x15
#define YG_OFFSET_L 0x16
#define ZG_OFFSET_H 0x17
#define ZG_OFFSET_L 0x18

#endif
