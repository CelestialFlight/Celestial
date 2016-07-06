#ifndef __PWM_OUTPUT_H__
#define __PWM_OUTPUT_H__

#define MIN_MOTOR_VALUE 0
#define MAX_MOTOR_VALUE 1000

#define MAX_MOTORS 8

#include <stdint.h>

struct PWMOutput
{
    // Current motor values.  The ranges should be 0 - 1000, unless a
    // specific platform species otherwise.
	int16_t motors[MAX_MOTORS];

	// Pin information.  This will be platform dependent, so
	// that datastructure should be defined elsewhere.
	void* motorData;
};

// Initializes the PWM Output structure and hardware.
// void* data referes to pin information about the PWM ports.
// That info will be platform dependent.
void PWMOutputInit(struct PWMOutput* pwm, void* data);

// Sets the PWM value for a given motor.
// motor is which motor number to use.`
// value is the value that should be applied to that motor.  The value should
//  be between 0 - 1000, unless a specific platform specifies otherwise.
void PWMOutputSetMotor(struct PWMOutput* pwm, uint8_t motor, int16_t value);

#endif
