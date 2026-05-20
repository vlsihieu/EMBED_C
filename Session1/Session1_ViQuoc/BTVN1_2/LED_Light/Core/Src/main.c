/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  */
/* USER CODE END Header */

#include "main.h"

/* USER CODE BEGIN Includes */
#include "GPIO_x.h"
/* USER CODE END Includes */

/* USER CODE BEGIN PV */
typedef enum {
    DIR_STOP  = 0,
    DIR_LEFT,     // Button A: phải -> trái  (PB14 -> PB13 -> PB12 -> PB11)
    DIR_RIGHT     // Button C: trái -> phải  (PB11 -> PB12 -> PB13 -> PB14)
} Direction_t;

/* Mảng LED theo thứ tự từ TRÁI sang PHẢI
 * index 0 = PB11 (D1, trái nhất)
 * index 3 = PB14 (D4, phải nhất)  */
static const uint16_t LED_PINS[4] = {
    GPIOx_Pin_11,  // index 0 - trái
    GPIOx_Pin_12,  // index 1
    GPIOx_Pin_13,  // index 2
    GPIOx_Pin_14   // index 3 - phải
};

static Direction_t  g_dir      = DIR_STOP;
static int8_t       g_ledIdx   = 0;
/* USER CODE END PV */

void SystemClock_Config(void);
static void MX_GPIO_Init(void);

/* USER CODE BEGIN PFP */
static void LED_AllOff(void);
static void LED_ShowIndex(int8_t idx);
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
static void LED_AllOff(void)
{
    GPIOx_Write(GPIOB, GPIOx_Pin_11, 0);
    GPIOx_Write(GPIOB, GPIOx_Pin_12, 0);
    GPIOx_Write(GPIOB, GPIOx_Pin_13, 0);
    GPIOx_Write(GPIOB, GPIOx_Pin_14, 0);
}

static void LED_ShowIndex(int8_t idx)
{
    LED_AllOff();
    GPIOx_Write(GPIOB, LED_PINS[idx], 1);
}
/* USER CODE END 0 */

int main(void)
{
    /* USER CODE BEGIN 1 */
    /* USER CODE END 1 */

    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();

    /* USER CODE BEGIN 2 */

    /* --- Khởi tạo 4 LED (PB11-PB14): Output Push-Pull --- */
    GPIOx_Init(GPIOB, GPIOx_Pin_11, OUT_PUSH_PULL, NO_PULL, OUTPUT_SPEED_2MHZ);
    GPIOx_Init(GPIOB, GPIOx_Pin_12, OUT_PUSH_PULL, NO_PULL, OUTPUT_SPEED_2MHZ);
    GPIOx_Init(GPIOB, GPIOx_Pin_13, OUT_PUSH_PULL, NO_PULL, OUTPUT_SPEED_2MHZ);
    GPIOx_Init(GPIOB, GPIOx_Pin_14, OUT_PUSH_PULL, NO_PULL, OUTPUT_SPEED_2MHZ);

    /* --- Khởi tạo 3 Button (PA3, PA4, PA5): Input Pull-Up --- */
    GPIOx_Init(GPIOA, GPIOx_Pin_3, INPUT_PULL_UP_DOW, PULL_UP, INPUT_MODE);
    GPIOx_Init(GPIOA, GPIOx_Pin_4, INPUT_PULL_UP_DOW, PULL_UP, INPUT_MODE);
    GPIOx_Init(GPIOA, GPIOx_Pin_5, INPUT_PULL_UP_DOW, PULL_UP, INPUT_MODE);

    LED_AllOff();

    /* Lưu trạng thái trước để detect cạnh nhả (released edge) */
    uint8_t prevA = 1, prevB = 1, prevC = 1;
    uint8_t currA,     currB,     currC;

    /* Debounce */
    uint32_t lastEventTick = 0;
    const uint32_t DEBOUNCE_MS = 50;

    /* Tốc độ dịch LED */
    uint32_t lastStepTick  = 0;
    const uint32_t STEP_MS = 300;

    /* USER CODE END 2 */

    /* USER CODE BEGIN WHILE */
    while (1)
    {
        /* ===== 1. ĐỌC BUTTON & DETECT CẠNH NHẢ ===== */
        currA = GPIOx_Read(GPIOA, GPIOx_Pin_3);  // 0 = đang nhấn, 1 = nhả
        currB = GPIOx_Read(GPIOA, GPIOx_Pin_4);
        currC = GPIOx_Read(GPIOA, GPIOx_Pin_5);

        uint32_t now = HAL_GetTick();

        if ((now - lastEventTick) >= DEBOUNCE_MS)
        {
            /* Button A nhả -> chạy phải sang trái, bắt đầu từ PB14 */
            if (prevA == 0 && currA == 1)
            {
                g_dir    = DIR_LEFT;
                g_ledIdx = 3;           // Bắt đầu từ index 3 = PB14
                LED_ShowIndex(g_ledIdx);
                lastStepTick  = now;
                lastEventTick = now;
            }

            /* Button B nhả -> dừng, tắt hết LED */
            if (prevB == 0 && currB == 1)
            {
                g_dir = DIR_STOP;
                LED_AllOff();
                lastEventTick = now;
            }

            /* Button C nhả -> chạy trái sang phải, bắt đầu từ PB11 */
            if (prevC == 0 && currC == 1)
            {
                g_dir    = DIR_RIGHT;
                g_ledIdx = 0;           // Bắt đầu từ index 0 = PB11
                LED_ShowIndex(g_ledIdx);
                lastStepTick  = now;
                lastEventTick = now;
            }
        }

        /* Cập nhật trạng thái trước */
        prevA = currA;
        prevB = currB;
        prevC = currC;

        /* ===== 2. CẬP NHẬT LED THEO THỜI GIAN ===== */
        if (g_dir != DIR_STOP && (now - lastStepTick) >= STEP_MS)
        {
            lastStepTick = now;

            if (g_dir == DIR_LEFT)
            {
                g_ledIdx--;
                if (g_ledIdx < 0) g_ledIdx = 3;  // Quay vòng: PB11 -> PB14
            }
            else // DIR_RIGHT
            {
                g_ledIdx++;
                if (g_ledIdx > 3) g_ledIdx = 0;  // Quay vòng: PB14 -> PB11
            }

            LED_ShowIndex(g_ledIdx);
        }

        /* USER CODE END WHILE */
        /* USER CODE BEGIN 3 */
    }
    /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState            = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_NONE;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                     | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_HSI;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
  * @brief GPIO Initialization Function (HAL - chỉ bật clock GPIOD)
  */
static void MX_GPIO_Init(void)
{
    /* GPIO Ports Clock Enable - GPIOD cần cho HSE/OSC */
    __HAL_RCC_GPIOD_CLK_ENABLE();
    /* GPIOA và GPIOB sẽ được bật bởi GPIOx_Init() */
}

void Error_Handler(void)
{
    __disable_irq();
    while (1) {}
}
