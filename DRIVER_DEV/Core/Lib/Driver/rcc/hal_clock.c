/***********************************************************************************************************************
 * Project Name: STM32F103_BareMetal_Drivers
 * 
 * File Name: hal_clock.c
 *
 * Description: Implementation of System Clock Configuration and RCC Driver for STM32F103C8T6
 * 
 *
 * Compiler: GCC
 *
 * Revision:
 *              Version         Date                Change History
 *              1.0.0           10/08/2026          Initial release for STM32F103xB RCC Module
 *
 **********************************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 *                                                    INCLUDES
 **********************************************************************************************************************/

#include "hal_clock.h"

/***********************************************************************************************************************
 *                                               SOURCE FILE VERSION
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                                FILE VERSION CHECK
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                                 LOCAL MACROS
 **********************************************************************************************************************/

#define CLOCK_TIMEOUT_VALUE     (0x000FFFFFU)

/***********************************************************************************************************************
 *                                                     EXTERN
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                      LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                                 LOCAL CONSTANTS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                                 LOCAL VARIABLES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                                GLOBAL VARIABLES
 **********************************************************************************************************************/

uint32_t sysCoreClock = 8000000U;

/***********************************************************************************************************************
 *                                           LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                                 LOCAL FUNCTION
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                                GLOBAL FUNCTION
 **********************************************************************************************************************/

/**
 * @brief  Initialise the Cortex-M3 SysTick timer.
 *
 * Programs LOAD with (ticks - 1), clears the current value, then
 * enables the counter and its interrupt with AHB/8 as clock source.
 * Used internally to generate the 1 ms HAL tick.
 *
 * @param[in] ticks Reload value - number of SysTick clock cycles between
 *                  successive interrupts. Pass (HCLK / 1000) for a 1 ms period.
 *
 * @retval None
 */
void HALx_SYSTICK_Config(uint32_t ticks)
{
    if ((ticks - 1U) <= 0x00FFFFFFU)
    {
        SysTick->LOAD = (uint32_t)(ticks - 1U);
        SysTick->VAL  = 0U;
        SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | 
                        SysTick_CTRL_TICKINT_Msk   | 
                        SysTick_CTRL_ENABLE_Msk;
    }
}

/**
 * @brief  Configure the system clock to 72 MHz using HSE and PLL.
 *
 * Clock configuration:
 * HSE (8 MHz) -> PLL (x9) -> SYSCLK (72 MHz)
 * HCLK  = 72 MHz
 * PCLK1 = 36 MHz
 * PCLK2 = 72 MHz
 *
 * @retval HAL_OK      Clock configured successfully.
 * @retval HAL_TIMEOUT Timeout occurred during clock configuration.
 */
