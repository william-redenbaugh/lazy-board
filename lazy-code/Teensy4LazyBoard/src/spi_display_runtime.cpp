#include "spi_display_runtime.hpp"
#include "Adafruit_SSD1351.h"

// Screen dimensions
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 128 // Change this to 96 for 1.27" OLED.

// The SSD1351 is connected like this (plus VCC plus GND)
const uint8_t   OLED_pin_scl_sck        = 13;
const uint8_t   OLED_pin_sda_mosi       = 11;
const uint8_t   OLED_pin_cs_ss          = 10;
const uint8_t   OLED_pin_res_rst        = 17;
const uint8_t   OLED_pin_dc_rs          = 16;

// SSD1331 color definitions
const uint16_t  OLED_Color_Black        = 0x0000;
const uint16_t  OLED_Color_Blue         = 0x001F;
const uint16_t  OLED_Color_Red          = 0xF800;
const uint16_t  OLED_Color_Green        = 0x07E0;
const uint16_t  OLED_Color_Cyan         = 0x07FF;
const uint16_t  OLED_Color_Magenta      = 0xF81F;
const uint16_t  OLED_Color_Yellow       = 0xFFE0;
const uint16_t  OLED_Color_White        = 0xFFFF;

// The colors we actually want to use
uint16_t        OLED_Text_Color         = OLED_Color_Black;
uint16_t        OLED_Backround_Color    = OLED_Color_Blue;


// Color definitions
#define	BLACK           0x0000
#define	BLUE            0x001F
#define	RED             0xF800
#define	GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0  
#define WHITE           0xFFFF

float p = 3.1415926;

// declare the display
Adafruit_SSD1351 oled =
    Adafruit_SSD1351(
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        &SPI,
        OLED_pin_cs_ss,
        OLED_pin_dc_rs,
        OLED_pin_res_rst
     );

extern void start_spi_display_thread(void);

void testlines(uint16_t color) {
   oled.fillScreen(BLACK);
   for (uint16_t x=0; x < oled.width()-1; x+=6) {
     oled.drawLine(0, 0, x, oled.height()-1, color);
   }
   for (uint16_t y=0; y < oled.height()-1; y+=6) {
     oled.drawLine(0, 0, oled.width()-1, y, color);
   }
   
   oled.fillScreen(BLACK);
   for (uint16_t x=0; x < oled.width()-1; x+=6) {
     oled.drawLine(oled.width()-1, 0, x, oled.height()-1, color);
   }
   for (uint16_t y=0; y < oled.height()-1; y+=6) {
     oled.drawLine(oled.width()-1, 0, 0, y, color);
   }
   
   oled.fillScreen(BLACK);
   for (uint16_t x=0; x < oled.width()-1; x+=6) {
     oled.drawLine(0, oled.height()-1, x, 0, color);
   }
   for (uint16_t y=0; y < oled.height()-1; y+=6) {
     oled.drawLine(0, oled.height()-1, oled.width()-1, y, color);
   }

   oled.fillScreen(BLACK);
   for (uint16_t x=0; x < oled.width()-1; x+=6) {
     oled.drawLine(oled.width()-1, oled.height()-1, x, 0, color);
   }
   for (uint16_t y=0; y < oled.height()-1; y+=6) {
     oled.drawLine(oled.width()-1, oled.height()-1, 0, y, color);
   }
   
}

void testdrawtext(char *text, uint16_t color) {
  oled.setCursor(0,0);
  oled.setTextColor(color);
  oled.print(text);
}

void testfastlines(uint16_t color1, uint16_t color2) {
   oled.fillScreen(BLACK);
   for (uint16_t y=0; y < oled.height()-1; y+=5) {
     oled.drawFastHLine(0, y, oled.width()-1, color1);
   }
   for (uint16_t x=0; x < oled.width()-1; x+=5) {
     oled.drawFastVLine(x, 0, oled.height()-1, color2);
   }
}

void testdrawrects(uint16_t color) {
 oled.fillScreen(BLACK);
 for (uint16_t x=0; x < oled.height()-1; x+=6) {
   oled.drawRect((oled.width()-1)/2 -x/2, (oled.height()-1)/2 -x/2 , x, x, color);
 }
}

void testfillrects(uint16_t color1, uint16_t color2) {
 oled.fillScreen(BLACK);
 for (uint16_t x=oled.height()-1; x > 6; x-=6) {
   oled.fillRect((oled.width()-1)/2 -x/2, (oled.height()-1)/2 -x/2 , x, x, color1);
   oled.drawRect((oled.width()-1)/2 -x/2, (oled.height()-1)/2 -x/2 , x, x, color2);
 }
}

void testfillcircles(uint8_t radius, uint16_t color) {
  for (uint8_t x=radius; x < oled.width()-1; x+=radius*2) {
    for (uint8_t y=radius; y < oled.height()-1; y+=radius*2) {
      oled.fillCircle(x, y, radius, color);
    }
  }  
}

void testdrawcircles(uint8_t radius, uint16_t color) {
  for (uint8_t x=0; x < oled.width()-1+radius; x+=radius*2) {
    for (uint8_t y=0; y < oled.height()-1+radius; y+=radius*2) {
      oled.drawCircle(x, y, radius, color);
    }
  }  
}

void testtriangles() {
  oled.fillScreen(BLACK);
  int color = 0xF800;
  int t;
  int w = oled.width()/2;
  int x = oled.height();
  int y = 0;
  int z = oled.width();
  for(t = 0 ; t <= 15; t+=1) {
    oled.drawTriangle(w, y, y, x, z, x, color);
    x-=4;
    y+=4;
    z-=4;
    color+=100;
  }
}

