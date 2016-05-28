#ifndef __PWM_OUTPUT_H__
#define __PWM_OUTPUT_H__

#define MIN_MOTOR_VALUE 0
#define MAX_MOTOR_VALUE 1000

#define MAX_MOTORS 8

#include <stdint.h>

struct PWMOutput
{
	int16_t motors[MAX_MOTORS];
	void* motorData;
};

void PWMOutputInit(struct PWMOutput* pwm, void* data);
void PWMOutputSetMotor(struct PWMOutput* pwm, uint8_t motor, int16_t value);

#endif