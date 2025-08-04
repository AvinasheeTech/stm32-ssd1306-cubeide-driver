#include "spi.h"

//pin definitions
//replace with your gpio pins
int8_t mosiPin = 1;   // (Master Out Slave In) - SPI
int8_t clkPin  = 2;    // (Clock Pin) - SPI
int8_t dcPin   = 3;     // (Data Pin) - SPI
int8_t csPin   = 4;     // (Chip Select Pin) - SPI 
int8_t rstPin  = 5;    // Display reset pin assignment

/**  
 * @brief  init function for spi peripheral
 * @param  None
 * @retval None.
 * @note   called in oled_begin function to init peripheral
*/
void spi_init(void){
    printf("spi init.\n");

    //spi initialization code
}

/**  
 * @brief  transfer function for spi peripheral
 * @param  c buffer with transfer data
 * @param  len length of buffer
 * @retval None.
 * @note   called in commandlist and datalist function
*/
void spi_transfer(uint8_t *c,size_t len){
    printf("spi_transfer called. len = %d, buf addr = %p\n", (int)len, c);

    //spi mosi data transfer code
}

/**  
 * @brief  Reset function for spi peripheral
 * @param  level logic level for reset pin
 * @retval None.
 * @note   called in oled_begin function
*/
void spi_reset(bool level){
  //reset pin high or low
}

/**  
 * @brief  Chp Select function for spi peripheral
 * @param  level logic level for cs pin
 * @retval None.
 * @note   called before and after spi transaction
*/
void spi_set_cs(bool level){
  //cs pin high or low
}

/**  
 * @brief  Data/Command function for spi peripheral
 * @param  level logic level for dc pin
 * @retval None.
 * @note   called before sending data or command
*/
void spi_set_dc(bool level){
  //dc pin high or low
}