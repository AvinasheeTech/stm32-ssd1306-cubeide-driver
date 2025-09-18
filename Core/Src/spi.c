/* @file  spi.c
   @brief source file for ssd1306 display based spi functions.
   @author Shyam Jha (Avinashee Tech)
*/

#include "spi.h"

#include "main.h"  //Main header :CubeIDE platform

//pin definitions :STM32F411
//replace with your gpio pins
int8_t mosiPin = SPI_MOSI_Pin;   // (Master Out Slave In) - SPI
int8_t clkPin  = SPI_CLK_PIN;    // (Clock Pin) - SPI
int8_t dcPin   = SPI_DC_Pin;     // (Data Pin) - SPI
int8_t csPin   = SPI_CS_Pin;     // (Chip Select Pin) - SPI
int8_t rstPin  = SPI_RESET_Pin;  // Display reset pin assignment

extern SPI_HandleTypeDef hspi1;  //SPI Handle :CubeIDE platform

/**  
 * @brief  init function for spi peripheral
 * @param  None
 * @retval None.
 * @note   called in oled_begin function to init peripheral
 *         updated according to CubeIDE platform
*/
void spi_init(void){
    printf("spi init.\n");

    //spi initialization code
    hspi1.Instance = SPI1;
	hspi1.Init.Mode = SPI_MODE_MASTER;
	hspi1.Init.Direction = SPI_DIRECTION_2LINES;
	hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
	hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
	if (HAL_SPI_Init(&hspi1) != HAL_OK)
	{
	  Error_Handler();
	}

}

/**  
 * @brief  transfer function for spi peripheral
 * @param  c buffer with transfer data
 * @param  len length of buffer
 * @retval None.
 * @note   called in commandlist and datalist function
 *         updated according to CubeIDE platform
*/
void spi_transfer(uint8_t *c,size_t len){
    printf("spi_transfer called. len = %d, buf addr = %p\n", (int)len, c);

    //spi mosi data transfer code
    HAL_SPI_Transmit(&hspi1, c, len, 100);
}

/**  
 * @brief  Reset function for spi peripheral
 * @param  level logic level for reset pin
 * @retval None.
 * @note   called in oled_begin function
 *         updated according to CubeIDE platform
*/
void spi_reset(bool level){
  //reset pin high or low
  HAL_GPIO_WritePin(SPI_RESET_GPIO_Port, rstPin, level);
}

/**  
 * @brief  Chp Select function for spi peripheral
 * @param  level logic level for cs pin
 * @retval None.
 * @note   called before and after spi transaction
 *         updated according to CubeIDE platform
*/
void spi_set_cs(bool level){
  //cs pin high or low
  HAL_GPIO_WritePin(SPI_CS_GPIO_Port, csPin, level);
}

/**  
 * @brief  Data/Command function for spi peripheral
 * @param  level logic level for dc pin
 * @retval None.
 * @note   called before sending data or command
 *         updated according to CubeIDE platform
*/
void spi_set_dc(bool level){
  //dc pin high or low
  HAL_GPIO_WritePin(SPI_DC_GPIO_Port, dcPin, level);
}
