#ifndef __KALMAN_FILTER_SINGLE_AXIS_H__
#define __KALMAN_FILTER_SINGLE_AXIS_H__

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

void KalmanFilterSingleAxisInit(
	struct KalmanFilterSingleAxis* kf,
	double Q_angle,
	double Q_gyro,
	double R_measure);

void KalmanFilterSingleAxisUpdate(
	struct KalmanFilterSingleAxis* kf,
	double angle,
	double rate,
	double dT);

#endif
