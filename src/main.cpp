#include "main.h"

// timeout para desligar o motor
#define TIMEOUT_MOTOR 10000

// strings para mostar no display 16x2
const char string_motor_on[] = "MOTOR LIGADO";
const char string_motor_off[] = "MOTOR DESLIGADO";

// Task de monitoramento da flag
void monitoringTask(void *pvParameters);
const char *monitoringTaskName = "monitoringTask";
const uint32_t monitoringTaskStackDeph = 10000;
UBaseType_t monitoringTaskPriority = 2;
TaskHandle_t *monitoringTaskHandle;
TickType_t monitoringTaskDelay = pdMS_TO_TICKS(5);

QueueHandle_t button_activation_qh;

void setup()
{
    Serial.begin(115200);

    // Criar a Queue que recebe o pressionamento do botão
    button_activation_qh = xQueueCreate(5, sizeof(byte));

    initLCD();      // inicializa o lcd
    setupIO();      // configura porta io e interuupção pelo botão
    setupMotor();   // configura porta io do potor

    // presset do estado do motor
    set_motor_state(false);
    
    // preset do estado do display
    LCD_print(0, 0, string_motor_off, sizeof(string_motor_off));

    // Criar a task de monitoramento
    xTaskCreate(
        monitoringTask,
        monitoringTaskName,
        monitoringTaskStackDeph,
        NULL,
        monitoringTaskPriority,
        monitoringTaskHandle);
}

void loop()
{
    vTaskDelay(UINT32_MAX);
}

void monitoringTask(void *pvParameters)
{
    const unsigned long timeoutMotor = pdMS_TO_TICKS(TIMEOUT_MOTOR);
    unsigned long last_activation = 0;

    while (true)
    {
        // o valor do flag não tem utilidade real pois a Queue avisa se tinha algum dado na fila ou não - ou seja, se o botão foi pressionado ou não
        bool flag;

        BaseType_t queueCode;
        queueCode = xQueueReceive(button_activation_qh, &flag, pdMS_TO_TICKS(5));
        if (queueCode == pdTRUE)    // botão pressionado
        {
            last_activation = millis();                                 // salva o tempo de execução da última ativação
            set_motor_state(true);                                      // ativa o motor
            LCD_print(0, 0, string_motor_on, sizeof(string_motor_on));  // atualiza no display o estado do motor
        }

        // se o tempo de execução tiver excedido o timeout o motor é desligado
        if (millis() - last_activation > timeoutMotor)
        {
            set_motor_state(false);                                         // desliga o motor
            LCD_print(0, 0, string_motor_off, sizeof(string_motor_off));    // atualiza no display o estado do motor
        }

        // pequeno delay para que o microcontrolador possa executar outras tarefas
        vTaskDelay(monitoringTaskDelay);
    }
}