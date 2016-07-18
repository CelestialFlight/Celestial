#ifndef __KALMAN_FILTER_SINGLE_AXIS_H__
#define __KALMAN_FILTER_SINGLE_AXIS_H__

// This implementation of the kalman filter is a simple 2-state version.
// It takes in the accelerometer & gyroscope on one axis and returns
// an approximated "noiseless" angle.`
struct KalmanFilterSingleAxis
{
    // State 0 is angle.
    // State 1 is bias of angular velocity.
    double state[2];

    // Priori error covariance Matrix.
    double P[2][2];

    // Process noise matrix.
    double Q[2][2];

    // Measurement Noise
    double R[1][1];
};

// Initializes the kalman filter so some default values.
// Q_angle is how noisy the angle approximation is on the accelerometer.
// Q_gyro is how noisy the gyroscope reading is
// R_measure is how noisy the measurements are.
// Returns 0 if succesful, -1 if null pointer.
int KalmanFilterSingleAxisInit(
    struct KalmanFilterSingleAxis* kf,
    double Q_angle,
    double Q_gyro,
    double R_measure);

// Updates the kalman filter on a single axis.  Angle is the accelerometer
// angle, rate is the gyroscope rate, and dT is the time since the last
// update in seconds.
// Returns 0 if succesful, -1 if null pointer.
int KalmanFilterSingleAxisUpdate(
    struct KalmanFilterSingleAxis* kf,
    double angle,
    double rate,
    double dT);

// Returns the current approximation for a given state.
double KalmanFilterSingleAxisGetState(
    struct KalmanFilterSingleAxis* kf, int state);

#endif
