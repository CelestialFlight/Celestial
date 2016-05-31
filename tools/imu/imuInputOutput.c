#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "csvParser/csvparser.h"
#include "mflight.h"

int main()
{
	printf("Start\n");

	// Load IMU Data
	CsvParser* csvparser = CsvParser_new("../ImuData0.csv", ",", 0);
	CsvRow* row;
	int i = 0;

	FILE* logFile = fopen("test.csv", "w");

	double dT = 0.0025; // 400 Hz

	// Unfiltered data
	double gyroPitch = 0, gyroRoll = 0;

	// Complementary Filter
	struct ComplementaryFilter cf;
	ComplementaryFilterInit(&cf, 0.995);

	// Kalman Filter
	struct KalmanFilterSingleAxis kfPitch;
	struct KalmanFilterSingleAxis kfRoll;
	KalmanFilterSingleAxisInit(&kfPitch, 0.001, 0.003, 0.03);
	KalmanFilterSingleAxisInit(&kfRoll, 0.001, 0.003, 0.03);

	// Go through each row of the file
	while ((row = CsvParser_getRow(csvparser)) )
	{
		// Parse through and get each row's data.
		const char** rowFields = CsvParser_getFields(row);

		if (CsvParser_getNumFields(row) >= 8)
		{
			double accelX = strtod(rowFields[2], NULL);
			double accelY = strtod(rowFields[3], NULL);
			double accelZ = strtod(rowFields[4], NULL);
			double gyroX = strtod(rowFields[5], NULL);
			double gyroY = strtod(rowFields[6], NULL);
			double gyroZ = strtod(rowFields[7], NULL);

			double aPitch = atan2(accelY, sqrt(accelX*accelX + accelZ*accelZ));
			double aRoll = -atan2(accelX, sqrt(accelY*accelY + accelZ*accelZ));

			// Calculate Unfiltered results
			gyroPitch += gyroX * dT;
			gyroRoll += gyroY * dT;

			// Calculate Complementary filter results
			struct Vector3D accelData = {accelX, accelY, accelZ};
			struct Vector3D gyroData = {gyroX, gyroY, gyroZ};
			ComplementaryFilterUpdate(&cf, &accelData, &gyroData, dT);

			// Calculate Kalman Filter results
			KalmanFilterSingleAxisUpdate(&kfPitch, aPitch, gyroX, dT);
			KalmanFilterSingleAxisUpdate(&kfRoll, aRoll, gyroY, dT);

			// Print results to log csv file
			if (logFile != 0)
			{
				fprintf(logFile, "%f,%f,%f,%f,%f,%f,%f,%f\n",
					aPitch, aRoll,
					gyroPitch,gyroRoll,
					cf.pitch,cf.roll,
					kfPitch.state[0], kfRoll.state[0]);
			}
			else
			{
				printf("Write to file error!\n");
			}
		}

		CsvParser_destroy_row(row);
	}

	fclose(logFile);
	printf("End\n");

	return 0;
}
