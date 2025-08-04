#include "ssd1306.h"
#include "spi.h"

//macros
#define HIGH 1
#define LOW  0
#define SSD1306_SELECT       oled.set_cs(LOW);      // Device select
#define SSD1306_DESELECT     oled.set_cs(HIGH);     // Device deselect
#define SSD1306_MODE_COMMAND oled.set_dc(LOW);      // Command mode
#define SSD1306_MODE_DATA    oled.set_dc(HIGH);     // Data mode
#define DELAY(microseconds)  delay_us(microseconds); //microsecond delay
#define ssd1306_swap(a, b)   (((a) ^= (b)), ((b) ^= (a)), ((a) ^= (b))) // No-temp-var swap operation

//spi peripheral structure
typedef struct{
    void (*init)(void);
    void (*transfer)(uint8_t *c,size_t len);
    void (*set_cs)(bool level);
    void (*set_dc)(bool level);
    void (*set_reset)(bool level);
}spihandle_t;

//initialize spi structure
spihandle_t oled = {.init=spi_init,.transfer=spi_transfer,.set_cs=spi_set_cs,.set_dc=spi_set_dc,.set_reset=spi_reset};

//variables
uint8_t *buffer;  // Buffer data used for display buffer
int8_t vccstate;  // VCC selection
uint8_t contrast; // normal contrast setting for this device
int16_t WIDTH;    // display width
int16_t HEIGHT;   // display height
int16_t _width;   // display width as modified by current rotation
int16_t _height;  // display height as modified by current rotation
uint8_t rotation; // display rotation (0 thru 3)
uint8_t command_val = 0;

//spi pins (defined in spi.c)
extern int8_t mosiPin;   // (Master Out Slave In) - SPI
extern int8_t clkPin;    // (Clock Pin) - SPI
extern int8_t dcPin;     // (Data Pin) - SPI
extern int8_t csPin;     // (Chip Select Pin) - SPI 
extern int8_t rstPin;    // Display reset pin assignment

/**  
 * @brief  microseconds delay function
 * @param  us microseconds value
 * @retval None.
*/
void delay_us(int us){
    //delay in microseconds
}

/**  
 * @brief  initialize non spi gpios 
 * @param  None
 * @retval None.
*/
void gpio_init(void){
  //init gpio pins (CS,D/C and RESET)
}


/**  
 * @brief  Clear contents of display buffer (set all pixels to off).
 * @retval None.
 * @note   Changes buffer contents only, no immediate effect on display.
*/
void oled_clearDisplay() {
  memset(buffer, 0, WIDTH * ((HEIGHT + 7) / 8));
}


/**
 * @brief  Allocate RAM for image buffer, initialize peripherals and pins.
 * @param  w
           width of the display
 * @param  h
           height of the display 
 * @param  vcs
            VCC selection. Pass SSD1306_SWITCHCAPVCC to generate the display
            voltage (step up) from the 3.3V source.
 * @param  reset
            If true, and if the reset pin passed is valid, a hard reset will 
            be performed before initializing the display.
 * @retval true on successful allocation/init, false otherwise.
 * @note   MUST call this function before any drawing or updates!
*/
bool oled_begin(int16_t w, int16_t h, uint8_t vcs, bool reset) {

  WIDTH = w;
  HEIGHT = h;
  _width = WIDTH;
  _height = HEIGHT;
  rotation = 0;

  if ((!buffer) && !(buffer = (uint8_t *)malloc(WIDTH * ((HEIGHT + 7) / 8)))){
    return false;
  }
  
  oled_clearDisplay();
  vccstate = vcs;

  //Initialize non-SPI GPIOs
  //CS, D/C and RESET pins
  gpio_init();

  // SPI peripheral initialize
  oled.init();

  // Reset SSD1306 if requested
  if (reset && (rstPin >= 0)) {
    oled.set_reset(HIGH);
    DELAY(1000);          // VDD goes high at start, pause for 1 ms
    oled.set_reset(LOW);      // Bring reset low
    DELAY(10000);         // Wait 10 ms
    oled.set_reset(HIGH);     // Bring out of reset
  }

  SSD1306_SELECT

// Init sequence
  static const uint8_t init1[] = {SSD1306_DISPLAYOFF,         // 0xAE
                                  SSD1306_SETDISPLAYCLOCKDIV, // 0xD5
                                  0x80, // the suggested ratio 0x80
                                  SSD1306_SETMULTIPLEX}; // 0xA8
  ssd1306_commandList(init1, sizeof(init1));
  command_val = HEIGHT - 1;
  ssd1306_command(command_val);

  static const uint8_t init2[] = {SSD1306_SETDISPLAYOFFSET, // 0xD3
                                  0x0,                      // no offset
                                  SSD1306_SETSTARTLINE | 0x0, // line #0
                                  SSD1306_CHARGEPUMP};        // 0x8D
  ssd1306_commandList(init2, sizeof(init2));
  command_val = (vccstate == SSD1306_EXTERNALVCC) ? 0x10 : 0x14;
  ssd1306_command(command_val);

  static const uint8_t init3[] = {SSD1306_MEMORYMODE, // 0x20
                                  0x00, 
                                  SSD1306_SEGREMAP,
                                  SSD1306_COMSCANINC};
  ssd1306_commandList(init3, sizeof(init3));

  uint8_t comPins = 0x02;
  contrast = 0x8F;

  if ((WIDTH == 128) && (HEIGHT == 32)) {
    comPins = 0x02;
    contrast = 0x8F;
  } else if ((WIDTH == 128) && (HEIGHT == 64)) {
    comPins = 0x12;
    contrast = (vccstate == SSD1306_EXTERNALVCC) ? 0x9F : 0xCF;
  } else if ((WIDTH == 96) && (HEIGHT == 16)) {
    comPins = 0x2; // ada x12
    contrast = (vccstate == SSD1306_EXTERNALVCC) ? 0x10 : 0xAF;
  } else {
    // Other screen varieties -- TBD
  }

  command_val = SSD1306_SETCOMPINS;
  ssd1306_command(command_val);
  command_val = comPins;
  ssd1306_command(command_val);
  command_val = SSD1306_SETCONTRAST;
  ssd1306_command(command_val);
  command_val = contrast;
  ssd1306_command(command_val);
  command_val = SSD1306_SETPRECHARGE;
  ssd1306_command(command_val); // 0xd9
  command_val = (vccstate == SSD1306_EXTERNALVCC) ? 0x22 : 0xF1;
  ssd1306_command(command_val);
  static const uint8_t init5[] = {
      SSD1306_SETVCOMDETECT, // 0xDB
      0x20,
      SSD1306_DISPLAYALLON_RESUME, // 0xA4
      SSD1306_NORMALDISPLAY,       // 0xA6
      SSD1306_DEACTIVATE_SCROLL,
      SSD1306_DISPLAYON}; // Main screen turn on
  ssd1306_commandList(init5, sizeof(init5));

  SSD1306_DESELECT

  return true; // Success
}

