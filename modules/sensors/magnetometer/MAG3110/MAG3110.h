#ifndef __MAG_3110_H__
#define __MAG_3110_H__

#include "common/vector.h"
#include "peripherals/i2cDevice.h"
#include "sensors/sensors.h"

struct MAG3110
{
    struct Vector3D magData;

    struct I2CDevice i2c;
};

int MAG3110Init(struct MAG3110* mag, void* pinData);
int MAG3110Enable(struct MAG3110* mag);
int MAG3110Sample(struct MAG3110* mag);

int MAG3110SetCtrl_Reg1(struct MAG3110* mag,
    uint8_t DR, uint8_t OS, uint8_t FR, uint8_t TM, uint8_t AC);

// Registers
#define MAG3110_REG_DR_STATUS   0x00
#define MAG3110_REG_OUT_X_MSB   0x01
#define MAG3110_REG_OUT_X_LSB   0x02
#define MAG3110_REG_OUT_Y_MSB   0x03
#define MAG3110_REG_OUT_Y_LSB   0x04
#define MAG3110_REG_OUT_Z_MSB   0x05
#define MAG3110_REG_OUT_Z_LSB   0x06
#define MAG3110_REG_WHOAMI      0x07
#define MAG3110_REG_SYSMOD      0x08
#define MAG3110_REG_OFF_X_MSB   0x09
#define MAG3110_REG_OFF_X_LSB   0x0A
#define MAG3110_REG_OFF_Y_MSB   0x0B
#define MAG3110_REG_OFF_Y_LSB   0x0C
#define MAG3110_REG_OFF_Z_MSB   0x0D
#define MAG3110_REG_OFF_Z_LSB   0x0E
#define MAG3110_REG_DIE_TEMP    0x0F
#define MAG3110_REG_CTRL_REG1   0x10
#define MAG3110_REG_CTRL_REG2   0x11

// Datarate
#define MAG3110_DR_000          0b000
#define MAG3110_DR_001          0b001
#define MAG3110_DR_010          0b010
#define MAG3110_DR_011          0b011
#define MAG3110_DR_100          0b100
#define MAG3110_DR_101          0b101
#define MAG3110_DR_110          0b110
#define MAG3110_DR_111          0b111

// Oversample
#define MAG3110_OS_00           0b00
#define MAG3110_OS_01           0b01
#define MAG3110_OS_10           0b10
#define MAG3110_OS_11           0b11

// Fast Read
#define MAG3110_FAST_READ_OFF   0
#define MAG3110_FAST_READ_ON    1

// Trigger Immediate measurement
#define MAG3110_TM_NORMAL       0
#define MAG3110_TM_TRIGGER      1

// Operating mode
#define MAG3110_STANDBY         0
#define MAG3110_ACTIVE          1

#endif
