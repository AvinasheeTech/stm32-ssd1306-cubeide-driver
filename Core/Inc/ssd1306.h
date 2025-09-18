/* @file  ssd1306.h
   @brief header file for ssd1306 display basic functions.
   @author Shyam Jha (Avinashee Tech)
*/

#ifndef _SSD1306_H_
#define _SSD1306_H_

#include <stdbool.h>
#include <stdint.h>

/***********************COMMANDS**********************/

//fundamental commands
#define SSD1306_SETCONTRAST         0x81 //set contrast level
#define SSD1306_DISPLAYALLON_RESUME 0xA4 //follow RAM content - then turn display on accordingly
#define SSD1306_DISPLAYALLON        0xA5 //not follow RAM content - turn entire display on
#define SSD1306_NORMALDISPLAY       0xA6 //set normal display      
#define SSD1306_INVERTDISPLAY       0xA7 //set invert display
#define SSD1306_DISPLAYOFF          0xAE //display off (sleep)         
#define SSD1306_DISPLAYON           0xAF //display on (normal)

//scrolling commands
#define SSD1306_RIGHT_HORIZONTAL_SCROLL              0x26  //right horizontal scroll
#define SSD1306_LEFT_HORIZONTAL_SCROLL               0x27  //left horizontal scroll
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29  //vertical & right horizontal  scroll
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL  0x2A  //vertical & left horizontal scroll
#define SSD1306_DEACTIVATE_SCROLL                    0x2E  //Stop scroll
#define SSD1306_ACTIVATE_SCROLL                      0x2F  //Start scroll
#define SSD1306_SET_VERTICAL_SCROLL_AREA             0xA3  //Set scroll range

//addressing commands
#define SSD1306_SETLOWCOLUMN        0x00 //lower column addr. (for page addressing mode) 
#define SSD1306_SETHIGHCOLUMN       0x10 //higher column addr. (for page addressing mode)
#define SSD1306_MEMORYMODE          0x20 //set memory addressing mode       
#define SSD1306_COLUMNADDR          0x21 //set column start & end address         
#define SSD1306_PAGEADDR            0x22 //set page start & end address          

        
//hardware configuration commands
#define SSD1306_SETSTARTLINE        0x40 //set display start line
#define SSD1306_SEGREMAP            0xA0 //set segment remap
#define SSD1306_SETMULTIPLEX        0xA8 //set mux ratio
#define SSD1306_COMSCANINC          0xC0 //set com output scan direction
#define SSD1306_COMSCANDEC          0xC8
#define SSD1306_SETDISPLAYOFFSET    0xD3 //set display offset
#define SSD1306_SETCOMPINS          0xDA //set com pins config.
#define SSD1306_SETDISPLAYCLOCKDIV  0xD5 //set clock divide/oscillator freq.
#define SSD1306_SETPRECHARGE        0xD9 //set precharge period
#define SSD1306_SETVCOMDETECT       0xDB //set vcom deselect level

//charge pump command
#define SSD1306_CHARGEPUMP          0x8D    

#define SSD1306_EXTERNALVCC         0x01  //External display voltage source
#define SSD1306_SWITCHCAPVCC        0x02  //Generate display voltage from 3.3V

//pixel macros
#define SSD1306_BLACK   0   // Draw 'off' pixels
#define SSD1306_WHITE   1   // Draw 'on' pixels
#define SSD1306_INVERSE 2   // Invert pixels

//function declarations
void oled_clearDisplay(void);
bool oled_begin(int16_t w, int16_t h,uint8_t vcs, bool reset);
void oled_display(void);
void oled_drawPixel(int16_t x, int16_t y, uint16_t color);
void ssd1306_command(uint8_t c);
void ssd1306_commandList(const uint8_t *c, uint8_t n);
void ssd1306_datalist(const uint8_t *c, uint16_t n);

int16_t width(void);
int16_t height(void);
uint8_t getRotation(void);

#endif