HALx_StatusTypeDef HAL_CLOCK_ConfigHSE(void)
{
    CLOCKx_OscInitTypeDef_t osc_init_struct = {0};
    CLOCKx_ClkInitTypeDef_t clk_init_struct = {0};
    uint32_t timeout_counter = 0U;

    /* Load configuration for HSE + PLL */
    osc_init_struct.OscillatorType = CLOCKx_OSCType_HSE;
    osc_init_struct.HSEState       = CLOCKx_State_ON;
    osc_init_struct.PLLState       = CLOCKx_State_ON;
    osc_init_struct.PLLSource      = CLOCK_PLLSOURCE_HSE;
    osc_init_struct.PLLMul         = CLOCKx_PLLMul_9;

    /* Enable HSE Oscillator and wait until ready */
    if (osc_init_struct.HSEState == CLOCKx_State_ON)
    {
        RCC->CR |= RCCx_CR_HSEON;

        while ((RCC->CR & RCCx_CR_HSERDY) == 0U)
        {
            timeout_counter++;
            if (timeout_counter > CLOCK_TIMEOUT_VALUE)
            {
                return STD_TIMEOUT;
            }
        }
    }

    /* Flash configuration: 2 Wait States + Prefetch Buffer */
    FLASH->ACR = (FLASH->ACR & ~FLASH_ACR_LATENCY) | FLASH_ACR_LATENCY_2;
    FLASH->ACR |= FLASH_ACR_PRFTBE;

    /* Configure and enable PLL */
    if (osc_init_struct.PLLState == CLOCKx_State_ON)
    {
        /* Routine PLLXTPRE = 0, PLLMULL = 1, PLLMULL = x9 */
        RCC->CFGR &= ~(RCCx_CFGR_PLLSRC | RCCx_CFGR_PLLMULL);
        RCC->CFGR |= (osc_init_struct.PLLSource | osc_init_struct.PLLMul);

        /* Enable PLL */
        RCC->CR |= RCCx_CR_PLLON;

        timeout_counter = 0U;
        while ((RCC->CR & RCCx_CR_PLLRDY) == 0U)
        {
            timeout_counter++;
            if (timeout_counter > CLOCK_TIMEOUT_VALUE)
            {
                return STD_TIMEOUT;
            }
        }
    }

    /* Set bus dividers (AHB div1, APB1 div2, APB2 div1) */
    clk_init_struct.SYSCLKSource   = CLOCKx_SysSrc_PLLCLK;
    clk_init_struct.AHBCLKDivider  = CLOCKx_AhbDiv_1;
    clk_init_struct.APB1CLKDivider = CLOCKx_Apb1Div_2_APB1;
    clk_init_struct.APB2CLKDivider = CLOCKx_Apb2Div_1_APB2;

    RCC->CFGR &= ~(RCCx_CFGR_HPRE | RCCx_CFGR_PPRE1 | RCCx_CFGR_PPRE2);
    RCC->CFGR |= (clk_init_struct.AHBCLKDivider | clk_init_struct.APB1CLKDivider | clk_init_struct.APB2CLKDivider);

    /* Switch system clock source to PLL and wait for confirmation */
    RCC->CFGR = (RCC->CFGR & ~RCCx_CFGR_SW) | clk_init_struct.SYSCLKSource;

    timeout_counter = 0U;
    while ((RCC->CFGR & RCCx_CFGR_SWS) != RCCx_CFGR_SWS_PLL)
    {
        timeout_counter++;
        if (timeout_counter > CLOCK_TIMEOUT_VALUE)
        {
            return STD_TIMEOUT;
        }
    }

    /* Global clock tracking and SysTick update */
    sysCoreClock = 72000000U;
    HALx_SYSTICK_Config(sysCoreClock / 1000U);

    return STD_OK;
}

/**
 * @brief  Configure the system clock to 64 MHz using HSI and PLL.
 *
 * Clock configuration:
 * HSI (8 MHz) -> Div 2 (4 MHz) -> PLL (x16) -> SYSCLK (64 MHz)
 * HCLK  = 64 MHz
 * PCLK1 = 32 MHz
 * PCLK2 = 64 MHz
 *
 * @retval HAL_OK      Clock configured successfully.
 * @retval HAL_TIMEOUT Timeout occurred during clock configuration.
 */
