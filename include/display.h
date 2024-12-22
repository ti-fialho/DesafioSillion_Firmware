#pragma once

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

// define altura e largura do display utilizado no projeto
#define DISPLAY_L 16
#define DISPLAY_HT 2

void initLCD();
void CriarMutex();
void LCD_clear();
void LCD_backlight();
void LCD_blinkEnable(bool mode);
void LCD_noCursor();
void LCD_setCursor(int x, int y);
void LCD_print(char d);
void LCD_print(int x, int y, const char * txt, int lenght);
void updateDisplay(void);