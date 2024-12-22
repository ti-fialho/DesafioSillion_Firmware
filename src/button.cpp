#include "button.h"

// handle da queue definido no main
extern QueueHandle_t button_activation_qh;

unsigned long last_button_interrupt = 0;
bool flag_button = true;

// Task de monitoramento da flag_button
void monitoringButtonTask(void *pvParameters);
const char *monitoringButtonTaskName = "monitoringButtonTask";
const uint32_t monitoringButtonTaskStackDeph = 10000;
UBaseType_t monitoringButtonTaskPriority = 2;
TaskHandle_t *monitoringButtonTaskHandle;
TickType_t monitoringButtonTaskDelay = pdMS_TO_TICKS(5);

void setupIO(void)
{
    // define o gpio configurado no header como entrada de interrupção por borda de descida
    pinMode(BUTTON, INPUT);
    attachInterrupt(BUTTON, button_ISR, FALLING);

    xTaskCreate(
        monitoringButtonTask,
        monitoringButtonTaskName,
        monitoringButtonTaskStackDeph,
        NULL,
        monitoringButtonTaskPriority,
        monitoringButtonTaskHandle);
}

// acionado o flag, essa task verifica se após o bouncing_time o botão continuou pressionado,
// o que valida o acionamento do botão e envia uma queue para o loop principal
void monitoringButtonTask(void *pvParameters)
{
    while (true)
    {
        if (flag_button)
        {
            vTaskDelay(BOUNCE_TIME);
            if (!digitalRead(BUTTON))
                xQueueSend(button_activation_qh, &flag_button, pdMS_TO_TICKS(10));

            flag_button = false;
        }

        // pequeno delay para que o microcontrolador possa executar outras tarefas
        vTaskDelay(monitoringButtonTaskDelay);
    }
}

// a interrupção seta o flag quando é acionada
void IRAM_ATTR button_ISR()
{
    // é utilizado um timeout para que a interrupção não seja acionada muitas vezes por um bouincing
    if ((millis() - last_button_interrupt) > 100)
    {
        last_button_interrupt = millis();
        flag_button = true;
    }
}

// usado para desabilitar a interrupção, caso necessário
void unsetIO(void)
{
    detachInterrupt(BUTTON);
    vTaskDelete(monitoringButtonTaskHandle);
}