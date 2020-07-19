/*
Author: William Redenbaugh
Last Edit Date: 7/3/2020
*/

#include "MatrixOLED.hpp"

// GFX Font from AdafruitGFX Library, so I can do font stuff. 
#include "glcdfont.c"

#ifndef _swap_int16_t
#define _swap_int16_t(a, b)                                                    \
  {                                                                            \
    int16_t t = a;                                                             \
    a = b;                                                                     \
    b = t;                                                                     \
  }
#endif

static volatile const uint8_t ssd1351_cmd_init_list[] = {
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
    0
}; 
// END OF COMMAND LIST

/**************************************************************************/
/*!
   @brief   Sets up the Matrix OLED so it's ready to send out data!
*/
/**************************************************************************/
void MatrixOLED::begin(void){
    // Setup our gpio
    pinMode(OLED_pin_cs_ss, OUTPUT);
    pinMode(OLED_pin_res_rst, OUTPUT);
    pinMode(OLED_pin_dc_rs, OUTPUT);
    pinMode(OLED_pin_scl_sck, OUTPUT);
    pinMode(OLED_pin_sda_mosi, OUTPUT);
    
    // Lets display reset if needed. 
    this->rst_low();
    chThdSleepMilliseconds(20);
    this->rst_high();
    chThdSleepMilliseconds(20);
    this->rst_low();
    chThdSleepMilliseconds(20);
    this->rst_high();
    chThdSleepMilliseconds(20);
    this->rst_low();
    chThdSleepMilliseconds(20);
    this->rst_high();

    SPI.begin();

    // Make sure system isn't normally in reset or command mode. 
    this->dc_high();
    this->cs_high();
    this->rst_high();

    // Settup SPI Settings. 
    this->settings = SPISettings(36000000, MSBFIRST, SPI_MODE0);

    // Push out all the required settings to enable the display. 
    const uint8_t *addr = (const uint8_t *)ssd1351_cmd_init_list;
    uint8_t cmd, x, numArgs;
    while ((cmd = pgm_read_byte(addr++)) > 0) { // '0' command ends list
        x = pgm_read_byte(addr++);
        numArgs = x & 0x7F;
        if (cmd != 0xFF) { // '255' is ignored
        send_command(cmd, addr, numArgs);
        }
        addr += numArgs;
    } 

    this->set_rotation(0);
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
    SPI.transfer(out_arr, NULL, 32768, dma_event);

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

void MatrixOLED::draw_char(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size_x, uint8_t size_y){
    
    // NOTE ** TAKEN STRAIGHT FROM ADAFRUIT GFX LIBRARY! ** //
    
    if((x >= _width)             || // Clip right
      (y >= _height)             || // Clip bottom
      ((x + 6 * size_x - 1) < 0) || // Clip left
      ((y + 8 * size_y - 1) < 0))   // Clip top
          return;

        if((c >= 176)) c++; // Handle 'classic' charset behavior

        start_write();
        for(int8_t i=0; i<5; i++ ) { // Char bitmap = 5 columns
            uint8_t line = pgm_read_byte(&font[c * 5 + i]);
            for(int8_t j=0; j<8; j++, line >>= 1) {
                if(line & 1) {
                    if(size_x == 1 && size_y == 1)
                        queue_pixel(x+i, y+j, color);
                    else
                        queue_rect_fill(x+i*size_x, y+j*size_y, size_x, size_y, color);
                } else if(bg != color) {
                    if(size_x == 1 && size_y == 1)
                        queue_pixel(x+i, y+j, bg);
                    else
                        queue_rect_fill(x+i*size_x, y+j*size_y, size_x, size_y, bg);
                }
            }
        }
        end_write();
}

/**************************************************************************/
/*!
   @brief    Draw a perfectly vertical line (this is often optimized in a
   subclass!)
    @param    x   Top-most x coordinate
    @param    y   Top-most y coordinate
    @param    h   Height in pixels
   @param    color 16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void MatrixOLED::queue_draw_fast_v_line(int16_t x, int16_t y, int16_t h, uint16_t color){
  this->queue_draw_line(x, y, x, y + h - 1, color);
}

/**************************************************************************/
/*!
   @brief    Draw a perfectly horizontal line (this is often optimized in a
   subclass!)
    @param    x   Left-most x coordinate
    @param    y   Left-most y coordinate
    @param    w   Width in pixels
   @param    color 16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void MatrixOLED::queue_draw_fast_h_line(int16_t x, int16_t y, int16_t w, uint16_t color){
  this->queue_draw_line(x, y, x + w - 1, y, color);
}

/**************************************************************************/
/*!
   @brief    Draw a circle outline
    @param    x0   Center-point x coordinate
    @param    y0   Center-point y coordinate
    @param    r   Radius of circle
    @param    color 16-bit 5-6-5 Color to draw with
*/
/**************************************************************************/
void MatrixOLED::queue_draw_circle(int16_t x0, int16_t y0, int16_t r, uint16_t color){
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  this->queue_pixel(x0, y0 + r, color);
  this->queue_pixel(x0, y0 - r, color);
  this->queue_pixel(x0 + r, y0, color);
  this->queue_pixel(x0 - r, y0, color);

  while (x < y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;

    this->queue_pixel(x0 + x, y0 + y, color);
    this->queue_pixel(x0 - x, y0 + y, color);
    this->queue_pixel(x0 + x, y0 - y, color);
    this->queue_pixel(x0 - x, y0 - y, color);
    this->queue_pixel(x0 + y, y0 + x, color);
    this->queue_pixel(x0 - y, y0 + x, color);
    this->queue_pixel(x0 + y, y0 - x, color);
    this->queue_pixel(x0 - y, y0 - x, color);
  }
}

/**************************************************************************/
/*!
    @brief    Quarter-circle drawer, used to do circles and roundrects
    @param    x0   Center-point x coordinate
    @param    y0   Center-point y coordinate
    @param    r   Radius of circle
    @param    cornername  Mask bit #1 or bit #2 to indicate which quarters of
   the circle we're doing
    @param    color 16-bit 5-6-5 Color to draw with
*/
/**************************************************************************/
void MatrixOLED::queue_circle_helper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color){
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  while (x < y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
    if (cornername & 0x4) {
      this->queue_pixel(x0 + x, y0 + y, color);
      this->queue_pixel(x0 + y, y0 + x, color);
    }
    if (cornername & 0x2) {
      this->queue_pixel(x0 + x, y0 - y, color);
      this->queue_pixel(x0 + y, y0 - x, color);
    }
    if (cornername & 0x8) {
      this->queue_pixel(x0 - y, y0 + x, color);
      this->queue_pixel(x0 - x, y0 + y, color);
    }
    if (cornername & 0x1) {
      this->queue_pixel(x0 - y, y0 - x, color);
      this->queue_pixel(x0 - x, y0 - y, color);
    }
  }
}

