/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  *
  * Chức năng:
  *   - Button_A (PA3): LED chạy 1 điểm sáng từ PHẢI sang TRÁI (LED4→LED3→LED2→LED1)
  *   - Button_B (PA4): Dừng LED
  *   - Button_C (PA5): LED chạy 1 điểm sáng từ TRÁI sang PHẢI (LED1→LED2→LED3→LED4)
  *   - Delay chuyển LED: 500ms
  *
  * Chân LED  : PB11=LED1, PB12=LED2, PB13=LED3, PB14=LED4
  * Chân Button: PA3=Button_A, PA4=Button_B, PA5=Button_C
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define NUM_LEDS  4
/* USER CODE END PD */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum {
  STATE_STOP        = 0,
  STATE_RIGHT2LEFT  = 1,   /* Button_A: LED chạy phải → trái */
  STATE_LEFT2RIGHT  = 2    /* Button_C: LED chạy trái → phải */
} RunState;
/* USER CODE END PTD */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
static RunState  g_state     = STATE_STOP;
static uint8_t   g_led_idx   = 0;
static int8_t    g_direction = 1;
static uint32_t  g_last_tick = 0;

/* Mảng Port và Pin của 4 LED theo thứ tự: LED1(trái) → LED4(phải) */
static GPIO_TypeDef* const LED_PORT[NUM_LEDS] = {
  LED1_GPIO_Port,
  LED2_GPIO_Port,
  LED3_GPIO_Port,
  LED4_GPIO_Port
};

static const uint16_t LED_PIN[NUM_LEDS] = {
  LED1_Pin,
  LED2_Pin,
  LED3_Pin,
  LED4_Pin
};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);

/* USER CODE BEGIN PFP */
static void     LED_AllOff(void);
static void     LED_ShowOnly(uint8_t idx);
static uint8_t  Button_A_Pressed(void);
static uint8_t  Button_B_Pressed(void);
static uint8_t  Button_C_Pressed(void);
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* Tắt toàn bộ LED */
static void LED_AllOff(void)
{
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    HAL_GPIO_WritePin(LED_PORT[i], LED_PIN[i], GPIO_PIN_RESET);
  }
}

/* Chỉ bật LED tại vị trí idx, tắt các LED còn lại */
static void LED_ShowOnly(uint8_t idx)
{
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    HAL_GPIO_WritePin(LED_PORT[i], LED_PIN[i],
                      (i == idx) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  }
}

/*
 * Tất cả button dùng PULLUP → nhấn = LOW (GPIO_PIN_RESET)
 */
static uint8_t Button_A_Pressed(void)
{
  return (HAL_GPIO_ReadPin(Button_A_GPIO_Port, Button_A_Pin) == GPIO_PIN_RESET);
}

static uint8_t Button_B_Pressed(void)
{
  return (HAL_GPIO_ReadPin(Button_B_GPIO_Port, Button_B_Pin) == GPIO_PIN_RESET);
}

static uint8_t Button_C_Pressed(void)
{
  return (HAL_GPIO_ReadPin(Button_C_GPIO_Port, Button_C_Pin) == GPIO_PIN_RESET);
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

  HAL_Init();

  /* USER CODE BEGIN Init */
  /* USER CODE END Init */

  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */

  MX_GPIO_Init();

  /* USER CODE BEGIN 2 */
  LED_AllOff();
  g_last_tick = HAL_GetTick();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    /* USER CODE BEGIN 3 */

    /* -------- 1. Đọc nút nhấn và cập nhật trạng thái -------- */

    /* Button_A: LED chạy PHẢI → TRÁI (index từ 3 xuống 0) */
    if (Button_A_Pressed()) {
      HAL_Delay(20); /* debounce */
      if (Button_A_Pressed()) {
        if (g_state != STATE_RIGHT2LEFT) {
          g_state     = STATE_RIGHT2LEFT;
          g_direction = -1;           /* giảm index: phải → trái */
          g_led_idx   = NUM_LEDS - 1; /* bắt đầu từ LED4 (ngoài cùng phải) */
          g_last_tick = HAL_GetTick();
          LED_ShowOnly(g_led_idx);
        }
        while (Button_A_Pressed()) {} /* chờ nhả nút */
      }
    }

    /* Button_B: Dừng tất cả LED */
    else if (Button_B_Pressed()) {
      HAL_Delay(20);
      if (Button_B_Pressed()) {
        if (g_state != STATE_STOP) {
          g_state = STATE_STOP;
          LED_AllOff();
        }
        while (Button_B_Pressed()) {}
      }
    }

    /* Button_C: LED chạy TRÁI → PHẢI (index từ 0 lên 3) */
    else if (Button_C_Pressed()) {
      HAL_Delay(20);
      if (Button_C_Pressed()) {
        if (g_state != STATE_LEFT2RIGHT) {
          g_state     = STATE_LEFT2RIGHT;
          g_direction = 1;  /* tăng index: trái → phải */
          g_led_idx   = 0;  /* bắt đầu từ LED1 (ngoài cùng trái) */
          g_last_tick = HAL_GetTick();
          LED_ShowOnly(g_led_idx);
        }
        while (Button_C_Pressed()) {}
      }
    }

    /* -------- 2. Cập nhật LED theo thời gian (500ms) -------- */
    if (g_state != STATE_STOP) {
      if ((HAL_GetTick() - g_last_tick) >= 500U) {
        g_last_tick = HAL_GetTick();

        /* Chuyển LED tiếp theo, quay vòng */
        g_led_idx = (uint8_t)((g_led_idx + NUM_LEDS + g_direction) % NUM_LEDS);
        LED_ShowOnly(g_led_idx);
      }
    }

  /* USER CODE END 3 */
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState            = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                   | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
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

  /* Mức mặc định cho LED: tắt */
  HAL_GPIO_WritePin(GPIOB, LED1_Pin | LED2_Pin | LED3_Pin | LED4_Pin, GPIO_PIN_RESET);

  /*
   * Button_A (PA3), Button_B (PA4), Button_C (PA5)
   * Tất cả dùng PULLUP → nhấn = LOW
   */
  GPIO_InitStruct.Pin  = Button_A_Pin | Button_B_Pin | Button_C_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*
   * LED1=PB11, LED2=PB12, LED3=PB13, LED4=PB14
   * Output push-pull
   */
  GPIO_InitStruct.Pin   = LED1_Pin | LED2_Pin | LED3_Pin | LED4_Pin;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
/* USER CODE END 4 */

/**
  * @brief  Error Handler
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  __disable_irq();
  while (1) {}
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
