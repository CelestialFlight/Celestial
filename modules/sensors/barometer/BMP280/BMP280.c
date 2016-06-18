#include "BMP280.h"

int BMP280Init(struct BMP280* bar, void* pinData)
{
    // Defensive check.
    if (bar == 0) return SENSOR_NULL_POINTER;
    if (pinData == 0) return SENSOR_NULL_POINTER;

    // Initalize the I2C pins.
    I2CDeviceInit(&bar->i2c, 0x76, 100000, pinData);

    // Read the WHOAMI register off the chip.
    uint8_t whoAmIByte = 0;
    const uint8_t whoAmIRegister = BMP280_REG_WHOAMI;
    I2CDeviceReadRegister(&bar->i2c, whoAmIRegister, &whoAmIByte, 1);

    if (whoAmIByte != 88) return SENSOR_NOT_DETECTED;

    // Get calibration values.
    BMP280GetCalibration(bar);

    return SENSOR_CONNECTED;
}

int BMP280Enable(struct BMP280* bar)
{
    // Set oversampling settings and mode.
    BMP280SetCtrl_MeasReg(bar,
        BMP280_TEMP_X16,
        BMP280_PRES_X16,
        BMP280_NORMAL_MODE);

    BMP280SetConfigReg(bar,
        BMP280_STANDBY_0_5MS,
        BMP280_FILTER_16,
        BMP280_3WIRE_SPI_OFF);

    return 0;
}

int BMP280Sample(struct BMP280* bar)
{
    // Sample Barometer
    uint8_t data[6];
    uint8_t barReg = BMP280_REG_PRESS_MSB;
    I2CDeviceReadRegister(&bar->i2c, barReg, data, 6);

    int32_t adcTemp = (int32_t)(
        ((int32_t) data[3] << 16 | (int32_t) data[4] << 8 | data[5]) >> 4);

    int32_t adcPressure = (int32_t)(
        ((int32_t) data[0] << 16 | (int32_t) data[1] << 8 | data[2]) >> 4);

    // Take the ADC values and determine temperature and pressure.
    int32_t temp = _BMP280CalculateTemperature(bar, adcTemp);
    int32_t pressure = _BMP280CalculatePressure(bar, adcPressure);

    // Save the resulting values.
    bar->temperature = temp / 100.0;
    bar->pressure = pressure / 256.0;

    return 0;
}

// Helper function that take the ADC value and the calibration
// values to determine absolute temperature in celcius.  The resulting
// value will be a number such as "2589" which equals "25.89 celcius".
int32_t _BMP280CalculateTemperature(struct BMP280* bar, int32_t adc_T)
{
    int32_t var1, var2, T;

    var1 = ((
        ((adc_T >> 3) - ((int32_t)bar->calib.dig_T1 << 1))) *
        ((int32_t)bar->calib.dig_T2)) >> 11;

    var2 = (((
        ((adc_T >> 4) - ((int32_t)bar->calib.dig_T1)) *
        ((adc_T >> 4) - ((int32_t)bar->calib.dig_T1))) >> 12) *
        ((int32_t)bar->calib.dig_T3)) >> 14;

    bar->t_fine = var1 + var2;
    T = (bar->t_fine * 5 + 128) >> 8;
    return T;
}

// Helper function that takes the ADC value and the calibration
// values to determine absolute pressure in Pa. The resulting
// value will be an number such as "24674867", which equals
// "24674867/256 = 96386.2 Pa".
uint32_t _BMP280CalculatePressure(struct BMP280* bar, int32_t adc_P)
{
    int64_t var1, var2, p;

    var1 = ((int64_t)bar->t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)bar->calib.dig_P6;
    var2 = var2 + ((var1*(int64_t)bar->calib.dig_P5)<<17);
    var2 = var2 + (((int64_t)bar->calib.dig_P4)<<35);
    var1 = ((var1 * var1 * (int64_t)bar->calib.dig_P3)>>8) +
        ((var1 * (int64_t)bar->calib.dig_P2)<<12);
    var1 = (((((int64_t)1)<<47)+var1))*((int64_t)bar->calib.dig_P1)>>33;

    if(var1 == 0)
    {
        // avoid exception caused by division by zero
        return 0;
    }

    p = 1048576 - adc_P;
    p = (((p<<31) - var2)*3125)/var1;
    var1 = (((int64_t)bar->calib.dig_P9) * (p>>13) * (p>>13)) >> 25;
    var2 = (((int64_t)bar->calib.dig_P8) * p)>> 19;
    p = ((p + var1 + var2) >> 8) + (((int64_t)bar->calib.dig_P7)<<4);
    return (uint32_t)p;
}

// Retrives the Calibration values from the sensor.
int BMP280GetCalibration(struct BMP280* bar)
{
    uint8_t data[24];
    I2CDeviceReadRegister(&bar->i2c, BMP280_CALIB00, data, 24);

    bar->calib.dig_T1 = (uint16_t)(((uint16_t) data[1] << 8) | data[0]);
    bar->calib.dig_T2 = ( int16_t)((( int16_t) data[3] << 8) | data[2]);
    bar->calib.dig_T3 = ( int16_t)((( int16_t) data[5] << 8) | data[4]);
    bar->calib.dig_P1 = (uint16_t)(((uint16_t) data[7] << 8) | data[6]);
    bar->calib.dig_P2 = ( int16_t)((( int16_t) data[9] << 8) | data[8]);
    bar->calib.dig_P3 = ( int16_t)((( int16_t) data[11] << 8) | data[10]);
    bar->calib.dig_P4 = ( int16_t)((( int16_t) data[13] << 8) | data[12]);
    bar->calib.dig_P5 = ( int16_t)((( int16_t) data[15] << 8) | data[14]);
    bar->calib.dig_P6 = ( int16_t)((( int16_t) data[17] << 8) | data[16]);
    bar->calib.dig_P7 = ( int16_t)((( int16_t) data[19] << 8) | data[18]);
    bar->calib.dig_P8 = ( int16_t)((( int16_t) data[21] << 8) | data[20]);
    bar->calib.dig_P9 = ( int16_t)((( int16_t) data[23] << 8) | data[22]);

    return 0;
}

// Sets the Ctrl_Meas register on the bmp280.  See the datasheet
// for more details.
int BMP280SetCtrl_MeasReg(
    struct BMP280* bar, uint8_t osrs_t, uint8_t osrs_p, uint8_t mode)
{
    uint8_t data =
        (osrs_t & 0b111) << 5 |
        (osrs_p & 0b111) << 2 |
        (mode & 0b11);

    I2CDeviceWriteRegister(&bar->i2c, BMP280_REG_CTRL_MEAS, &data, 1);

    return 0;
}

// Sets config register on the bmp280.  See datasheet for more details.
int BMP280SetConfigReg(
    struct BMP280* bar, uint8_t t_sb, uint8_t filter, uint8_t spi3w_en)
{
    uint8_t data =
        (t_sb & 0b111) << 5 |
        (filter & 0b111) << 2 |
        (spi3w_en & 0b1);

    I2CDeviceWriteRegister(&bar->i2c, BMP280_REG_CONFIG, &data, 1);

    return 0;
}