/**************************************************************************/
/*!
   @brief    Draw a line
    @param    x0  Start point x coordinate
    @param    y0  Start point y coordinate
    @param    x1  End point x coordinate
    @param    y1  End point y coordinate
    @param    color 16-bit 5-6-5 Color to draw with
*/
/**************************************************************************/
void MatrixOLED::queue_draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color){
  int16_t steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    _swap_int16_t(x0, y0);
    _swap_int16_t(x1, y1);
  }

  if (x0 > x1) {
    _swap_int16_t(x0, x1);
    _swap_int16_t(y0, y1);
  }

  int16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int16_t ystep;

  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;
  }

  for (; x0 <= x1; x0++) {
    if (steep) {
      queue_pixel(y0, x0, color);
    } else {
      queue_pixel(x0, y0, color);
    }
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}

/**************************************************************************/
/*!
   @brief   Draw a triangle with no fill color
    @param    x0  Vertex #0 x coordinate
    @param    y0  Vertex #0 y coordinate
    @param    x1  Vertex #1 x coordinate
    @param    y1  Vertex #1 y coordinate
    @param    x2  Vertex #2 x coordinate
    @param    y2  Vertex #2 y coordinate
    @param    color 16-bit 5-6-5 Color to draw with
*/
/**************************************************************************/
void MatrixOLED::queue_draw_triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color){
  queue_draw_line(x0, y0, x1, y1, color);
  queue_draw_line(x1, y1, x2, y2, color);
  queue_draw_line(x2, y2, x0, y0, color);
} 

