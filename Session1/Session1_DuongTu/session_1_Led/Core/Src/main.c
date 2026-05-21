#include "main.h"
#include "GPIOx.h"

#define LED_PORT GPIOB
#define LED_COUNT 4

#define BTN_PORT GPIOA
#define BTN_A_PIN GPIOx_Pin_0
#define BTN_B_PIN GPIOx_Pin_1
#define BTN_C_PIN GPIOx_Pin_2

#define LED_STEP_DELAY_MS 300

typedef enum {
    STATE_STOP = 0,
    STATE_RIGHT_TO_LEFT,
    STATE_LEFT_TO_RIGHT
} LedState_t;

static const uint8_t led_pins[LED_COUNT] = {
    GPIOx_Pin_11,
    GPIOx_Pin_12,
    GPIOx_Pin_13,
    GPIOx_Pin_14
};

static LedState_t led_state = STATE_STOP;
static int8_t led_index = 0;
static uint32_t last_led_tick = 0;

static uint8_t prev_btn_a = 1;
static uint8_t prev_btn_b = 1;
static uint8_t prev_btn_c = 1;

void SystemClock_Config(void);

static void LED_AllOff(void)
{
    for (uint8_t i = 0; i < LED_COUNT; i++)
        GPIOx_Write(LED_PORT, led_pins[i], 0);
}

static void LED_SetOnly(int8_t index)
{
    for (uint8_t i = 0; i < LED_COUNT; i++)
        GPIOx_Write(LED_PORT, led_pins[i], (i == index));
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();

    GPIOx_Init(LED_PORT, GPIOx_Pin_11, OUT_PUSH_PULL, NO_PULL, OUTPUT_SPEED_50MHZ);
    GPIOx_Init(LED_PORT, GPIOx_Pin_12, OUT_PUSH_PULL, NO_PULL, OUTPUT_SPEED_50MHZ);
    GPIOx_Init(LED_PORT, GPIOx_Pin_13, OUT_PUSH_PULL, NO_PULL, OUTPUT_SPEED_50MHZ);
    GPIOx_Init(LED_PORT, GPIOx_Pin_14, OUT_PUSH_PULL, NO_PULL, OUTPUT_SPEED_50MHZ);

    GPIOx_Init(BTN_PORT, BTN_A_PIN, INPUT_PULL_UP_DOW, PULL_UP, INPUT_MODE);
    GPIOx_Init(BTN_PORT, BTN_B_PIN, INPUT_PULL_UP_DOW, PULL_UP, INPUT_MODE);
    GPIOx_Init(BTN_PORT, BTN_C_PIN, INPUT_PULL_UP_DOW, PULL_UP, INPUT_MODE);

    LED_AllOff();

    while (1)
    {
        uint32_t now = HAL_GetTick();

        uint8_t cur_btn_a = GPIOx_Read(BTN_PORT, BTN_A_PIN);
        uint8_t cur_btn_b = GPIOx_Read(BTN_PORT, BTN_B_PIN);
        uint8_t cur_btn_c = GPIOx_Read(BTN_PORT, BTN_C_PIN);

        if ((cur_btn_a == 1) && (prev_btn_a == 0))
        {
            led_state = STATE_RIGHT_TO_LEFT;
            led_index = LED_COUNT - 1;
            last_led_tick = now;
            LED_SetOnly(led_index);
        }

        if ((cur_btn_b == 1) && (prev_btn_b == 0))
        {
            led_state = STATE_STOP;
            LED_AllOff();
        }

        if ((cur_btn_c == 1) && (prev_btn_c == 0))
        {
            led_state = STATE_LEFT_TO_RIGHT;
            led_index = 0;
            last_led_tick = now;
            LED_SetOnly(led_index);
        }

        prev_btn_a = cur_btn_a;
        prev_btn_b = cur_btn_b;
        prev_btn_c = cur_btn_c;

        if ((now - last_led_tick) >= LED_STEP_DELAY_MS)
        {
            last_led_tick = now;

            if (led_state == STATE_RIGHT_TO_LEFT)
            {
                led_index--;
                if (led_index < 0)
                    led_index = LED_COUNT - 1;

                LED_SetOnly(led_index);
            }
            else if (led_state == STATE_LEFT_TO_RIGHT)
            {
                led_index++;
                if (led_index >= LED_COUNT)
                    led_index = 0;

                LED_SetOnly(led_index);
            }
        }
    }
}

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
        Error_Handler();

    RCC_ClkInitStruct.ClockType =
        RCC_CLOCKTYPE_HCLK |
        RCC_CLOCKTYPE_SYSCLK |
        RCC_CLOCKTYPE_PCLK1 |
        RCC_CLOCKTYPE_PCLK2;

    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
        Error_Handler();
}

void Error_Handler(void)
{
    __disable_irq();
    while (1) {}
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
}
#endif
