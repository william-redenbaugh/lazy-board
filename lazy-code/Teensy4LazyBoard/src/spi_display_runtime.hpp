#ifndef _SPI_DISPLAY_RUNTIME_HPP
#define _SPI_DISPLAY_RUNTIME_HPP

// Arduino and ChibiOS Libraries 
#include "Arduino.h"
#include "ChRt.h"

// LCD SPI PIN DENOMINATIONS
#define LCD_SPI_PIN_CS 10
#define LCD_SPI_PIN_MOSI 11
#define LCD_SPI_PIN_MISO 12
#define LCD_SPI_PIN_CLK 13

extern void start_spi_display_thread(void);

#endif