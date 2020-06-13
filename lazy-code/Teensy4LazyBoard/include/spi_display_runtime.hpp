#ifndef _SPI_DISPLAY_RUNTIME_HPP
#define _SPI_DISPLAY_RUNTIME_HPP

// Arduino and ChibiOS Libraries 
#include "Arduino.h"
#include "ChRt.h"

#include "Adafruit_GFX.h"
#include "Adafruit_ST7735.h"

// LCD SPI PIN DENOMINATIONS
#define LCD_SPI_PIN_CS 10
#define LCD_SPI_PIN_MOSI 11
#define LCD_SPI_PIN_MISO 12
#define LCD_SPI_PIN_CLK 13
#define LCD_SPI_PIN_RST 15
#define LCD_SPI_PIN_DC 16

extern void start_spi_display_thread(void);

#endif