/**************************************************************************/
/*!
   @brief     Draw a triangle with color-fill
    @param    x0  Vertex #0 x coordinate
    @param    y0  Vertex #0 y coordinate
    @param    x1  Vertex #1 x coordinate
    @param    y1  Vertex #1 y coordinate
    @param    x2  Vertex #2 x coordinate
    @param    y2  Vertex #2 y coordinate
    @param    color 16-bit 5-6-5 Color to fill/draw with
*/
/**************************************************************************/
void MatrixOLED::queue_fill_triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color){
  int16_t a, b, y, last;

  // Sort coordinates by Y order (y2 >= y1 >= y0)
  if (y0 > y1) {
    _swap_int16_t(y0, y1);
    _swap_int16_t(x0, x1);
  }
  if (y1 > y2) {
    _swap_int16_t(y2, y1);
    _swap_int16_t(x2, x1);
  }
  if (y0 > y1) {
    _swap_int16_t(y0, y1);
    _swap_int16_t(x0, x1);
  }

  if (y0 == y2) { // Handle awkward all-on-same-line case as its own thing
    a = b = x0;
    if (x1 < a)
      a = x1;
    else if (x1 > b)
      b = x1;
    if (x2 < a)
      a = x2;
    else if (x2 > b)
      b = x2;
    this->queue_draw_fast_h_line(a, y0, b - a + 1, color);
    return;
  }

  int16_t dx01 = x1 - x0, dy01 = y1 - y0, dx02 = x2 - x0, dy02 = y2 - y0,
          dx12 = x2 - x1, dy12 = y2 - y1;
  int32_t sa = 0, sb = 0;

  // For upper part of triangle, find scanline crossings for segments
  // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
  // is included here (and second loop will be skipped, avoiding a /0
  // error there), otherwise scanline y1 is skipped here and handled
  // in the second loop...which also avoids a /0 error here if y0=y1
  // (flat-topped triangle).
  if (y1 == y2)
    last = y1; // Include y1 scanline
  else
    last = y1 - 1; // Skip it

  for (y = y0; y <= last; y++) {
    a = x0 + sa / dy01;
    b = x0 + sb / dy02;
    sa += dx01;
    sb += dx02;
    /* longhand:
    a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
    b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    */
    if (a > b)
      _swap_int16_t(a, b);
    this->queue_draw_fast_h_line(a, y, b - a + 1, color);
  }

  // For lower part of triangle, find scanline crossings for segments
  // 0-2 and 1-2.  This loop is skipped if y1=y2.
  sa = (int32_t)dx12 * (y - y1);
  sb = (int32_t)dx02 * (y - y0);
  for (; y <= y2; y++) {
    a = x1 + sa / dy12;
    b = x0 + sb / dy02;
    sa += dx12;
    sb += dx02;
    /* longhand:
    a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
    b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    */
    if (a > b)
      _swap_int16_t(a, b);
    this->queue_draw_fast_h_line(a, y, b - a + 1, color);
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
  SPI.transfer(out_arr, NULL, 32768, dma_event);

  // Should take about this long to push up animation
  chThdSleepMilliseconds(9);
  end_write();
}

/**************************************************************************/
/*!
    @brief   Set origin of (0,0) and orientation of OLED display
    @param   r
             The index for rotation, from 0-3 inclusive
    @return  None (void).
    @note    SSD1351 works differently than most (all?) other SPITFT
             displays. With certain rotation changes the screen contents
             may change immediately into a peculiar format (mirrored, not
             necessarily rotated) (other displays, this only affects new
             drawing -- rotation combinations can apply to different
             areas). Therefore, it's recommend to clear the screen
             (fillScreen(0)) before changing rotation.
*/
/**************************************************************************/
void MatrixOLED::set_rotation(uint8_t rot){
    // madctl bits:
    // 6,7 Color depth (01 = 64K)
    // 5   Odd/even split COM (0: disable, 1: enable)
    // 4   Scan direction (0: top-down, 1: bottom-up)
    // 3   Reserved
    // 2   Color remap (0: A->B->C, 1: C->B->A)
    // 1   Column remap (0: 0-127, 1: 127-0)
    // 0   Address increment (0: horizontal, 1: vertical)
    
    uint8_t madctl = 0b01100100; // 64K, enable split, CBA
    rotation = rot & 3; // Clip input to valid range

    switch (rotation) {
    
    case 0:
        madctl |= 0b00010000; // Scan bottom-up
        _width = SSD1351WIDTH;
        _height = SSD1351HEIGHT;
    break;
    case 1:
        madctl |= 0b00010011; // Scan bottom-up, column remap 127-0, vertical
        _width = SSD1351HEIGHT;
        _height = SSD1351WIDTH;
    break;
    case 2:
        madctl |= 0b00000010; // Column remap 127-0
        _width = SSD1351WIDTH;
        _height = SSD1351HEIGHT;
    break;
    
    case 3:
    madctl |= 0b00000001; // Vertical
    _width = SSD1351HEIGHT;
    _height = SSD1351WIDTH;
    break;
    
    }

    this->send_command(SSD1351_CMD_SETREMAP, &madctl, 1);
    uint8_t startline = (rotation < 2) ? SSD1351HEIGHT : 0;
    this->send_command(SSD1351_CMD_STARTLINE, &startline, 1);
}

/**************************************************************************/
/*!
 @brief   Adafruit_SPITFT Send Command handles complete sending of commands and const data
 @param   bool en (whether or not display is enabled)
*/
/**************************************************************************/
void MatrixOLED::enable_display(bool en){
  this->start_write();
  if(en)
    this->write_command(SSD1351_CMD_NORMALDISPLAY);
  else
    this->write_command(SSD1351_CMD_DISPLAYALLOFF);
  this->end_write();
}

/**************************************************************************/
/*!
 @brief   Adafruit_SPITFT Send Command handles complete sending of commands and const data
 @param   command_byte       The Command Byte
 @param   data      A pointer to the Data bytes to send
 @param   num_bytes      The number of bytes we should send
*/
/**************************************************************************/
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


/**************************************************************************/
/*!
 @brief   Adafruit_SPITFT Send Command handles complete sending of commands and data
 @param   command_byte       The Command Byte
 @param   data      A pointer to the Data bytes to send
 @param   num_bytes      The number of bytes we should send
*/
/**************************************************************************/
void MatrixOLED::send_command(uint8_t command_byte, uint8_t *data, uint8_t num_bytes){
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

// Quick Swap Preprocessor function
#define ssd1351_swap(a, b)                                                     \
  (((a) ^= (b)), ((b) ^= (a)), ((a) ^= (b))) ///< No-temp-var swap operation

/**************************************************************************/
/*!
    @brief   Set the "address window" - the rectangle we will write to
             graphics RAM with the next chunk of SPI data writes. The
             SSD1351 will automatically wrap the data as each row is filled.
    @param   x1 Leftmost column of rectangle (screen pixel coordinates).
    @param   y1 Topmost row of rectangle (screen pixel coordinates).
    @param   w Width of rectangle.
    @param   h Height of rectangle.
*/
/**************************************************************************/
void MatrixOLED::set_address_window(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h){
    uint16_t x2 = x1 + w - 1, y2 = y1 + h - 1;

    write_command(SSD1351_CMD_SETCOLUMN); // X range

    out_arr[0] = x1; 
    out_arr[1] = x2; 
    SPI.transfer(out_arr, NULL, 2, dma_event);
    chThdSleepMicroseconds(2);

    write_command(SSD1351_CMD_SETROW); // Y range

    out_arr[0] = y1; 
    out_arr[1] = y2; 
    SPI.transfer(out_arr,NULL, 2, dma_event);
    chThdSleepMicroseconds(2);

    write_command(SSD1351_CMD_WRITERAM); // Begin write
}

/**************************************************************************/
/*!
    @brief Writes 8 bit command to display. 
*/
/**************************************************************************/
void MatrixOLED::write_command(uint8_t cmd){
    this->dc_low();
    SPI.transfer(cmd);
    this->dc_high();
}

// GPIO HELPER FUNCTIONS BEGIN // 
/**************************************************************************/
/*!
  @brief Pulls the command pin high
*/
/**************************************************************************/
void MatrixOLED::dc_high(void){
    digitalWrite(OLED_pin_dc_rs, HIGH);
}

/**************************************************************************/
/*!
  @brief Pulls the command pin low
*/
/**************************************************************************/
void MatrixOLED::dc_low(void){
    digitalWrite(OLED_pin_dc_rs, LOW);
}

/**************************************************************************/
/*!
  @brief Pulls the reset pin high
*/
/**************************************************************************/
void MatrixOLED::rst_high(void){
    digitalWrite(OLED_pin_res_rst, HIGH); 
}

/**************************************************************************/
/*!
  @brief Pulls the reset pin low
*/
/**************************************************************************/
void MatrixOLED::rst_low(void){
    digitalWrite(OLED_pin_res_rst, LOW);
}

/**************************************************************************/
/*!
  @brief Pulls the chip select pin high. 
*/
/**************************************************************************/
void MatrixOLED::cs_high(void){
    digitalWrite(OLED_pin_cs_ss, HIGH);
}

/**************************************************************************/
/*!
  @brief  Pulls the chip select pin low
*/
/**************************************************************************/
void MatrixOLED::cs_low(void){
    digitalWrite(OLED_pin_cs_ss, LOW);
}
// GPIO HELPER FUNCTIONS END // 

/**************************************************************************/
/*!
  @brief Starts the SPI write process
*/
/**************************************************************************/
void MatrixOLED::start_write(void){
    SPI.beginTransaction(this->settings);
    this->cs_low();
}

/**************************************************************************/
/*!
  @brief Ends the SPI write process
*/
/**************************************************************************/
void MatrixOLED::end_write(void){
    this->cs_high();
    SPI.endTransaction();
}