HALx_StatusTypeDef HAL_CLOCK_ConfigHSI(void)
{
    CLOCKx_OscInitTypeDef_t osc_init_struct = {0};
    CLOCKx_ClkInitTypeDef_t clk_init_struct = {0};
    uint32_t timeout_counter = 0U;

    /* Load configuration for HSI + PLL */
    osc_init_struct.OscillatorType = CLOCKx_OSCType_HSI;
    osc_init_struct.HSEState       = CLOCKx_State_OFF;
    osc_init_struct.PLLState       = CLOCKx_State_ON;
    osc_init_struct.PLLSource      = CLOCK_PLLSOURCE_HSI;
    osc_init_struct.PLLMul         = CLOCKx_PLLMul_16;

    /* Enable HSI Oscillator and wait until ready */
    RCC->CR |= RCCx_CR_HSION;

    while ((RCC->CR & RCCx_CR_HSIRDY) == 0U)
    {
        timeout_counter++;
        if (timeout_counter > CLOCK_TIMEOUT_VALUE)
        {
            return STD_TIMEOUT;
        }
    }

    /* Flash configuration: 2 Wait States + Prefetch Buffer */
    FLASH->ACR = (FLASH->ACR & ~FLASH_ACR_LATENCY) | FLASH_ACR_LATENCY_2;
    FLASH->ACR |= FLASH_ACR_PRFTBE;

    /* Configure and enable PLL (Must disable PLL before re-configuration) */
    if (osc_init_struct.PLLState == CLOCKx_State_ON)
    {
        /* Routine HSI = 8MHz, Div = 2, PLLSRC = 0, PLLMULL = x16 */
        RCC->CFGR &= ~(RCCx_CFGR_PLLSRC | RCCx_CFGR_PLLMULL);
        RCC->CFGR |= (osc_init_struct.PLLSource | osc_init_struct.PLLMul);

        /* Enable PLL */
        RCC->CR &= ~RCCx_CR_PLLON;
        RCC->CR |= RCCx_CR_PLLON;

        timeout_counter = 0U;
        while ((RCC->CR & RCCx_CR_PLLRDY) == 0U)
        {
            timeout_counter++;
            if (timeout_counter > CLOCK_TIMEOUT_VALUE)
            {
                return STD_TIMEOUT;
            }
        }
    }

    /* Set bus dividers (AHB div1, APB1 div2, APB2 div1) */
    clk_init_struct.SYSCLKSource   = CLOCKx_SysSrc_PLLCLK;
    clk_init_struct.AHBCLKDivider  = CLOCKx_AhbDiv_1;
    clk_init_struct.APB1CLKDivider = CLOCKx_Apb1Div_2_APB1;
    clk_init_struct.APB2CLKDivider = CLOCKx_Apb2Div_1_APB2;

    RCC->CFGR &= ~(RCCx_CFGR_HPRE | RCCx_CFGR_PPRE1 | RCCx_CFGR_PPRE2);
    RCC->CFGR |= (clk_init_struct.AHBCLKDivider | clk_init_struct.APB1CLKDivider | clk_init_struct.APB2CLKDivider);

    /* Switch system clock source to PLL and wait for confirmation */
    RCC->CFGR = (RCC->CFGR & ~RCCx_CFGR_SW) | clk_init_struct.SYSCLKSource;

    timeout_counter = 0U;
    while ((RCC->CFGR & RCCx_CFGR_SWS) != RCCx_CFGR_SWS_PLL)
    {
        timeout_counter++;
        if (timeout_counter > CLOCK_TIMEOUT_VALUE)
        {
            return STD_TIMEOUT;
        }
    }

    /* Global clock tracking and SysTick update */
    sysCoreClock = 64000000U;
    HALx_SYSTICK_Config(sysCoreClock / 1000U);

    return STD_OK;
}

/**
 * @brief  Reset the RCC peripheral to its reset state.
 *
 * Restores the RCC clock configuration to its default reset values:
 * - Enables the internal HSI oscillator.
 * - Selects HSI as the system clock source.
 * - Resets AHB, APB1 and APB2 prescalers.
 * - Disables HSE, HSE bypass, Clock Security System (CSS) and PLL.
 * - Restores PLL source and multiplication factor to their reset values.
 * - Disables all RCC interrupts and clears all pending interrupt flags.
 *
 * @note   This function only resets the RCC peripheral. It does not modify
 *         Flash latency, SysTick configuration, or global interrupt state.
 *
 * @retval None
 */
