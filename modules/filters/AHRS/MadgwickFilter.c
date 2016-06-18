//=====================================================================================================
// MadgwickAHRS.c
//=====================================================================================================
//
// Implementation of Madgwick's IMU and AHRS algorithms.
// See: http://www.x-io.co.uk/node/8#open_source_ahrs_and_imu_algorithms
//
// Date         Author          Notes
// 29/09/2011   SOH Madgwick    Initial release
// 02/10/2011   SOH Madgwick    Optimised for reduced CPU load
// 19/02/2012   SOH Madgwick    Magnetometer measurement is normalised
//
//=====================================================================================================

#include "MadgwickFilter.h"
#include <math.h>

//---------------------------------------------------------------------------------------------------
// Definitions
//#define sampleFreq  512.0f      // sample frequency in Hz
//#define betaDef     0.1f        // 2 * proportional gain

//volatile float beta = betaDef;                              // 2 * proportional gain (Kp)
//volatile float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;  // quaternion of sensor frame relative to auxiliary frame

float invSqrt(float x);

// Initialize all values.
void MadgwickFilterInit(struct MadgwickFilter* mf)
{
    mf->beta = 0.5f;
    mf->q0 = 1;
    mf->q1 = 0;
    mf->q2 = 0;
    mf->q3 = 0;
    mf->sampleFrequency = 100.0f;
}

