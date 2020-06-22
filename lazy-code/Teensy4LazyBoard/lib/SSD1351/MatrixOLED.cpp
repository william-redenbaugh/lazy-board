#include "MatrixOLED.hpp"

static const uint8_t PROGMEM initList[] = {
    SSD1351_CMD_COMMANDLOCK,
    1, // Set command lock, 1 arg
    0x12,
    SSD1351_CMD_COMMANDLOCK,
    1, // Set command lock, 1 arg
    0xB1,
    SSD1351_CMD_DISPLAYOFF,
    0, // Display off, no args
    SSD1351_CMD_CLOCKDIV,
    1,
    0xF1, // 7:4 = Oscillator Freq, 3:0 = CLK Div Ratio (A[3:0]+1 = 1..16)
    SSD1351_CMD_MUXRATIO,
    1,
    127,
    SSD1351_CMD_DISPLAYOFFSET,
    1,
    0x0,
    SSD1351_CMD_SETGPIO,
    1,
    0x00,
    SSD1351_CMD_FUNCTIONSELECT,
    1,
    0x01, // internal (diode drop)
    SSD1351_CMD_PRECHARGE,
    1,
    0x32,
    SSD1351_CMD_VCOMH,
    1,
    0x05,
    SSD1351_CMD_NORMALDISPLAY,
    0,
    SSD1351_CMD_CONTRASTABC,
    3,
    0xC8,
    0x80,
    0xC8,
    SSD1351_CMD_CONTRASTMASTER,
    1,
    0x0F,
    SSD1351_CMD_SETVSL,
    3,
    0xA0,
    0xB5,
    0x55,
    SSD1351_CMD_PRECHARGE2,
    1,
    0x01,
    SSD1351_CMD_DISPLAYON,
    0,  // Main screen turn on
    0}; // END OF COMMAND LIST


void MatrixOLED::begin(void){
    // Setup our gpio
    pinMode(OLED_pin_cs_ss, OUTPUT);
    pinMode(OLED_pin_res_rst, OUTPUT);
    pinMode(OLED_pin_dc_rs, OUTPUT);
    pinMode(OLED_pin_scl_sck, OUTPUT);
    pinMode(OLED_pin_sda_mosi, OUTPUT);
    
    SPI.begin();

    // Make sure system isn't normally in reset or command mode. 
    this->dc_high();
    this->cs_high();
    this->rst_high();

    // Settup SPI Settings. 
    this->settings = SPISettings(36000000, MSBFIRST, SPI_MODE0);

    // Push out all the required settings to enable the display. 
    const uint8_t *addr = (const uint8_t *)initList;
    uint8_t cmd, x, numArgs;
    while ((cmd = pgm_read_byte(addr++)) > 0) { // '0' command ends list
        x = pgm_read_byte(addr++);
        numArgs = x & 0x7F;
        if (cmd != 0xFF) { // '255' is ignored
        send_command(cmd, addr, numArgs);
        }
        addr += numArgs;
    } 
}

/**************************************************************************/
/*!
   @brief    Fill the screen completely with one color. Update in subclasses if desired!
    @param    color 16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void MatrixOLED::fill_screen(uint16_t color){
    // write inforation
    uint16_t i = 0; 
    for(uint8_t y = 0; y < SSD1351HEIGHT; y++){
        for(uint8_t x = 0; x < SSD1351WIDTH; x++){
            out_arr[i] = color >> 8;
            i++; 
            out_arr[i] = color; 
            i++;
        }
    }

    this->start_write();

    // Set the size of the address window.
    this->set_address_window(0, 0, SSD1351WIDTH, SSD1351HEIGHT);

    // Start DMA transfer 
    SPI.transfer(out_arr, in_arr, 32768, dma_event);

    // Should take about this long to push up animation, so we can sleep the thread in the meantime. 
    chThdSleepMilliseconds(9);
    
    this->end_write();
}

/**************************************************************************/
/*!
  @brief    Writes a pixel to the pixel buffer with a desired color
  @param    x position
  @param    y position
  @param    color of the pixel. 
*/
/**************************************************************************/
void MatrixOLED::queue_pixel(uint8_t x, uint8_t y, uint16_t color){
  if((x < 128 && y < 128)){
    out_arr[((127 - x) * SSD1351HEIGHT + y) * 2] = color >> 8; 
    out_arr[((127 - x) * SSD1351WIDTH+ y) * 2 + 1] = color;
  } 
}

