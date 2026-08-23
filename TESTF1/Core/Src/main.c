/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body - Test HAL Clock & HAL GPIO Drivers
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
#include "hal_gpio.h"
#include "hal_timer.h"
#include "hal_dma.h"
#include "hal_clock.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Biến đếm mili-giây từ SysTick */
static volatile uint32_t ms_ticks = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */
static void delay_ms(uint32_t ms);
static void Test_GPIO_Output_Init(void);
static void Test_GPIO_Input_Init(void);
static void Test_GPIO_EXTI_Init(void);
static void Test_GPIO_Lock(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* Hàm tạo trễ mili-giây dùng SysTick */
static void delay_ms(uint32_t ms)
{
    uint32_t start = ms_ticks;
    while ((ms_ticks - start) < ms);
}

/**
 * @brief TEST GPIO 1: Khởi tạo PB2 làm Output Push-Pull 2MHz
 */
static void Test_GPIO_Output_Init(void)
{
    GPIOx_InitTypeDef_t gpio_init = {0};

    /* Bật clock cho GPIOB */
    HAL_CLOCK_APB2_Peripheral_Enable(RCC_APB2ENR_IOPBEN);

    /* Cấu hình chân PB2 làm Output Push-Pull 2MHz */
    gpio_init.Pin   = GPIOx_PIN_2;
    gpio_init.Mode  = GPIOx_MODE_OUTPUT_PP;
    gpio_init.Speed = GPIOx_SPEED_FREQ_LOW;
    gpio_init.Pull  = GPIOx_NOPULL;
    GPIO_Init_t(GPIOBx, &gpio_init);
}

/**
 * @brief TEST GPIO 2: Khởi tạo PA0 làm Input Pull-up
 */
static void Test_GPIO_Input_Init(void)
{
    GPIOx_InitTypeDef_t gpio_init = {0};

    /* Bật clock cho GPIOA */
    HAL_CLOCK_APB2_Peripheral_Enable(RCC_APB2ENR_IOPAEN);

    /* Cấu hình PA0 làm Input Pull-Up */
    gpio_init.Pin   = GPIOx_PIN_0;
    gpio_init.Mode  = GPIOx_MODE_INPUT;
    gpio_init.Pull  = GPIOx_PULLUP;
    GPIO_Init_t(GPIOAx, &gpio_init);
}

/**
 * @brief TEST GPIO 3: Khóa cấu hình chân PB2 (GPIO Lock Key)
 */
static void Test_GPIO_Lock(void)
{
    GPIOx_StatusTypeDef_t lock_status;

    /* Khóa cấu hình chân PB2 */
    lock_status = GPIO_LockPin_t(GPIOBx, GPIOx_PIN_2);

    if (lock_status == GPIOx_StatusTypeDef_OK)
    {
        /* Thử ghi đè cấu hình để kiểm tra khóa: thanh ghi CRL của PB2 vẫn giữ nguyên */
        GPIOx_InitTypeDef_t test_reconfig = {0};
        test_reconfig.Pin  = GPIOx_PIN_2;
        test_reconfig.Mode = GPIOx_MODE_ANALOG;
        GPIO_Init_t(GPIOBx, &test_reconfig);
    }
}

/**
 * @brief TEST GPIO 4: Cấu hình ngắt ngoài EXTI0 trên chân PA0 (Falling Edge)
 */
static void Test_GPIO_EXTI_Init(void)
{
    GPIOx_InitTypeDef_t exti_init = {0};

    HAL_CLOCK_APB2_Peripheral_Enable(RCC_APB2ENR_IOPAEN);

    /* Cấu hình PA0 nhận ngắt sườn xuống kèm Pull-Up */
    exti_init.Pin  = GPIOx_PIN_0;
    exti_init.Mode = GPIOx_MODE_IT_FALLING;
    exti_init.Pull = GPIOx_PULLUP;
    GPIO_Init_t(GPIOAx, &exti_init);

    /* Kích hoạt ngắt NVIC cho EXTI0 */
    NVIC_SetPriority(EXTI0_IRQn, 2);
    NVIC_EnableIRQ(EXTI0_IRQn);
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  HALx_StatusTypeDef status;
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
  /* USER CODE BEGIN 2 */

  /* =========================================================================
   * PHẦN 1: KIỂM TRA DRIVER CLOCK (hal_clock.c)
   * ========================================================================= */
  /* Test Reset Clock */
  HAL_CLOCK_DeInit();

  /* Test Clock 72 MHz (HSE + PLL) */
  status = HAL_CLOCK_ConfigHSE();
  if (status != STD_OK)
  {
      while (1);
  }

  /* Test bật / tắt xung Clock ngoại vi */
  HAL_CLOCK_AHB_Peripheral_Enable(RCC_AHBENR_DMA1EN);
  HAL_CLOCK_AHB_Peripheral_Disable(RCC_AHBENR_DMA1EN);

  HAL_CLOCK_APB1_Peripheral_Enable(RCC_APB1ENR_TIM2EN);
  HAL_CLOCK_APB1_Peripheral_Disable(RCC_APB1ENR_TIM2EN);

  /* =========================================================================
   * PHẦN 2: KIỂM TRA DRIVER GPIO (hal_gpio.c)
   * ========================================================================= */
  /* 1. Test Khởi tạo chân PB2 Output */
  Test_GPIO_Output_Init();

  /* 2. Test hàm Ghi chân (WritePin) */
  GPIO_WritePin_t(GPIOBx, GPIOx_PIN_2, GPIOx_PinState_HIGH);
  delay_ms(300);
  GPIO_WritePin_t(GPIOBx, GPIOx_PIN_2, GPIOx_PinState_LOW);
  delay_ms(300);

  /* 3. Test hàm Đảo trạng thái (Toggle) */
  for (int i = 0; i < 6; i++)
  {
      GPIO_Toggle_t(GPIOBx, GPIOx_PIN_2);
      delay_ms(150);
  }

  /* 4. Test hàm Lock chân */
  Test_GPIO_Lock();

  /* 5. Test Khởi tạo chân PA0 làm Input & EXTI0 */
  Test_GPIO_Input_Init();
  Test_GPIO_EXTI_Init();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* =========================================================================
     * Vòng lặp chính: Test đọc chân PA0 (Polling)
     * Nếu nhấn nút nối PA0 xuống GND (mức LOW) -> Bật LED PB2
     * Thả nút (mức HIGH) -> Tắt LED PB2
     * ========================================================================= */
    if (GPIO_ReadPin_t(GPIOAx, GPIOx_PIN_0) == GPIOx_PinState_LOW)
    {
        GPIO_WritePin_t(GPIOBx, GPIOx_PIN_2, GPIOx_PinState_HIGH);
    }
    else
    {
        GPIO_WritePin_t(GPIOBx, GPIOx_PIN_2, GPIOx_PinState_LOW);
    }

    delay_ms(50);
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
  __HAL_RCC_GPIOA_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */
/* Callback tăng ms_ticks cho SysTick */
void HAL_IncTick(void)
{
    ms_ticks++;
}

/* Hàm phục vụ ngắt EXTI0 */
void EXTI0_IRQHandler(void)
{
    GPIO_EXTI_IRQHandler_t(GPIOx_PIN_0);
}

/* Định nghĩa lại hàm Callback của GPIO EXTI khi có ngắt sườn xuống trên PA0 */
void GPIO_EXTI_Callback_t(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIOx_PIN_0)
    {
        /* Đảo trạng thái LED PB2 khi có ngắt nút bấm */
        GPIO_Toggle_t(GPIOBx, GPIOx_PIN_2);
    }
}
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
