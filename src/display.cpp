#include "display.h"

// Mutex para controlar o acesso ao display
SemaphoreHandle_t Mutex_I2C;

char display[DISPLAY_L][DISPLAY_HT] = {
    {' ', ' '},
    {' ', ' '},
    {' ', ' '},
    {' ', ' '},
    {' ', ' '},
    {' ', ' '},
    {' ', ' '},
    {' ', ' '},
    {' ', ' '},
    {' ', ' '},
    {' ', ' '},
    {' ', ' '},
    {' ', ' '},
    {' ', ' '},
    {' ', ' '},
    {' ', ' '},
};

char lastDisplay[DISPLAY_L][DISPLAY_HT] = {
    {' ', ' '},
    {' ', ' '},
    {' ', ' '},
    {' ', ' '},
    {' ', ' '},
    {' ', ' '},
    {' ', ' '},
    {' ', ' '},
    {' ', ' '},
    {' ', ' '},
    {' ', ' '},
    {' ', ' '},
    {' ', ' '},
    {' ', ' '},
    {' ', ' '},
    {' ', ' '},
};

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x3F, DISPLAY_L, DISPLAY_HT);

// Inicializa o display
void initLCD()
{
    CriarMutex();

    lcd.init();
    LCD_clear();
    LCD_backlight();
}

// Inicializar o mutex do I2C para que não haja concorrencia pelo uso do display, o que pode gerar comportamentos inesperados
void CriarMutex()
{
    Mutex_I2C = xSemaphoreCreateMutex();

    if (Mutex_I2C != NULL)
        Serial.println("Mutex_I2C criado com sucesso");
    else
        Serial.println("Erro ao criar Mutex_I2C");
}

// Função que Limpa o display
void LCD_clear()
{
    xSemaphoreTake(Mutex_I2C, portMAX_DELAY);
    lcd.clear();
    xSemaphoreGive(Mutex_I2C);
}

// Função que Liga o backlight do display
void LCD_backlight()
{
    xSemaphoreTake(Mutex_I2C, portMAX_DELAY);
    lcd.backlight();
    xSemaphoreGive(Mutex_I2C);
}

// Função que seta o cursor do display
void LCD_setCursor(int x, int y)
{
    xSemaphoreTake(Mutex_I2C, portMAX_DELAY);
    lcd.setCursor(x, y);
    xSemaphoreGive(Mutex_I2C);
}

// Função que printa um caractere display
void LCD_print(int c)
{
    xSemaphoreTake(Mutex_I2C, portMAX_DELAY);
    lcd.print(c);
    xSemaphoreGive(Mutex_I2C);
}

// Atualiza o buffer caracteres
void LCD_print(int x, int y, const char *txt, int lenght)
{
    for (size_t i = x; i < DISPLAY_L; i++)
            display[i][y] = (i - x < lenght - 1) ? txt[i - x] : ' ';

    updateDisplay();
}

// verifica atualizações no buffer de caracteres e printa as diferenças
void updateDisplay(void)
{
    xSemaphoreTake(Mutex_I2C, portMAX_DELAY);
    for (byte j = 0; j < DISPLAY_HT; j++)
        for (byte i = 0; i < DISPLAY_L; i++)
            if (display != lastDisplay)
            {
                lcd.setCursor(i, j);
                lcd.print(display[i][j]);
                lastDisplay[i][j] = display[i][j];
            }
    xSemaphoreGive(Mutex_I2C);
}