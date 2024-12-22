#pragma once

#include <Arduino.h>
#include "gpio.h"

// define o gpio do botão com base no arquivo gpio.h
#define BUTTON GPIO_BUTTON

// tempo de debounce
#define BOUNCE_TIME 20

void setupIO(void);
void monitoringButtonTask(void *pvParameters);
void IRAM_ATTR button_ISR();
void unsetIO(void);