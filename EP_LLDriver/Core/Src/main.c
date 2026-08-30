/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "private_configuration.h"
#include <string.h>



#define max_size 30



/* USER CODE END Includes */


/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum{
	NONE = 0,
	DO1,
	DO2,
}happens_t;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void init_gpio(void);

/* USER CODE BEGIN PFP */
UARTx_HandleTypeDef uart2 = {0};
TIMx_HandleTypeDef htim2 = {0};
TIMx_OC_InitTypeDef sConfig = {0};
TIMx_HandleTypeDef htim3 = {0};
static uint8_t flag = 0;
static uint8_t TX_flag = 0;
static uint8_t RX_flag = 0;


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
happens_t button1(void){
	if(GPIO_ReadPin_t(GPIOBx, GPIOx_PIN_0) == 0){
		HAL_TIM_DelayMs(&htim3, 50);
		if(GPIO_ReadPin_t(GPIOBx, GPIOx_PIN_0) == 0){
			while(GPIO_ReadPin_t(GPIOBx, GPIOx_PIN_0) == 0);
			GPIO_WritePin_t(GPIOBx, GPIOx_PIN_5, 1);
			HAL_TIM_DelayMs(&htim3, 500);
			GPIO_WritePin_t(GPIOBx, GPIOx_PIN_5, 0);
			return DO1;
		}
	}
	if(GPIO_ReadPin_t(GPIOBx, GPIOx_PIN_1) == 0){
		HAL_TIM_DelayMs(&htim3, 50);
		if(GPIO_ReadPin_t(GPIOBx, GPIOx_PIN_1) == 0){
			while(GPIO_ReadPin_t(GPIOBx, GPIOx_PIN_1) == 0);
			GPIO_WritePin_t(GPIOBx, GPIOx_PIN_6, 1);
			HAL_TIM_DelayMs(&htim3, 500);
			GPIO_WritePin_t(GPIOBx, GPIOx_PIN_6, 0);
			return DO2;
		}
	}
	return NONE;

}

happens_t button(void)
{
    if(GPIO_ReadPin_t(GPIOBx, GPIOx_PIN_0) == 0)
    {
        GPIO_WritePin_t(GPIOBx, GPIOx_PIN_6, 1);
        return DO1;
    }

    if(GPIO_ReadPin_t(GPIOBx, GPIOx_PIN_1) == 0)
    {
        GPIO_WritePin_t(GPIOBx, GPIOx_PIN_5, 1);
        return DO2;
    }

    return NONE;
}
// do1
void behave1(uint16_t *angle){
	servo(&htim2,*angle);
	*angle += 60;
	if(*angle>=180) *angle = 0;
}

//do2
void behave2(void){
	HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_1);
}

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
  init_clock();
  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  init_gpio();

  /* USER CODE BEGIN 2 */
  init_uart(&uart2);
  init_timer_pwm(&htim2,&sConfig);
  init_timer_delay(&htim3);


  HAL_TIM_Base_Start(&htim2);
  HAL_TIM_Base_Start(&htim3);
  HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);

  uint8_t uart_tx1[] = "start\r\n";
  uint8_t uart_tx2[] = "restart...\r\n";
  uint8_t uart_tx3[] = "edone\r\n";
  uint8_t uart_tx4[] = "executing\r\n";
  uint8_t arr_rx[max_size] = {0};
  UARTx_Transmit(&uart2, uart_tx1, sizeof(uart_tx1), 1000);
  uint16_t angle = 40;



  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      /* USER CODE END WHILE */

      /* USER CODE BEGIN 3 */

	  happens_t state_button = button1();

      if (!flag){

    	  //UARTx_Transmit(&uart2, uart_tx4, 11, 1000);
    	  //HAL_TIM_DelayMs(&htim3, 2000);
          if (state_button == DO1){
              //HAL_TIM_DelayMs(&htim3, 200);
              behave1(&angle);

          }
          else if (state_button == DO2){
        	  behave2();
              flag = 1;
          }
      }
      else{
          //UARTx_Transmit(&uart2, uart_tx2, sizeof(uart_tx2), 1000);
         // HAL_TIM_DelayMs(&htim3, 2000);
          //memset(arr_rx, 0, sizeof(arr_rx));

          UARTx_Receive(&uart2, arr_rx, sizeof(arr_rx), 1000);

          if (strncmp((char*)arr_rx, "restart\r\n", 7) == 0){
        	  UARTx_Transmit(&uart2, uart_tx3, sizeof(uart_tx3), 1000);
        	  HAL_TIM_DelayMs(&htim3, 2000);
        	  HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
              flag = 0;
          }
      }
      //HAL_TIM_DelayMs(&htim3, 2000);

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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
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

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
