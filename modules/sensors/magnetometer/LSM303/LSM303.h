#ifndef __LSM303_H__
#define __LSM303_H__

#include "common/vector.h"
#include "peripherals/i2cDevice.h"
#include "sensors/sensors.h"

struct LSM303
{
    struct Vector3D magData;
    struct I2CDevice i2c;
};

int LSM303Init(struct LSM303* mag, void* pinData);
int LSM303Enable(struct LSM303* mag);
int LSM303Sample(struct LSM303* mag);

// Helper functions.
int LSM303SetCtrl5(struct LSM303* mag,
    uint8_t TEMP_EN, uint8_t M_RES, uint8_t M_ODR, uint8_t LIR2, uint8_t LIR1);

int LSM303SetCtrl6(struct LSM303* mag,
    uint8_t MFS);

int LSM303SetCtrl7(struct LSM303* mag,
    uint8_t AHPM, uint8_t AFDS, uint8_t T_ONLY, uint8_t MLP, uint8_t MD);

// Registers
#define LSM303_REG_TEMP_OUT_L       0x05
#define LSM303_REG_TEMP_OUT_H       0x06
#define LSM303_REG_STATUS_M         0x07
#define LSM303_REG_OUT_X_L_M        0x08
#define LSM303_REG_OUT_X_H_M        0x09
#define LSM303_REG_OUT_Y_L_M        0x0A
#define LSM303_REG_OUT_Y_H_M        0x0B
#define LSM303_REG_OUT_Z_L_M        0x0C
#define LSM303_REG_OUT_Z_H_M        0x0D
#define LSM303_REG_WHO_AM_I         0x0F
#define LSM303_REG_INT_CTRL_M       0x12
#define LSM303_REG_INT_SRC_M        0x13
#define LSM303_REG_INT_THIS_L_M     0x14
#define LSM303_REG_INT_THIS_H_M     0x15
#define LSM303_REG_OFFSET_X_L_M     0x16
#define LSM303_REG_OFFSET_X_H_M     0x17
#define LSM303_REG_OFFSET_Y_L_M     0x18
#define LSM303_REG_OFFSET_Y_H_M     0x19
#define LSM303_REG_OFFSET_Z_L_M     0x1A
#define LSM303_REG_OFFSET_Z_H_M     0x1B
#define LSM303_REG_CTRL0            0x1F
#define LSM303_REG_CTRL1            0x20
#define LSM303_REG_CTRL2            0x21
#define LSM303_REG_CTRL3            0x22
#define LSM303_REG_CTRL4            0x23
#define LSM303_REG_CTRL5            0x24
#define LSM303_REG_CTRL6            0x25
#define LSM303_REG_CTRL7            0x26
#define LSM303_OUT_ALL              (1 << 7)

// Temperature Sensor Enable
#define LSM303_TEMP_OFF             0
#define LSM303_TEMP_ON              1

// Magnetic resolution selection
#define LSM303_MAG_LOW_RESOLUTION   0b00
#define LSM303_MAG_HIGH_RESOLUTION  0b11

// Magnetic Datarate
#define LSM303_MAGDR_3_125HZ        0b000
#define LSM303_MAGDR_6_25HZ         0b001
#define LSM303_MAGDR_12_5HZ         0b010
#define LSM303_MAGDR_25HZ           0b011
#define LSM303_MAGDR_50HZ           0b100
#define LSM303_MAGDR_100HZ          0b101

// Latch interrupt request
#define LSM303_LATCH_INT1           1
#define LSM303_DONT_LATCH_INT1      0
#define LSM303_LATCH_INT2           1
#define LSM303_DONT_LATCH_INT2      0

// Magnetic scale
#define LSM303_2GAUSS               0b00
#define LSM303_4GAUSS               0b01
#define LSM303_8GAUSS               0b10
#define LSM303_12GAUSS              0b11

// Accelerometer high pass filtering mode
#define LSM303_HIGHPASS_A_NORMAL    0b00
#define LSM303_HIGHPASS_A_REF_SIG   0b01
#define LSM303_HIGHPASS_A_AUTO      0b11

// Accelerometer Highpass filter
#define LSM303_HIGHPASS_A_OFF       0
#define LSM303_HGIHPASS_A_ON        1

// Temperature sensor only (turns off magnetometer if 1)
#define LSM303_NOT_ONLY_TEMP        0
#define LSM303_ONLY_TEMP            1

// Magnetometer low power mode
#define LSM303_MAG_NORMAL_POWER     0
#define LSM303_MAG_LOW_POWER        1

// Magnetic sensor mode selection
#define LSM303_MAG_CONTINUOUS_MODE  0b00
#define LSM303_MAG_SINGLE_MODE      0b01
#define LSM303_MAG_POWER_DOWN       0b10

#endif