/** 
 * @brief Issue single command to SSD1306, using SPI as
   needed. Because command calls are often grouped, SPI transaction and
   selection must be started/ended in calling function for efficiency.
 * @param c the command character to send to the display.
          Refer to ssd1306 data sheet for commands
 * @retval None.
 * @note
*/
void ssd1306_command(uint8_t c) {
   // SPI -- transaction started in calling function
    SSD1306_MODE_COMMAND
    oled.transfer(&c,sizeof(uint8_t));
}

/**
 * @brief Issue list of commands to SSD1306, same rules as above re:
   transactions.
 * @param c pointer to list of commands
 * @param n number of commands in the list
 * @retval None.
 * @note
*/
void ssd1306_commandList(const uint8_t *c, uint8_t n) {
   // SPI -- transaction started in calling function
    SSD1306_MODE_COMMAND
    oled.transfer(c,n);
}

/** 
 * @brief Issue list of data to SSD1306, using SPI as
   needed. Because command calls are often grouped, SPI transaction and
   selection must be started/ended in calling function for efficiency.
 * @param c pointer to list of data
 * @param n number of data in the list
 * @retval None.
 * @note
*/
void ssd1306_datalist(const uint8_t *c, uint16_t n) {
   // SPI -- transaction started in calling function
    SSD1306_MODE_DATA
    oled.transfer(c,n);
}

/**
 * @brief  Push data currently in RAM to SSD1306 display.
 * @retval None (void).
 * @note   Drawing operations are not visible until this function is
            called. Call after each graphics command, or after a whole set
            of graphics commands, as best needed by one's own application.
*/
void oled_display(void) {
  SSD1306_SELECT
  static const uint8_t dlist1[] = {
      SSD1306_PAGEADDR,
      0x00,                   // Page start address
      0x07,                   // Page end address
      SSD1306_COLUMNADDR, 
      0x00};                  // Column start address
                        

  ssd1306_commandList(dlist1, sizeof(dlist1));
  command_val = WIDTH - 1;
  ssd1306_command(command_val); // Column end address

  uint16_t count = WIDTH * ((HEIGHT + 7) / 8);
  uint8_t *ptr = buffer;
  // SPI
  ssd1306_datalist(ptr,count);
  SSD1306_DESELECT

}


/**
 * @brief     Get width of the display, accounting for current rotation
 * @retval    Width in pixels
*/
int16_t width(void) { return _width; }


/**
 * @brief     Get height of the display, accounting for current rotation
 * @retval    Height in pixels
*/
int16_t height(void) { return _height; }

/**
 * @brief     Get rotation setting for display
 * @retval    0 thru 3 corresponding to 4 cardinal rotations
*/
uint8_t getRotation(void) { return rotation; }



/** 
 * @brief  Set/clear/invert a single pixel.
 * @param  x
            Column of display -- 0 at left to (screen width - 1) at right.
 * @param  y
            Row of display -- 0 at top to (screen height -1) at bottom.
 * @param  color
            Pixel color, one of: SSD1306_BLACK, SSD1306_WHITE or
            SSD1306_INVERSE.
 * @retval None.
 * @note   Changes buffer contents only, no immediate effect on display.
           Follow up with a call to display(), or with other graphics
           commands as needed by one's own application.
*/
void oled_drawPixel(int16_t x, int16_t y, uint16_t color) {
  if ((x >= 0) && (x < width()) && (y >= 0) && (y < height())) {
    // Pixel is in-bounds. Rotate coordinates if needed.
    switch (getRotation()) {
    case 1:
      ssd1306_swap(x, y);
      x = WIDTH - x - 1;
      break;
    case 2:
      x = WIDTH - x - 1;
      y = HEIGHT - y - 1;
      break;
    case 3:
      ssd1306_swap(x, y);
      y = HEIGHT - y - 1;
      break;
    }
    switch (color) {
    case SSD1306_WHITE:
      buffer[x + (y / 8) * WIDTH] |= (1 << (y & 7));
      break;
    case SSD1306_BLACK:
      buffer[x + (y / 8) * WIDTH] &= ~(1 << (y & 7));
      break;
    case SSD1306_INVERSE:
      buffer[x + (y / 8) * WIDTH] ^= (1 << (y & 7));
      break;
    }
  }
}