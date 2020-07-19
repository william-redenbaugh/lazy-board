/*
Author: William Redenbaugh
Last Edit Date: 7/3/2020
*/

#ifndef _MATRIX_OLED_HPP
#define _MATRIX_OLED_HPP

// All the libraries we need to write our interface. 
#include "Arduino.h"
#include "ChRt.h"
#include "SPI.h"

// These #defines are DEPRECATED but present for older code compatibility:
#define SSD1351WIDTH 128  ///< DEPRECATED screen width
#define SSD1351HEIGHT 128 ///< DEPRECATED screen height, set to 96 for 1.27"
// (NEW CODE SHOULD IGNORE THIS, USE THE CONSTRUCTORS THAT ACCEPT WIDTH
// AND HEIGHT ARGUMENTS).

#define SSD1351_CMD_SETCOLUMN 0x15      ///< See datasheet
#define SSD1351_CMD_SETROW 0x75         ///< See datasheet
#define SSD1351_CMD_WRITERAM 0x5C       ///< See datasheet
#define SSD1351_CMD_READRAM 0x5D        ///< Not currently used
#define SSD1351_CMD_SETREMAP 0xA0       ///< See datasheet
#define SSD1351_CMD_STARTLINE 0xA1      ///< See datasheet
#define SSD1351_CMD_DISPLAYOFFSET 0xA2  ///< See datasheet
#define SSD1351_CMD_DISPLAYALLOFF 0xA4  ///< Not currently used
#define SSD1351_CMD_DISPLAYALLON 0xA5   ///< Not currently used
#define SSD1351_CMD_NORMALDISPLAY 0xA6  ///< See datasheet
#define SSD1351_CMD_INVERTDISPLAY 0xA7  ///< See datasheet
#define SSD1351_CMD_FUNCTIONSELECT 0xAB ///< See datasheet
#define SSD1351_CMD_DISPLAYOFF 0xAE     ///< See datasheet
#define SSD1351_CMD_DISPLAYON 0xAF      ///< See datasheet
#define SSD1351_CMD_PRECHARGE 0xB1      ///< See datasheet
#define SSD1351_CMD_DISPLAYENHANCE 0xB2 ///< Not currently used
#define SSD1351_CMD_CLOCKDIV 0xB3       ///< See datasheet
#define SSD1351_CMD_SETVSL 0xB4         ///< See datasheet
#define SSD1351_CMD_SETGPIO 0xB5        ///< See datasheet
#define SSD1351_CMD_PRECHARGE2 0xB6     ///< See datasheet
#define SSD1351_CMD_SETGRAY 0xB8        ///< Not currently used
#define SSD1351_CMD_USELUT 0xB9         ///< Not currently used
#define SSD1351_CMD_PRECHARGELEVEL 0xBB ///< Not currently used
#define SSD1351_CMD_VCOMH 0xBE          ///< See datasheet
#define SSD1351_CMD_CONTRASTABC 0xC1    ///< See datasheet
#define SSD1351_CMD_CONTRASTMASTER 0xC7 ///< See datasheet
#define SSD1351_CMD_MUXRATIO 0xCA       ///< See datasheet
#define SSD1351_CMD_COMMANDLOCK 0xFD    ///< See datasheet
#define SSD1351_CMD_HORIZSCROLL 0x96    ///< Not currently used
#define SSD1351_CMD_STOPSCROLL 0x9E     ///< Not currently used
#define SSD1351_CMD_STARTSCROLL 0x9F    ///< Not currently used

#define OLED_pin_scl_sck 13
#define OLED_pin_sda_mosi 11
#define OLED_pin_cs_ss 10
#define OLED_pin_res_rst 8
#define OLED_pin_dc_rs 9

class MatrixOLED{
    public: 
        void begin(void);

        // New fill screen command that allows us to use SPI DMA transfers. 
        void fill_screen(uint16_t color);

        // INFO KEEP FRAMEBUFFER BEGIN //
        
        // Draw primitives. 
        void queue_pixel(uint8_t x, uint8_t y, uint16_t color);
        void queue_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t col);
        void queue_rect_fill(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t col);
        void queue_draw_fast_v_line(int16_t x, int16_t y, int16_t h, uint16_t color);
        void queue_draw_fast_h_line(int16_t x, int16_t y, int16_t h, uint16_t color);
        void queue_draw_circle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
        void queue_circle_helper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);
        void queue_draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
        void queue_draw_triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color); 
        void queue_fill_triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
        
        // Draws a character wherever :0
        void draw_char(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size_x, uint8_t size_y);
        
        // Allows us to actually draw the character 
        void draw_queue(void);
        
        // INFO KEEP FRAMEBUFFER END // 

        void set_rotation(uint8_t rot);
        void enable_display(bool en);
        
    private: 

        void send_command(uint8_t command_byte, const uint8_t *data, uint8_t num_bytes);
        void send_command(uint8_t command_byte, uint8_t *data, uint8_t num_bytes);
        void set_address_window(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h);
        void write_command(uint8_t cmd);

        // Easy command to pull high or low the dc and rst pins. 
        void dc_high(void);
        void dc_low(void);
        void rst_high(void);
        void rst_low(void);
        void cs_high(void);
        void cs_low(void);

        // Allows us to setup enabling pushing data
        void start_write(void);
        void end_write(void);
        // Allows us to disable enabling pushing data. 

        // Spi Settings used for dealing with data transmission
        SPISettings settings; 

        // Buffers needed for async transfer.         
        uint8_t out_arr[32768];
        
        // DMA event responder.
        EventResponder dma_event;

        uint8_t _width = SSD1351WIDTH; 
        uint8_t _height = SSD1351HEIGHT;
        uint8_t rotation = 0; 
};

#endif 