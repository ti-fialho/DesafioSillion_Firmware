#pragma once

#include <Arduino.h>
#include "gpio.h"

// define o gpio do motor com base no arquivo gpio.h
#define MOTOR GPIO_MOTOR

// altera o modo de acionamento do motor para pwm
#define PWM_OUTPUT true

#if PWM_OUTPUT
#define PWM_frequency 2000
#define PWM_resolution 10
#define PWM_channel 0
#endif

void setupMotor(void);
void set_motor_state(bool state);
bool get_motor_state(void);