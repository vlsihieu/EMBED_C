/**
 ******************************************************************************
 * @file    main.c
 * @brief   Main application for STM32F103C8T6 using custom GPIO library
 ******************************************************************************
 */

#include "main.h"
#include "hal_gpio.h" // Your custom GPIO library
#include "hal_clock.h"

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init_Custom(void);

int main(void)
{
    /* 1. Initialize ST HAL library (for SysTick and Clocks) */
    HAL_Init();

    /* 2. Configure system clock to 72MHz */
    SystemClock_Config();

    /* 3. Initialize GPIO using your custom library */
    MX_GPIO_Init_Custom();

    /* 4. Main application loop */
    while (1)
    {
        /* Toggle PC13 pin (Onboard LED on Blue Pill) */
        GPIO_Toggle_t(GPIOCx, GPIOx_PIN_13);

        /* Delay for approximately 500ms */
        HAL_Delay(500);
    }
}

/**
 * @brief  Configure GPIO pins using custom library
 * @retval None
 */
static void MX_GPIO_Init_Custom(void)
{
    /* Step 1: Enable clock for PORT C (equivalent to __HAL_RCC_GPIOC_CLK_ENABLE) */
    /* Note: If your hal_clock.h doesn't have this function yet,
       ensure the corresponding bit is set in RCC->APB2ENR */
    HAL_CLOCK_APB2_Peripheral_Enable(RCCx_APB2ENR_Index_GPIOC);

    /* Step 2: Declare configuration structure */
    GPIOx_InitTypeDef_t led_config = {0};

    led_config.Pin   = GPIOx_PIN_13 | GPIOx_PIN_12 | GPIOx_PIN_11;
    led_config.Mode  = GPIOx_MODE_OUTPUT_PP;
    led_config.Speed = GPIOx_SPEED_FREQ_HIGH;
    led_config.Pull  = GPIOx_NOPULL;

    /* Step 3: Call initialization function */
    GPIO_Init_t(GPIOCx, &led_config);
}

/**
 * @brief  System Clock Configuration (Standard 72MHz for STM32F103)
 * @retval None
 */
void SystemClock_Config(void)
{
    /* Configure 72MHz Clock using custom library */
    if (HAL_CLOCK_ConfigHSE() != STD_OK)
    {
        /* Handle clock configuration error */
        while (1);
    }
}

/**
 * @brief  System error handler
 * @retval None
 */
void Error_Handler(void)
{
    __disable_irq();
    while (1) { }
}

/**
 * @brief  Callback for assert_param (required if USE_FULL_ASSERT is defined)
 * @param  file: Pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* Report error: File %s, Line %d */
    while (1);
}