void testroundrects() {
  oled.fillScreen(BLACK);
  int color = 100;
  
  int x = 0;
  int y = 0;
  int w = oled.width();
  int h = oled.height();
  for(int i = 0 ; i <= 24; i++) {
    oled.drawRoundRect(x, y, w, h, 5, color);
    x+=2;
    y+=3;
    w-=4;
    h-=6;
    color+=1100;
    Serial.println(i);
  }
}

void tftPrintTest() {
  oled.fillScreen(BLACK);
  oled.setCursor(0, 5);
  oled.setTextColor(RED);  
  oled.setTextSize(1);
  oled.println("Hello World!");
  oled.setTextColor(YELLOW);
  oled.setTextSize(2);
  oled.println("Hello World!");
  oled.setTextColor(BLUE);
  oled.setTextSize(3);
  oled.print(1234.567);
  chThdSleepMilliseconds(1500);
  oled.setCursor(0, 5);
  oled.fillScreen(BLACK);
  oled.setTextColor(WHITE);
  oled.setTextSize(0);
  oled.println("Hello World!");
  oled.setTextSize(1);
  oled.setTextColor(GREEN);
  oled.print(p, 6);
  oled.println(" Want pi?");
  oled.println(" ");
  oled.print(8675309, HEX); // print 8,675,309 out in HEX!
  oled.println(" Print HEX!");
  oled.println(" ");
  oled.setTextColor(WHITE);
  oled.println("Sketch has been");
  oled.println("running for: ");
  oled.setTextColor(MAGENTA);
  oled.print(millis() / 1000);
  oled.setTextColor(WHITE);
  oled.print(" seconds.");
}

void mediabuttons() {
 // play
  oled.fillScreen(BLACK);
  oled.fillRoundRect(25, 10, 78, 60, 8, WHITE);
  oled.fillTriangle(42, 20, 42, 60, 90, 40, RED);
  chThdSleepMilliseconds(500);
  // pause
  oled.fillRoundRect(25, 90, 78, 60, 8, WHITE);
  oled.fillRoundRect(39, 98, 20, 45, 5, GREEN);
  oled.fillRoundRect(69, 98, 20, 45, 5, GREEN);
  chThdSleepMilliseconds(500);
  // play color
  oled.fillTriangle(42, 20, 42, 60, 90, 40, BLUE);
  chThdSleepMilliseconds(50);
  // pause color
  oled.fillRoundRect(39, 98, 20, 45, 5, RED);
  oled.fillRoundRect(69, 98, 20, 45, 5, RED);
  // play color
  oled.fillTriangle(42, 20, 42, 60, 90, 40, GREEN);
}

/**************************************************************************/
/*! 
    @brief  Renders a simple test pattern on the screen
*/
/**************************************************************************/
void lcdTestPattern(void)
{
  static const uint16_t PROGMEM colors[] =
    { RED, YELLOW, GREEN, CYAN, BLUE, MAGENTA, BLACK, WHITE };

  for(uint8_t c=0; c<8; c++) {
    oled.fillRect(0, oled.height() * c / 8, oled.width(), oled.height() / 8,
      pgm_read_word(&colors[c]));
  }
}


/**************************************************************************/
/*!
    @brief Thread function and stack space for our spi display. 
*/
/**************************************************************************/
static THD_WORKING_AREA(spi_display_thread_wa, 4096);
static THD_FUNCTION(spi_display_thread, arg){
    (void)arg; 

    oled.begin();
    oled.setFont();
    oled.fillScreen(OLED_Backround_Color);

    // Looping runtime. 
    for(;;){
        
    uint16_t time = millis();
    oled.fillRect(0, 0, 128, 128, BLACK);
    time = millis() - time;
    
    Serial.println(time, DEC);
    chThdSleepMilliseconds(500);
    
    lcdTestPattern();
    chThdSleepMilliseconds(500);
    
    oled.invert(true);
    chThdSleepMilliseconds(100);
    oled.invert(false);
    chThdSleepMilliseconds(100);

    oled.fillScreen(BLACK);
    testdrawtext("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur adipiscing ante sed nibh tincidunt feugiat. Maecenas enim massa, fringilla sed malesuada et, malesuada sit amet turpis. Sed porttitor neque ut ante pretium vitae malesuada nunc bibendum. Nullam aliquet ultrices massa eu hendrerit. Ut sed nisi lorem. In vestibulum purus a tortor imperdiet posuere. ", WHITE);
    chThdSleepMilliseconds(500);

    // tft print function!
    tftPrintTest();
    chThdSleepMilliseconds(500);
    
    //a single pixel
    oled.drawPixel(oled.width()/2, oled.height()/2, GREEN);
    chThdSleepMilliseconds(500);

    // line draw test
    testlines(YELLOW);
    chThdSleepMilliseconds(500);    
    
    // optimized lines
    testfastlines(RED, BLUE);
    chThdSleepMilliseconds(500);    


    testdrawrects(GREEN);
    chThdSleepMilliseconds(1000);

    testfillrects(YELLOW, MAGENTA);
    chThdSleepMilliseconds(1000);

    oled.fillScreen(BLACK);
    testfillcircles(10, BLUE);
    testdrawcircles(10, WHITE);
    chThdSleepMilliseconds(1000);
    
    testroundrects();
    chThdSleepMilliseconds(500);
    
    testtriangles();
    chThdSleepMilliseconds(500);
    
    Serial.println("done");
    chThdSleepMilliseconds(1000);
    }   
}

/**************************************************************************/
/*!
    @brief Allows us to setup our spi display thread
*/
/**************************************************************************/
extern void start_spi_display_thread(void){
    chThdCreateStatic(spi_display_thread_wa, 
                      sizeof(spi_display_thread_wa), 
                      NORMALPRIO + 3, 
                      spi_display_thread, 
                      NULL);
}