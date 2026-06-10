/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "rgt.h"
#include "cnt.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#define LED1       PIN_0
#define LED2       PIN_1
#define LED3       PIN_2
#define LED4       PIN_3

#define btn1       PIN_0
#define btn2       PIN_1




/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum{
	MODE_IDLE = 0,
	MODE_1,
	MODE_2,
}app_mode;

static app_mode current_mode = MODE_IDLE;



/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
static void set_led(uint8_t l1, uint8_t l2, uint8_t l3, uint8_t l4);
static void ledoff();
static void runmode1(void);
static void runmode2(void);
static app_mode readbutton(void);
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
static void set_led(uint8_t l1, uint8_t l2, uint8_t l3, uint8_t l4){
	GPIO_write(GPIOA,LED1,l1?1:0);
	GPIO_write(GPIOA,LED2,l2?1:0);
	GPIO_write(GPIOA,LED3,l3?1:0);
	GPIO_write(GPIOA,LED4,l4?1:0);
}

static void ledoff(){
	set_led(0,0,0,0);
}

static void runmode1(void){
	const uint8_t seq[4][4]= {
			{1,0,0,1},
			{0,1,1,0},
			{1,0,1,0},
			{0,1,0,1},
	};
	for(uint8_t i = 0;i<4;i++){
		if(GPIO_read(GPIOB,btn1)==0 ||
				GPIO_read(GPIOB,btn2)==0){
			return;
		}
		set_led(seq[i][0],seq[i][1],seq[i][2],seq[i][3]);
		delayms(TIM2,1000);
	}
}

static void runmode2(void){
	const uint8_t seq[6][4]= {
			{1,1,0,0},
			{0,0,1,1},
			{0,1,1,0},
			{1,0,0,0},
			{1,1,0,0},
			{1,1,1,0},

	};
	for(uint8_t i = 0;i<6;i++){
		if(GPIO_read(GPIOB,btn1)==0 ||
				GPIO_read(GPIOB,btn2)==0){
			return;
		}
		set_led(seq[i][0],seq[i][1],seq[i][2],seq[i][3]);
		delayms(TIM2,1000);
	}
}

static app_mode readbutton(void){
	if(GPIO_read(GPIOB,btn1)==0){
		delayms(TIM2,50);
		if(GPIO_read(GPIOB,btn1)==0){
			while(GPIO_read(GPIOB,btn1)==0);
			return MODE_1;

		}
	}
	if(GPIO_read(GPIOB,btn2)==0){
		delayms(TIM2,50);
		if(GPIO_read(GPIOB,btn2)==0){
			while(GPIO_read(GPIOB,btn2)==0);
			return MODE_2;

		}
	}
	return current_mode;
}

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  TIMx_init(TIM2);

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */
  GPIO_init(GPIOA,LED1,OUTPUT_MODE_10MHZ,PUSH_PULL,NO_PULL);
  GPIO_init(GPIOA,LED2,OUTPUT_MODE_10MHZ,PUSH_PULL,NO_PULL);
  GPIO_init(GPIOA,LED3,OUTPUT_MODE_10MHZ,PUSH_PULL,NO_PULL);
  GPIO_init(GPIOA,LED4,OUTPUT_MODE_10MHZ,PUSH_PULL,NO_PULL);
  GPIO_init(GPIOB,btn1,INPUT_MODE,PULL_UP_DOWN,PULL_UP);
  GPIO_init(GPIOB,btn2,INPUT_MODE,PULL_UP_DOWN,PULL_UP);
  ledoff();



  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  current_mode = readbutton();

	  switch(current_mode){
	  case MODE_1: runmode1(); break;
	  case MODE_2: runmode2(); break;
	  default:     ledoff(); break;
	  }
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

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
#ifdef USE_FULL_ASSERT
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
