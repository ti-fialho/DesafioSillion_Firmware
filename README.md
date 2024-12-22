Firmware do Desafio de Hardware proposto pela Serviços de Tecnologia LTDA

A proposta era criar um programa que liga um motor e o mantêm ligado por 10 segundo após o acionamento de um botão.

A solução foi dada pelos seguintes métodos:
Leitura de uma interrupção via harware e o tratamento do bouncing.
Queue para sinalizar o loop principal sobre o acionamento do botão.
No loop principal foi utilizado um timeout para verificar o tempo desde o ultimo acionamento.
O acionamento do motor pode ser feito via nível lógico (alto == desligado / baixo == ligado) ou via pwm.
E para o display foi utilizada a biblioteca LiquidCrystal_I2C@^1.1.4