// AHRS algorithm update
void MadgwickFilterUpdate(
    struct MadgwickFilter* mf,
    float gx, float gy, float gz,
    float ax, float ay, float az,
    float mx, float my, float mz)
{
    float recipNorm;
    float s0, s1, s2, s3;
    float qDot1, qDot2, qDot3, qDot4;
    float hx, hy;
    float _2q0mx, _2q0my, _2q0mz, _2q1mx, _2bx, _2bz, _4bx, _4bz, _2q0, _2q1, _2q2, _2q3, _2q0q2, _2q2q3, q0q0, q0q1, q0q2, q0q3, q1q1, q1q2, q1q3, q2q2, q2q3, q3q3;

    // Use IMU algorithm if magnetometer measurement invalid (avoids NaN in magnetometer normalisation)
    if((mx == 0.0f) && (my == 0.0f) && (mz == 0.0f))
    {
        MadgwickFilterUpdateIMU(mf, gx, gy, gz, ax, ay, az);
        return;
    }

    // Rate of change of quaternion from gyroscope
    qDot1 = 0.5f * (-mf->q1 * gx - mf->q2 * gy - mf->q3 * gz);
    qDot2 = 0.5f * (mf->q0 * gx + mf->q2 * gz - mf->q3 * gy);
    qDot3 = 0.5f * (mf->q0 * gy - mf->q1 * gz + mf->q3 * gx);
    qDot4 = 0.5f * (mf->q0 * gz + mf->q1 * gy - mf->q2 * gx);

    // Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
    if(!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f)))
    {
        // Normalise accelerometer measurement
        recipNorm = invSqrt(ax * ax + ay * ay + az * az);
        ax *= recipNorm;
        ay *= recipNorm;
        az *= recipNorm;

        // Normalise magnetometer measurement
        recipNorm = invSqrt(mx * mx + my * my + mz * mz);
        mx *= recipNorm;
        my *= recipNorm;
        mz *= recipNorm;

        // Auxiliary variables to avoid repeated arithmetic
        _2q0mx = 2.0f * mf->q0 * mx;
        _2q0my = 2.0f * mf->q0 * my;
        _2q0mz = 2.0f * mf->q0 * mz;
        _2q1mx = 2.0f * mf->q1 * mx;
        _2q0 = 2.0f * mf->q0;
        _2q1 = 2.0f * mf->q1;
        _2q2 = 2.0f * mf->q2;
        _2q3 = 2.0f * mf->q3;
        _2q0q2 = 2.0f * mf->q0 * mf->q2;
        _2q2q3 = 2.0f * mf->q2 * mf->q3;
        q0q0 = mf->q0 * mf->q0;
        q0q1 = mf->q0 * mf->q1;
        q0q2 = mf->q0 * mf->q2;
        q0q3 = mf->q0 * mf->q3;
        q1q1 = mf->q1 * mf->q1;
        q1q2 = mf->q1 * mf->q2;
        q1q3 = mf->q1 * mf->q3;
        q2q2 = mf->q2 * mf->q2;
        q2q3 = mf->q2 * mf->q3;
        q3q3 = mf->q3 * mf->q3;

        // Reference direction of Earth's magnetic field
        hx = mx * q0q0 - _2q0my * mf->q3 + _2q0mz * mf->q2 + mx * q1q1 + _2q1 * my * mf->q2 + _2q1 * mz * mf->q3 - mx * q2q2 - mx * q3q3;
        hy = _2q0mx * mf->q3 + my * q0q0 - _2q0mz * mf->q1 + _2q1mx * mf->q2 - my * q1q1 + my * q2q2 + _2q2 * mz * mf->q3 - my * q3q3;
        _2bx = sqrt(hx * hx + hy * hy);
        _2bz = -_2q0mx * mf->q2 + _2q0my * mf->q1 + mz * q0q0 + _2q1mx * mf->q3 - mz * q1q1 + _2q2 * my * mf->q3 - mz * q2q2 + mz * q3q3;
        _4bx = 2.0f * _2bx;
        _4bz = 2.0f * _2bz;

        // Gradient decent algorithm corrective step
        s0 = -_2q2 * (2.0f * q1q3 - _2q0q2 - ax) + _2q1 * (2.0f * q0q1 + _2q2q3 - ay) - _2bz * mf->q2 * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (-_2bx * mf->q3 + _2bz * mf->q1) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + _2bx * mf->q2 * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
        s1 = _2q3 * (2.0f * q1q3 - _2q0q2 - ax) + _2q0 * (2.0f * q0q1 + _2q2q3 - ay) - 4.0f * mf->q1 * (1 - 2.0f * q1q1 - 2.0f * q2q2 - az) + _2bz * mf->q3 * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (_2bx * mf->q2 + _2bz * mf->q0) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + (_2bx * mf->q3 - _4bz * mf->q1) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
        s2 = -_2q0 * (2.0f * q1q3 - _2q0q2 - ax) + _2q3 * (2.0f * q0q1 + _2q2q3 - ay) - 4.0f * mf->q2 * (1 - 2.0f * q1q1 - 2.0f * q2q2 - az) + (-_4bx * mf->q2 - _2bz * mf->q0) * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (_2bx * mf->q1 + _2bz * mf->q3) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + (_2bx * mf->q0 - _4bz * mf->q2) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
        s3 = _2q1 * (2.0f * q1q3 - _2q0q2 - ax) + _2q2 * (2.0f * q0q1 + _2q2q3 - ay) + (-_4bx * mf->q3 + _2bz * mf->q1) * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (-_2bx * mf->q0 + _2bz * mf->q2) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + _2bx * mf->q1 * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
        recipNorm = invSqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3); // normalise step magnitude
        s0 *= recipNorm;
        s1 *= recipNorm;
        s2 *= recipNorm;
        s3 *= recipNorm;

        // Apply feedback step
        qDot1 -= mf->beta * s0;
        qDot2 -= mf->beta * s1;
        qDot3 -= mf->beta * s2;
        qDot4 -= mf->beta * s3;
    }

    // Integrate rate of change of quaternion to yield quaternion
    mf->q0 += qDot1 * (1.0f / mf->sampleFrequency);
    mf->q1 += qDot2 * (1.0f / mf->sampleFrequency);
    mf->q2 += qDot3 * (1.0f / mf->sampleFrequency);
    mf->q3 += qDot4 * (1.0f / mf->sampleFrequency);

    // Normalise quaternion
    recipNorm = invSqrt(mf->q0 * mf->q0 + mf->q1 * mf->q1 + mf->q2 * mf->q2 + mf->q3 * mf->q3);
    mf->q0 *= recipNorm;
    mf->q1 *= recipNorm;
    mf->q2 *= recipNorm;
    mf->q3 *= recipNorm;
}
#include <stdio.h>
//---------------------------------------------------------------------------------------------------
// IMU algorithm update
void MadgwickFilterUpdateIMU(
    struct MadgwickFilter* mf,
    float gx, float gy, float gz,
    float ax, float ay, float az)
{
    float recipNorm;
    float s0, s1, s2, s3;
    float qDot1, qDot2, qDot3, qDot4;
    float _2q0, _2q1, _2q2, _2q3, _4q0, _4q1, _4q2 ,_8q1, _8q2, q0q0, q1q1, q2q2, q3q3;

    // Rate of change of quaternion from gyroscope
    qDot1 = 0.5f * (-mf->q1 * gx - mf->q2 * gy - mf->q3 * gz);
    qDot2 = 0.5f * (mf->q0 * gx + mf->q2 * gz - mf->q3 * gy);
    qDot3 = 0.5f * (mf->q0 * gy - mf->q1 * gz + mf->q3 * gx);
    qDot4 = 0.5f * (mf->q0 * gz + mf->q1 * gy - mf->q2 * gx);

    // Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
    if(!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f)))
    {
        // Normalise accelerometer measurement
        recipNorm = invSqrt(ax * ax + ay * ay + az * az);
        ax *= recipNorm;
        ay *= recipNorm;
        az *= recipNorm;

        printf("%f %f %f\n", ax, ay, az);

        // Auxiliary variables to avoid repeated arithmetic
        _2q0 = 2.0f * mf->q0;
        _2q1 = 2.0f * mf->q1;
        _2q2 = 2.0f * mf->q2;
        _2q3 = 2.0f * mf->q3;
        _4q0 = 4.0f * mf->q0;
        _4q1 = 4.0f * mf->q1;
        _4q2 = 4.0f * mf->q2;
        _8q1 = 8.0f * mf->q1;
        _8q2 = 8.0f * mf->q2;
        q0q0 = mf->q0 * mf->q0;
        q1q1 = mf->q1 * mf->q1;
        q2q2 = mf->q2 * mf->q2;
        q3q3 = mf->q3 * mf->q3;

        // Gradient decent algorithm corrective step
        s0 = _4q0 * q2q2 + _2q2 * ax + _4q0 * q1q1 - _2q1 * ay;
        s1 = _4q1 * q3q3 - _2q3 * ax + 4.0f * q0q0 * mf->q1 - _2q0 * ay - _4q1 + _8q1 * q1q1 + _8q1 * q2q2 + _4q1 * az;
        s2 = 4.0f * q0q0 * mf->q2 + _2q0 * ax + _4q2 * q3q3 - _2q3 * ay - _4q2 + _8q2 * q1q1 + _8q2 * q2q2 + _4q2 * az;
        s3 = 4.0f * q1q1 * mf->q3 - _2q1 * ax + 4.0f * q2q2 * mf->q3 - _2q2 * ay;
        recipNorm = invSqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3); // normalise step magnitude
        s0 *= recipNorm;
        s1 *= recipNorm;
        s2 *= recipNorm;
        s3 *= recipNorm;

        // Apply feedback step
        qDot1 -= mf->beta * s0;
        qDot2 -= mf->beta * s1;
        qDot3 -= mf->beta * s2;
        qDot4 -= mf->beta * s3;
    }

    // Integrate rate of change of quaternion to yield quaternion
    mf->q0 += qDot1 * (1.0f / mf->sampleFrequency);
    mf->q1 += qDot2 * (1.0f / mf->sampleFrequency);
    mf->q2 += qDot3 * (1.0f / mf->sampleFrequency);
    mf->q3 += qDot4 * (1.0f / mf->sampleFrequency);

    // Normalise quaternion
    recipNorm = invSqrt(mf->q0 * mf->q0 + mf->q1 * mf->q1 + mf->q2 * mf->q2 + mf->q3 * mf->q3);
    mf->q0 *= recipNorm;
    mf->q1 *= recipNorm;
    mf->q2 *= recipNorm;
    mf->q3 *= recipNorm;
}

int instability_fix = 1;

//---------------------------------------------------------------------------------------------------
// Fast inverse square-root
// See: http://en.wikipedia.org/wiki/Fast_inverse_square_root
float invSqrt(float x)
{
    if (instability_fix == 0)
    {
        /* original code */
        float halfx = 0.5f * x;
        float y = x;
        long i = *(long*)&y;
        i = 0x5f3759df - (i>>1);
        y = *(float*)&i;
        y = y * (1.5f - (halfx * y * y));
        return y;
    }
    else if (instability_fix == 1)
    {
        /* close-to-optimal  method with low cost from http://pizer.wordpress.com/2008/10/12/fast-inverse-square-root */
        unsigned int i = 0x5F1F1412 - (*(unsigned int*)&x >> 1);
        float tmp = *(float*)&i;
        return tmp * (1.69000231f - 0.714158168f * x * tmp * tmp);
    }
    else
    {
        /* optimal but expensive method: */
        return 1.0f / sqrtf(x);
    }
}

