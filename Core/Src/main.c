/* @file  main.c
   @brief this code involves STM32 and SSD1306 oled display implementation
          with lvgl support.
   @author Shyam Jha (Avinashee Tech)
*/


/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ssd1306.h"
#include "lvgl.h"
#include "ui.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define LV_TICK_PERIOD_MS 1
#define DISP_BUF_LINES 8   // e.g., 8 lines at a time
#define BYTES_PER_PIXEL (LV_COLOR_FORMAT_GET_SIZE(LV_COLOR_FORMAT_I1))
#define DISP_BUF_SIZE (SCREEN_WIDTH * DISP_BUF_LINES)
#define HW_COLUMNS (128 / 8)
#define LV_TICK_PERIOD_MS 1

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim2;

osThreadId guiHandle;
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM2_Init(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

extern const lv_img_dsc_t moustacheman_bmp;
extern const lv_img_dsc_t hatwomen_bmp;
extern const lv_img_dsc_t building_bmp;
extern const lv_img_dsc_t animal_bmp;
extern const lv_img_dsc_t flower_bmp;

//tick count variable
volatile int tick = 0;

static int hour = 7;
static int minute = 30;
static int second = 0;


/*function declarations*/
void guiTask(void const * pvParameter);
void oled_flush_cb(struct _lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p);
void oled_set_px_cb(struct _lv_disp_drv_t * disp_drv, uint8_t * buf, lv_coord_t buf_w, lv_coord_t x, lv_coord_t y, lv_color_t color, lv_opa_t opa);
void oled_rounder_cb(struct _lv_disp_drv_t * disp_drv, lv_area_t *a);
void flush_one_row(unsigned int address, uint8_t *buf, int linelen);
void lvgl_display_handler(int ms);
static void lv_tick_task(void);
void update_clock(lv_timer_t * timer);
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

  HAL_TIM_Base_Start(&htim2);  //for delay in microseconds

  //initialize display driver
  if(!oled_begin(SCREEN_WIDTH,SCREEN_HEIGHT,SSD1306_SWITCHCAPVCC,true)) { // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
	  printf("SSD1306 allocation failed");
  }

  // Clear the buffer
  oled_clearDisplay();



  /* USER CODE END 2 */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of gui */
  osThreadDef(gui, guiTask, osPriorityNormal, 0, 2000);
  guiHandle = osThreadCreate(osThread(gui), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 25-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 4294967295;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, SPI_RESET_Pin|SPI_DC_Pin|SPI_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : SPI_RESET_Pin SPI_DC_Pin SPI_CS_Pin */
  GPIO_InitStruct.Pin = SPI_RESET_Pin|SPI_DC_Pin|SPI_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/**
 * @brief  task for GUI operation
 * @param  argument default task parameters
 * @retval None
 * @note   takes care of LVGL initialization and Displays Text
 *         and Images chosen. (UI also)
 */
void guiTask(void const * pvParameter)
{
	//initialize LVGL
	lv_init();

	//create buffer for display data
	printf("buf size-%d\n",DISP_BUF_SIZE * sizeof(lv_color_t));
	lv_color_t* buf1 = malloc(DISP_BUF_SIZE * sizeof(lv_color_t));
	assert(buf1 != NULL);
	lv_color_t *buf2 = NULL;


	static lv_disp_draw_buf_t draw_buf;
	lv_disp_draw_buf_init(&draw_buf, buf1, buf2, DISP_BUF_SIZE);

	lv_disp_drv_t disp_drv;
	lv_disp_drv_init(&disp_drv);
	disp_drv.hor_res = SCREEN_WIDTH;
	disp_drv.ver_res = SCREEN_HEIGHT;
	disp_drv.draw_buf = &draw_buf;
	disp_drv.flush_cb = oled_flush_cb;
	disp_drv.set_px_cb = oled_set_px_cb;
	disp_drv.rounder_cb = oled_rounder_cb;

	lv_disp_t * disp;
	disp = lv_disp_drv_register(&disp_drv);

    /*ui init*/
	ui_init();
	lv_timer_create(update_clock,1000,NULL); //1s period timer

   /* Infinite loop */
    for(;;)
    {
        /*update clock variables*/
    	static char clock_buffer[4] = {0};

		memset(clock_buffer,0,sizeof(clock_buffer));
		sprintf(clock_buffer,"%02d",hour);
		lv_label_set_text(ui_Label1,clock_buffer);

		memset(clock_buffer,0,sizeof(clock_buffer));
		sprintf(clock_buffer,"%02d",minute);
		lv_label_set_text(ui_Label2,clock_buffer);

		memset(clock_buffer,0,sizeof(clock_buffer));
		sprintf(clock_buffer,"%02d",second);
		lv_label_set_text(ui_Label3,clock_buffer);

    	lv_task_handler();
    }

    /* A task should NEVER return */
	free(buf1);
}

/**
 * @brief  function to update UI periodically
 * @param  timer timer descriptor
 * @retval None
 * @note   updates hour, minute and seconds value from the default set variable
 *         defined on the top.
 */
void update_clock(lv_timer_t * timer){
    second++;
    if(second>=60){
        minute++;
        second=0;
    }
    if(minute>=60){
        hour++;
        minute=0;
    }
    if(hour>=24){
        hour=0;
    }



}
/**
 * @brief  helper function to handle lvgl task in background
 * @param  ms millisecond to wait
 * @retval None
 */
void lvgl_display_handler(int ms){
    int current_tick = tick;
    while(abs(tick-current_tick)<ms){
        lv_task_handler();
    }
}

/**
 * @brief  lvgl flush callback for ssd1306 oled
 * @param  disp_drv display driver pointer
 * @param  area display area to update
 * @param  color_p pixel data to be updated
 * @retval None
 * @note   this callback function is called after set_px_cb has prepared 1 bit/pixel
 *         buffer data.
 */
void oled_flush_cb(struct _lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
    int row = area->y1, col;
    uint8_t *buffer;
    int linelen = (area->x2 - area->x1)/8;
    buffer = (uint8_t*) color_p;

    for (row = area->y1; row <= area->y2; row++) {
        unsigned int address = (unsigned int)row * HW_COLUMNS + area->x1/8;
        flush_one_row(address, buffer, linelen);
        buffer += linelen+1;
    }


    oled_display();
    lv_disp_flush_ready(disp_drv);
}

/**
 * @brief  lvgl set pixel callback for ssd1306 oled
 * @param  disp_drv display driver pointer
 * @param  buf  display data buffer
 * @param  buf_w display width
 * @param  x display x co-ordinates
 * @param  y display y co-ordinates
 * @param  color 8bit color value
 * @param  opa opacity value
 * @retval None
 * @note   converts 8 bit/pixel color value into 1 bit/pixel based on
 *         display buffer value.
 */
void oled_set_px_cb(struct _lv_disp_drv_t * disp_drv, uint8_t * buf, lv_coord_t buf_w, lv_coord_t x, lv_coord_t y, lv_color_t color, lv_opa_t opa)
{
    buf += buf_w/8 * y;
    buf += x/8;
    if(lv_color_brightness(color) > 128) {(*buf) |= (1 << (7 - x % 8));}
    else {(*buf) &= ~(1 << (7 - x % 8));}

}

/**
 * @brief  rounds or adjust the dimension of display data buffer
 * @param  disp_drv display driver pointer
 * @param  a display area
 * @retval None
 * @note   useful in adjusting data buffer boundary for proper update.
 */
void oled_rounder_cb(struct _lv_disp_drv_t * disp_drv, lv_area_t *a)
{
    a->x1 = a->x1 & ~(0x7);  //Round Down
    a->x2 = a->x2 |  (0x7);  //Round Up
}

/**
 * @brief  function to update display pixel data
 * @param  address display data buffer index address
 * @param  buf display data buffer
 * @param  linelen number of bytes to update
 * @retval None
 * @note   this function is called from within flush callback, and
 *         then writes spi buffer for respective display pixels.
 */
void flush_one_row(unsigned int address, uint8_t *buf, int linelen)
{
    for (int byte_idx = 0; byte_idx < linelen; byte_idx++) {
        uint8_t data = buf[byte_idx];
        for (int bit = 0; bit < 8; bit++) {
            int x = (address % HW_COLUMNS) * 8 + bit;
            int y = address / HW_COLUMNS;
            uint16_t ssd_color = (data & 0x80) ? 1 : 0;
            data = data<<1;
            oled_drawPixel(x, y, ssd_color);
        }
        address++;
    }
}

/**
 * @brief  tick interrupt ISR
 * @param  arg argument
 * @retval None
 * @note   periodically updates tick value and calls lv_tick_inc
 */
static void lv_tick_task(void) {
    tick++;
    lv_tick_inc(LV_TICK_PERIOD_MS);
}

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1)
  {
    HAL_IncTick();
    lv_tick_task();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