/**************************************************************************/
/*!
  @brief    Write a rectangle with no fill of desired size and desired color
  @param    x position
  @param    y position
  @param    w how wide the rectangle is 
  @param    h how tall the rectangle is. 
  @param    color of the pixel. 
*/
/**************************************************************************/
void MatrixOLED::queue_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t col){
  if(((x + w) < 128 && (y + h) < 128)){
    
    for(uint8_t x_len = x; x_len < (x + w); x_len++){
      this->queue_pixel(x_len, y, col);
      this->queue_pixel(x_len, y + h-1, col);  
    }

    for(uint8_t y_len = y; y_len < (y + h); y_len++){
      this->queue_pixel(x + w-1, y_len, col);
      this->queue_pixel(x, y_len, col);  
    }
  }
}
/**************************************************************************/
/*!
  @brief    Write a rectangle with fill of desired size and desired color
  @param    x position
  @param    y position
  @param    w how wide the rectangle is 
  @param    h how tall the rectangle is. 
  @param    color of the pixel. 
*/
/**************************************************************************/
void MatrixOLED::queue_rect_fill(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t col){
  if(((x + w) < 128 && (y + h) < 128)){
    for(uint8_t x_len = x; x_len < (x + w); x_len++){
      for(uint8_t y_len = y; y_len < (y + h); y_len++){
        this->queue_pixel(x_len, y_len, col);
      }
    }
  }
}


/**************************************************************************/
/*!
  @brief    Draws out the buffer to the display
*/
/**************************************************************************/
void MatrixOLED::draw_queue(void){
  // Starts writing buffer to display via dma. 
  start_write();

  // so we know the size of the window, which is full size. 
  set_address_window(0, 0, SSD1351WIDTH, SSD1351HEIGHT);
  // Doing a DMA transfer. 
  SPI.transfer(out_arr, in_arr, 32768, dma_event);

  // Should take about this long to push up animation
  chThdSleepMilliseconds(9);
  end_write();
}

/*!
 @brief   Adafruit_SPITFT Send Command handles complete sending of commands and const data
 @param   command_byte       The Command Byte
 @param   data      A pointer to the Data bytes to send
 @param   num_bytes      The number of bytes we should send
 */
void MatrixOLED::send_command(uint8_t command_byte, const uint8_t *data, uint8_t num_bytes){
    // Starts spi transation process. 
    this->start_write();

    // Sends our command byte using the command mode. 
    this->write_command(command_byte);

    for(uint8_t n = 0; n < num_bytes; n++){
        SPI.transfer(*data);
        data++;
    }

    // Ends the SPI transaction process. 
    this->end_write();
}

#define ssd1351_swap(a, b)                                                     \
  (((a) ^= (b)), ((b) ^= (a)), ((a) ^= (b))) ///< No-temp-var swap operation

/*!
    @brief   Set the "address window" - the rectangle we will write to
             graphics RAM with the next chunk of SPI data writes. The
             SSD1351 will automatically wrap the data as each row is filled.
    @param   x1 Leftmost column of rectangle (screen pixel coordinates).
    @param   y1 Topmost row of rectangle (screen pixel coordinates).
    @param   w Width of rectangle.
    @param   h Height of rectangle.
*/
void MatrixOLED::set_address_window(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h){
    uint16_t x2 = x1 + w - 1, y2 = y1 + h - 1;

    write_command(SSD1351_CMD_SETCOLUMN); // X range

    out_arr[0] = x1; 
    out_arr[1] = x2; 
    SPI.transfer(out_arr, in_arr, 2, dma_event);
    chThdSleepMicroseconds(2);

    write_command(SSD1351_CMD_SETROW); // Y range

    out_arr[0] = y1; 
    out_arr[1] = y2; 
    SPI.transfer(out_arr,in_arr, 2, dma_event);
    chThdSleepMicroseconds(2);

    write_command(SSD1351_CMD_WRITERAM); // Begin write
}

void MatrixOLED::write_command(uint8_t cmd){
    this->dc_low();
    SPI.transfer(cmd);
    this->dc_high();
}

// GPIO HELPER FUNCTIONS BEGIN // 
void MatrixOLED::dc_high(void){
    digitalWrite(OLED_pin_dc_rs, HIGH);
}

void MatrixOLED::dc_low(void){
    digitalWrite(OLED_pin_dc_rs, LOW);
}

void MatrixOLED::rst_high(void){
    digitalWrite(OLED_pin_res_rst, HIGH); 
}

void MatrixOLED::rst_low(void){
    digitalWrite(OLED_pin_res_rst, LOW);
}

void MatrixOLED::cs_low(void){
    digitalWrite(OLED_pin_cs_ss, LOW);
}

void MatrixOLED::cs_high(void){
    digitalWrite(OLED_pin_cs_ss, HIGH);
}
// GPIO HELPER FUNCTIONS END // 

void MatrixOLED::start_write(void){
    SPI.beginTransaction(this->settings);
    this->cs_low();
}

void MatrixOLED::end_write(void){
    this->cs_high();
    SPI.endTransaction();
}