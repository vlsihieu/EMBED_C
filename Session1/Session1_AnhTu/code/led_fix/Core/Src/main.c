/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "GPIO.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
// Định nghĩa các trạng thái
#define MODE_STOP           0
#define MODE_RIGHT_TO_LEFT  1
#define MODE_LEFT_TO_RIGHT  2
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

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */
  // 1. Khởi tạo 4 LED tại PB11, PB12, PB13, PB14 bằng thư viện của Vinh
  GPIOx_Init(GPIOB, 11, OUT_PUSH_PULL, NO_PULL, OUTPUT_SPEED_2MHZ);
  GPIOx_Init(GPIOB, 12, OUT_PUSH_PULL, NO_PULL, OUTPUT_SPEED_2MHZ);
  GPIOx_Init(GPIOB, 13, OUT_PUSH_PULL, NO_PULL, OUTPUT_SPEED_2MHZ);
  GPIOx_Init(GPIOB, 14, OUT_PUSH_PULL, NO_PULL, OUTPUT_SPEED_2MHZ);

  // 2. Khởi tạo 3 Button tại PA0 (A), PA1 (B), PA2 (C) - Chế độ Pull-up
  GPIOx_Init(GPIOA, 0, INPUT_PULL_UP_DOW, PULL_UP, INPUT_MODE);
  GPIOx_Init(GPIOA, 1, INPUT_PULL_UP_DOW, PULL_UP, INPUT_MODE);
  GPIOx_Init(GPIOA, 2, INPUT_PULL_UP_DOW, PULL_UP, INPUT_MODE);

  uint8_t current_mode = MODE_STOP;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    // Đọc trạng thái nút nhấn (Logic 0 là nhấn)
    if (GPIOx_Read(GPIOA, 0) == 0) current_mode = MODE_RIGHT_TO_LEFT;
    if (GPIOx_Read(GPIOA, 1) == 0) current_mode = MODE_STOP;
        if (GPIOx_Read(GPIOA, 2) == 0) current_mode = MODE_LEFT_TO_RIGHT;

        // Hiệu ứng dịch điểm sáng
        if (current_mode == MODE_RIGHT_TO_LEFT)
        {
          for (int i = 14; i >= 11 && current_mode == MODE_RIGHT_TO_LEFT; i--)
          {
            // Tắt hết 4 LED
            for(int j = 11; j <= 14; j++) GPIOx_Write(GPIOB, j, 0);
            // Bật LED hiện tại
            GPIOx_Write(GPIOB, i, 1);
            HAL_Delay(200);

            // Check nút bấm liên tục để thoát loop ngay lập tức
            if (GPIOx_Read(GPIOA, 1) == 0) current_mode = MODE_STOP;
            if (GPIOx_Read(GPIOA, 2) == 0) current_mode = MODE_LEFT_TO_RIGHT;
          }
        }
        else if (current_mode == MODE_LEFT_TO_RIGHT)
        {
          for (int i = 11; i <= 14 && current_mode == MODE_LEFT_TO_RIGHT; i++)
          {
            for(int j = 11; j <= 14; j++) GPIOx_Write(GPIOB, j, 0);
            GPIOx_Write(GPIOB, i, 1);
            HAL_Delay(200);

            if (GPIOx_Read(GPIOA, 1) == 0) current_mode = MODE_STOP;
            if (GPIOx_Read(GPIOA, 0) == 0) current_mode = MODE_RIGHT_TO_LEFT;
          }
        }
        else
        {
          // MODE_STOP: Tắt hết LED
          for(int j = 11; j <= 14; j++) GPIOx_Write(GPIOB, j, 0);
        }
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
      GPIO_InitTypeDef GPIO_InitStruct = {0};
      /* USER CODE BEGIN MX_GPIO_Init_1 */

      /* USER CODE END MX_GPIO_Init_1 */

      /* GPIO Ports Clock Enable */
      __HAL_RCC_GPIOD_CLK_ENABLE();
      __HAL_RCC_GPIOA_CLK_ENABLE();
      __HAL_RCC_GPIOB_CLK_ENABLE();
      /*Configure GPIO pin Output Level */
        HAL_GPIO_WritePin(GPIOB, LED1_Pin|LED2_Pin|LED3_Pin|LED4_Pin, GPIO_PIN_RESET);

        /*Configure GPIO pins : BTA_Pin BTB_Pin BTC_Pin */
        GPIO_InitStruct.Pin = BTA_Pin|BTB_Pin|BTC_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /*Configure GPIO pins : LED1_Pin LED2_Pin LED3_Pin LED4_Pin */
        GPIO_InitStruct.Pin = LED1_Pin|LED2_Pin|LED3_Pin|LED4_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

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
