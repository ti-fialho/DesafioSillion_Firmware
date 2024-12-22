#include "motor.h"

bool motor_state = true;

void setupMotor(void)
{   
    // define o gpio configurado no header como saída
    pinMode(MOTOR, OUTPUT);

#ifndef PWM_OUTPUT
    // Associa um pino a um canal de PWM
    ledcAttachPin(MOTOR, PWM_channel); // Pino , Canal

    // Configura a frequencia do PWM no respectivo canal e quantos bits de resolução o canal terá
    ledcSetup(PWM_channel, PWM_frequency, PWM_resolution); // Canal , Frequência , Bits de resolução
#endif
}

void set_motor_state(bool state)
{
    // somente atualiza o gpio se o estado for diferente do anterior
    if (state == motor_state)
        return;

    motor_state = state;

    // Acionamento com lógica invertida no hardware
#ifndef PWM_OUTPUT
    ledcWrite(PWM_channel, state ? 0 : 1024);
#else
    digitalWrite(MOTOR, state ? LOW : HIGH);
#endif
}

bool get_motor_state(void){
    return motor_state;
}