void HAL_CLOCK_DeInit(void)
{
    uint32_t timeout = 0xFFFFU;

    /* 1. Enable HSI oscillator */
    RCC->CR |= (1U << 0U); 

    /* 2. Wait until HSI is ready */
    while ((RCC->CR & (1U << 1U)) == 0U)
    {
        if (--timeout == 0U)
        {
            break;
        }
    }

    /* 3. Switch SYSCLK source back to HSI */
    RCC->CFGR &= ~0x00000003U; 

    /* 4. Reset AHB and APB prescalers */
    RCC->CFGR &= ~(0x00000F00U | 0x000000F0U);

    /* 5. Disable HSE, CSS, and PLL */
    RCC->CR &= ~((1U << 16U) | (1U << 19U) | (1U << 24U));

    /* 6. Reset PLL configuration */
    RCC->CFGR &= ~(0x003F0000U); 

    /* 7. Disable all RCC interrupts and clear pending flags */
    RCC->CIR = 0x00000000U;
    
    /* 8. Update global system clock variable */
    sysCoreClock = 8000000U;
}

/**
 * @brief  Enable the clock for an AHB peripheral.
 *
 * Sets the corresponding bit in RCC->AHBENR.
 * A short delay is automatically inserted after enabling to allow the
 * clock to propagate before the peripheral is accessed.
 *
 * @param[in] PeripheralIndex Peripheral bit position.
 *
 * @retval None
 */
void HAL_CLOCK_AHB_Peripheral_Enable(RCCx_AHBENR_Index_t PeripheralIndex)
{
    RCC->AHBENR |= (1U << (uint32_t)PeripheralIndex);
}

/**
 * @brief  Disable the clock for an AHB peripheral.
 *
 * Clears the corresponding bit in RCC->AHBENR.
 *
 * @param[in] PeripheralIndex Peripheral bit position.
 *
 * @retval None
 */
void HAL_CLOCK_AHB_Peripheral_Disable(RCCx_AHBENR_Index_t PeripheralIndex)
{
    RCC->AHBENR &= ~(1U << (uint32_t)PeripheralIndex);
}

/**
 * @brief  Enable the clock for an APB2 peripheral.
 *
 * Sets the corresponding bit in RCC->APB2ENR.
 *
 * @param[in] PeripheralIndex Peripheral bit position.
 *
 * @retval None
 */
void HAL_CLOCK_APB2_Peripheral_Enable(RCCx_APB2ENR_Index_t PeripheralIndex)
{
    RCC->APB2ENR |= (1U << (uint32_t)PeripheralIndex);
}

/**
 * @brief  Disable the clock for an APB2 peripheral.
 *
 * Clears the corresponding bit in RCC->APB2ENR.
 *
 * @param[in] PeripheralIndex Peripheral bit position.
 *
 * @retval None
 */
void HAL_CLOCK_APB2_Peripheral_Disable(RCCx_APB2ENR_Index_t PeripheralIndex)
{
    RCC->APB2ENR &= ~(1U << (uint32_t)PeripheralIndex);
}

/**
 * @brief  Enable the clock for an APB1 peripheral.
 *
 * Sets the corresponding bit in RCC->APB1ENR.
 *
 * @param[in] PeripheralIndex Peripheral bit position.
 *
 * @retval None
 */
void HAL_CLOCK_APB1_Peripheral_Enable(RCCx_APB1ENR_Index_t PeripheralIndex)
{
    RCC->APB1ENR |= (1U << (uint32_t)PeripheralIndex);
}

/**
 * @brief  Disable the clock for an APB1 peripheral.
 *
 * Clears the corresponding bit in RCC->APB1ENR.
 *
 * @param[in] PeripheralIndex Peripheral bit position.
 *
 * @retval None
 */
void HAL_CLOCK_APB1_Peripheral_Disable(RCCx_APB1ENR_Index_t PeripheralIndex)
{
    RCC->APB1ENR &= ~(1U << (uint32_t)PeripheralIndex);
}

#ifdef __cplusplus
}
#endif

/*--------------------------------------------------- End Of File -----------------------------------------------------*/
