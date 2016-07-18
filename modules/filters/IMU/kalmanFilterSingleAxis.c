#include "kalmanFilterSingleAxis.h"
#include "common/error.h"

int KalmanFilterSingleAxisInit(
    struct KalmanFilterSingleAxis* kf, double Q_angle, double Q_gyro, double R_measure)
{
    // Defensive check.
    if (error(kf != 0)) return 0;

    // Set the initial angle and initial bias to 0.
    kf->state[0] = 0;
    kf->state[1] = 0;

    // We assume we know the initial states of the
    // angle and bias (as we should calibrate before flying)
    kf->P[0][0] = 10000;
    kf->P[0][1] = 0;
    kf->P[1][0] = 10000;
    kf->P[1][1] = 0;

    kf->Q[0][0] = Q_angle;
    kf->Q[1][1] = Q_gyro;
    kf->R[0][0] = R_measure;

    return 0;
}

int KalmanFilterSingleAxisUpdate(
    struct KalmanFilterSingleAxis* kf, double angle, double rate, double dT)
{
    // Defensive check.
    if (error(kf != 0)) return -1;

    // Predict
    kf->state[0] = kf->state[0] + rate * dT - kf->state[1] * dT;

    // State covariance update.
    kf->P[0][0] += dT *
        (dT * kf->P[1][1] - kf->P[0][1] - kf->P[1][0] + kf->Q[0][0]);
    kf->P[0][1] -= dT * kf->P[1][1];
    kf->P[1][0] -= dT * kf->P[1][1];
    kf->P[1][1] += kf->Q[1][1] * dT;

    // Update
    // Difference between the measured value and the state
    double y = angle - kf->state[0];

    // Measurement covariance update.
    double S = kf->P[0][0] + kf->R[0][0];

    // Throw an error if S is somehow zero.
    if (error(S != 0)) return 1;

    // Calculate Kalman Gain.
    double kAngle = kf->P[0][0] / S;
    double kBias = kf->P[1][0] / S;

    // Improve upon state estimates.
    kf->state[0] += kAngle * y;
    kf->state[1] += kBias * y;

    // Update state covariance, given this new information.
    double tmp00 = kf->P[0][0];
    double tmp01 = kf->P[0][1];

    kf->P[0][0] -= kAngle * tmp00;
    kf->P[0][1] -= kAngle * tmp01;
    kf->P[1][0] -= kBias * tmp00;
    kf->P[1][1] -= kBias * tmp01;

    return 0;
}

double KalmanFilterSingleAxisGetState(
    struct KalmanFilterSingleAxis* kf, int state)
{
    // Defensive Check.
    if (error(kf != 0)) return 0;
    if (error(state > 1)) return 0; // State can only be 0 or 1

    return kf->state[state];
}

