#ifndef __BMP_280_H__
#define __BMP_280_H__

#include <stdint.h>
#include "peripherals/i2cDevice.h"
#include "sensors/sensors.h"

struct BMP280Calibration
{
    uint16_t dig_T1;
    int16_t dig_T2, dig_T3;

    uint16_t dig_P1;
    int16_t dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;
};

// Driver for the BMP280 barometer.
struct BMP280
{
    struct I2CDevice i2c;

    // Temperature in Celcius
    double temperature;

    // Pressure in Pa.
    double pressure;

    int32_t t_fine;

    struct BMP280Calibration calib;
};

// Initialize the BMP280 and test that it can be connected to.
// void* pinData is a platform dependent structure for defining
// the i2c pins.
int BMP280Init(struct BMP280* bar, void* pinData);

// Enable any registers necessary for getting the BMP280 running.
int BMP280Enable(struct BMP280* bar);

// Sample the BMP280 for data and compute the measure pressures.
int BMP280Sample(struct BMP280* bar);

// Helper functions
int BMP280SetCtrl_MeasReg(
    struct BMP280* bar, uint8_t osrs_t, uint8_t osrs_p, uint8_t mode);

int BMP280SetConfigReg(
    struct BMP280* bar, uint8_t t_sb, uint8_t filter, uint8_t spi3w_en);

int BMP280GetCalibration(struct BMP280* bar);
int32_t _BMP280CalculateTemperature(struct BMP280* bar, int32_t adc_T);
uint32_t _BMP280CalculatePressure(struct BMP280* bar, int32_t adc_P);

// Registers
#define BMP280_REG_WHOAMI            0xD0
#define BMP280_REG_CTRL_MEAS         0xF4
#define BMP280_REG_CONFIG            0xF5
#define BMP280_REG_PRESS_MSB         0xF7
#define BMP280_REG_PRESS_LSB         0xF8
#define BMP280_REG_PRESS_XLSB        0xF9
#define BMP280_REG_TEMP_MSB          0xFA
#define BMP280_REG_TEMP_LSB          0xFB
#define BMP280_REG_TEMP_XLSB         0xFC
#define BMP280_CALIB00               0x88

// Pressure oversampling settings (osrs_p)
#define BMP280_SKIP_PRESSURE         0b000
#define BMP280_PRES_X1               0b001
#define BMP280_PRES_X2               0b010
#define BMP280_PRES_X4               0b011
#define BMP280_PRES_X8               0b100
#define BMP280_PRES_X16              0b101

// Temperature oversampling settings (osrs_t)
#define BMP280_SKIP_TEMP             0b000
#define BMP280_TEMP_X1               0b001
#define BMP280_TEMP_X2               0b010
#define BMP280_TEMP_X4               0b010
#define BMP280_TEMP_X8               0b100
#define BMP280_TEMP_X16              0b101

// Mode settings
#define BMP280_SLEEP_MODE            0b00
#define BMP280_FORCED_MODE           0b01
#define BMP280_NORMAL_MODE           0b11

// Standby time settings
#define BMP280_STANDBY_0_5MS         0b000
#define BMP280_STANDBY_62_5MS        0b001
#define BMP280_STANDBY_125MS         0b010
#define BMP280_STANDBY_250MS         0b011
#define BMP280_STANDBY_500MS         0b100
#define BMP280_STANDBY_1000MS        0b101
#define BMP280_STANDBY_2000MS        0b110
#define BMP280_STANDBY_4000MS        0b111

// IIR filter
#define BMP280_FILTER_OFF            0b000
#define BMP280_FILTER_2              0b001
#define BMP280_FILTER_4              0b010
#define BMP280_FILTER_8              0b011
#define BMP280_FILTER_16             0b100

// 3-wire SPI interface
#define BMP280_3WIRE_SPI_ON          1
#define BMP280_3WIRE_SPI_OFF         0

#endif
