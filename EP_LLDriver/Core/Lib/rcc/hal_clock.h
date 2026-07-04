/**
 ******************************************************************************
 * @file    hal_clock.h
 * @author  hieuk
 * @brief   HAL Clock Driver — Data types, register structures, bitmasks,
 *          and API prototypes for the RCC (Reset and Clock Control) peripheral
 *          on STM32F103xB devices.
 * @date    May 6, 2026
 *
 * @details This driver wraps the RCC peripheral to provide:
 *          - System clock configuration via HSE (72 MHz) or HSI (64 MHz)
 *          - PLL setup and source selection
 *          - AHB / APB1 / APB2 bus clock divider configuration
 *          - Per-peripheral clock enable / disable helpers
 *          - SysTick initialisation
 *
 ******************************************************************************
 */

#ifndef HAL_CLOCK_H_
#define HAL_CLOCK_H_

/***********************************************************************************************************************
* INCLUDES AND DEFINITIONS
**********************************************************************************************************************/

#include <stdint.h>
#include "stm32f103xb.h"
#include "define.h"

/***********************************************************************************************************************
* LOCAL MACROS
**********************************************************************************************************************/

#define RCCx_CR_HSIRDY              ((uint32_t)(0x1UL << 1))   ///< HSI oscillator ready flag (CR.HSIRDY)
#define RCCx_CR_HSEON               ((uint32_t)(0x1UL << 16))  ///< Enable HSE oscillator (CR.HSEON)
#define RCCx_CR_HSERDY              ((uint32_t)(0x1UL << 17))  ///< HSE oscillator ready flag (CR.HSERDY)
#define RCCx_CR_PLLON               ((uint32_t)(0x1UL << 24))  ///< Enable PLL (CR.PLLON)
#define RCCx_CR_PLLRDY              ((uint32_t)(0x1UL << 25))  ///< PLL lock ready flag (CR.PLLRDY)
#define RCCx_CFGR_SW                ((uint32_t)(0x3UL << 0))   ///< System clock switch (CFGR.SW[1:0])
#define RCCx_CFGR_SWS               ((uint32_t)(0x3UL << 2))   ///< System clock switch status (CFGR.SWS[1:0])
#define RCCx_CFGR_SWS_PLL           ((uint32_t)(0x2UL << 2))   ///< PLL selected as system clock
#define RCCx_CFGR_HPRE              ((uint32_t)(0xFUL << 4))   ///< AHB prescaler mask (CFGR.HPRE[3:0])
#define RCCx_CFGR_PPRE1             ((uint32_t)(0x7UL << 8))   ///< APB1 prescaler mask (CFGR.PPRE1[2:0])
#define RCCx_CFGR_PPRE2             ((uint32_t)(0x7UL << 11))  ///< APB2 prescaler mask (CFGR.PPRE2[2:0])
#define RCCx_CFGR_PLLSRC            ((uint32_t)(0x1UL << 16))  ///< PLL entry clock source (CFGR.PLLSRC)
#define RCCx_CFGR_PLLMULL           ((uint32_t)(0xFUL << 18))  ///< PLL multiplication factor mask (CFGR.PLLMUL[3:0])
#define RCCx_CR_PLLRDY              ((uint32_t)(0x1UL << 25))  ///< PLL clock ready flag
#define RCCx_CR_HSION               ((uint32_t)(0x1UL << 0))   ///< Internal High Speed clock enable
#define RCCx_CR_PLLON               ((uint32_t)(0x1UL << 24))  ///< PLL enable
#define RCCx_CR_CSSON               ((uint32_t)(0x1UL << 19))  ///< Clock Security System enable
#define RCCx_CR_HSEBYP              ((uint32_t)(0x1UL << 18))  ///< External High Speed clock bypass
#define CLOCK_PLLSOURCE_HSI         ((uint32_t)(0x0UL << 16))  ///< clock = HSI / 2
#define CLOCK_PLLSOURCE_HSE         ((uint32_t)(0x1UL << 16))  ///< clock = HSE


/***********************************************************************************************************************
* LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
**********************************************************************************************************************/

/**
 * @brief  Generic ON / OFF state for clock sources and PLLs.
 */
typedef enum
{
    CLOCKx_State_OFF = 0U, ///< Clock / PLL disabled
    CLOCKx_State_ON  = 1U  ///< Clock / PLL enabled
} CLOCKx_State_t;

/**
 * @brief  Oscillator type selector used in CLOCKx_OscInitTypeDef_t.
 *
 * Can be OR-ed together when both oscillators need to be configured.
 */
typedef enum
{
    CLOCKx_OSCType_HSE = 0x01U, ///< External high-speed oscillator (HSE)
    CLOCKx_OSCType_HSI = 0x02U  ///< Internal high-speed RC oscillator (HSI)
} CLOCKx_OscType_t;

/**
 * @brief  PLL multiplication factor (RCC_CFGR PLLMUL[3:0]).
 *
 * Output frequency = PLL input × multiplier.
 * Maximum output is 72 MHz; do not exceed this value.
 */
typedef enum
{
    CLOCKx_PLLMul_2         = (0x0 << 18),    ///< PLL input clock ×2 
    CLOCKx_PLLMul_3         = (0x1 << 18),    ///< PLL input clock ×3 
    CLOCKx_PLLMul_4         = (0x2 << 18),    ///< PLL input clock ×4 
    CLOCKx_PLLMul_5         = (0x3 << 18),    ///< PLL input clock ×5 
    CLOCKx_PLLMul_6         = (0x4 << 18),    ///< PLL input clock ×6 
    CLOCKx_PLLMul_7         = (0x5 << 18),    ///< PLL input clock ×7 
    CLOCKx_PLLMul_8         = (0x6 << 18),    ///< PLL input clock ×8 
    CLOCKx_PLLMul_9         = (0x7 << 18),    ///< PLL input clock ×9 
    CLOCKx_PLLMul_10        = (0x8 << 18),    ///< PLL input clock ×10
    CLOCKx_PLLMul_11        = (0x9 << 18),    ///< PLL input clock ×11
    CLOCKx_PLLMul_12        = (0xA << 18),    ///< PLL input clock ×12
    CLOCKx_PLLMul_13        = (0xB << 18),    ///< PLL input clock ×13
    CLOCKx_PLLMul_14        = (0xC << 18),    ///< PLL input clock ×14
    CLOCKx_PLLMul_15        = (0xD << 18),    ///< PLL input clock ×15
    CLOCKx_PLLMul_16        = (0xE << 18),    ///< PLL input clock ×16
    CLOCKx_PLLMul_16_ALT    = (0xF << 18)     ///< PLL input clock ×16
} CLOCKx_PLLMul_t;

/**
 * @brief  SYSCLK source selector (RCC_CFGR SW[1:0]).
 */
typedef enum
{
    CLOCKx_SysSrc_HSI    = 0x00U, ///< HSI selected as system clock (reset default)
    CLOCKx_SysSrc_HSE    = 0x01U, ///< HSE selected as system clock                
    CLOCKx_SysSrc_PLLCLK = 0x02U  ///< PLL output selected as system clock         
} CLOCKx_SysSrc_t;

/**
 * @brief  AHB bus (HCLK) prescaler (RCC_CFGR HPRE[3:0]).
 *
 * HCLK = SYSCLK / divider.
 */
typedef enum
{
    CLOCKx_AhbDiv_1   = (0x0 << 4), ///< SYSCLK not divided
    CLOCKx_AhbDiv_2   = (0x8 << 4), ///< SYSCLK divided by 2
    CLOCKx_AhbDiv_4   = (0x9 << 4), ///< SYSCLK divided by 4
    CLOCKx_AhbDiv_8   = (0xA << 4), ///< SYSCLK divided by 8
    CLOCKx_AhbDiv_16  = (0xB << 4), ///< SYSCLK divided by 16
    CLOCKx_AhbDiv_64  = (0xC << 4), ///< SYSCLK divided by 64
    CLOCKx_AhbDiv_128 = (0xD << 4), ///< SYSCLK divided by 128
    CLOCKx_AhbDiv_256 = (0xE << 4), ///< SYSCLK divided by 256
    CLOCKx_AhbDiv_512 = (0xF << 4)  ///< SYSCLK divided by 512
} CLOCKx_AhbDiv_t;

/**
 * @brief  APB1 bus (PCLK1) prescaler (RCC_CFGR PPRE1[2:0]).
 *
 * PCLK1 = HCLK / divider. Maximum PCLK1 is 36 MHz.
 */
typedef enum
{
    CLOCKx_Apb1Div_1_APB1  = (0x0 << 8), ///< HCLK not divided  
    CLOCKx_Apb1Div_2_APB1  = (0x4 << 8), ///< HCLK divided by 2 
    CLOCKx_Apb1Div_4_APB1  = (0x5 << 8), ///< HCLK divided by 4 
    CLOCKx_Apb1Div_8_APB1  = (0x6 << 8), ///< HCLK divided by 8 
    CLOCKx_Apb1Div_16_APB1 = (0x7 << 8)  ///< HCLK divided by 16
} CLOCKx_Apb1Div_t;

/**
 * @brief  APB2 bus (PCLK2) prescaler (RCC_CFGR PPRE2[2:0]).
 *
 * PCLK2 = HCLK / divider. Maximum PCLK2 is 72 MHz.
 */
typedef enum
{
    CLOCKx_Apb2Div_1_APB2  = (0x0UL << 11),     ///< HCLK not divided  
    CLOCKx_Apb2Div_2_APB2  = (0x4UL << 11),     ///< HCLK divided by 2 
    CLOCKx_Apb2Div_4_APB2  = (0x5UL << 11),     ///< HCLK divided by 4 
    CLOCKx_Apb2Div_8_APB2  = (0x6UL << 11),     ///< HCLK divided by 8 
    CLOCKx_Apb2Div_16_APB2 = (0x7UL << 11)      ///< HCLK divided by 16
} CLOCKx_Apb2Div_t;

/**
 * @brief  AHB peripheral indices for AHBENR register.
 *
 * Each value corresponds to the bit position of the peripheral's
 * clock-enable bit inside RCC->AHBENR.
 */
typedef enum
{
    RCCx_AHBENR_Index_DMA1  = 0U,  ///< DMA1 controller
    RCCx_AHBENR_Index_DMA2  = 1U,  ///< DMA2 controller
    RCCx_AHBENR_Index_SRAM  = 2U,  ///< SRAM interface 
    RCCx_AHBENR_Index_FLITF = 4U,  ///< Flash interface
    RCCx_AHBENR_Index_CRC   = 6U,  ///< CRC unit       
    RCCx_AHBENR_Index_FSMC  = 8U,  ///< FSMC           
    RCCx_AHBENR_Index_SDIO  = 10U  ///< SDIO           
} RCCx_AHBENR_Index_t;

/**
 * @brief  APB2 peripheral indices for APB2ENR register.
 *
 * Each value corresponds to the bit position of the peripheral's
 * clock-enable bit inside RCC->APB2ENR.
 */
typedef enum
{
    RCCx_APB2ENR_Index_AFIO   = 0U,  ///< Alternate function I/O 
    RCCx_APB2ENR_Index_GPIOA  = 2U,  ///< GPIOA                  
    RCCx_APB2ENR_Index_GPIOB  = 3U,  ///< GPIOB                  
    RCCx_APB2ENR_Index_GPIOC  = 4U,  ///< GPIOC                  
    RCCx_APB2ENR_Index_GPIOD  = 5U,  ///< GPIOD                  
    RCCx_APB2ENR_Index_GPIOE  = 6U,  ///< GPIOE                  
    RCCx_APB2ENR_Index_ADC1   = 9U,  ///< ADC1                   
    RCCx_APB2ENR_Index_ADC2   = 10U, ///< ADC2                   
    RCCx_APB2ENR_Index_TIM1   = 11U, ///< TIM1 advanced timer    
    RCCx_APB2ENR_Index_SPI1   = 12U, ///< SPI1                   
    RCCx_APB2ENR_Index_USART1 = 14U  ///< USART1                 
} RCCx_APB2ENR_Index_t;

/**
 * @brief  APB1 peripheral indices for APB1ENR register.
 *
 * Each value corresponds to the bit position of the peripheral's
 * clock-enable bit inside RCC->APB1ENR.
 */
typedef enum
{
    RCCx_APB1ENR_Index_TIM2   = 0U,  ///< TIM2 general-purpose timer 
    RCCx_APB1ENR_Index_TIM3   = 1U,  ///< TIM3 general-purpose timer 
    RCCx_APB1ENR_Index_TIM4   = 2U,  ///< TIM4 general-purpose timer 
    RCCx_APB1ENR_Index_WWDG   = 11U, ///< Window watchdog            
    RCCx_APB1ENR_Index_SPI2   = 14U, ///< SPI2 / I2S2                
    RCCx_APB1ENR_Index_USART2 = 17U, ///< USART2                     
    RCCx_APB1ENR_Index_USART3 = 18U, ///< USART3                     
    RCCx_APB1ENR_Index_I2C1   = 21U, ///< I2C1                       
    RCCx_APB1ENR_Index_I2C2   = 22U, ///< I2C2                       
    RCCx_APB1ENR_Index_CAN    = 25U, ///< bxCAN                      
    RCCx_APB1ENR_Index_BKP    = 27U, ///< Backup interface           
    RCCx_APB1ENR_Index_PWR    = 28U  ///< Power control              
} RCCx_APB1ENR_Index_t;


/* ============================================================
 * Configuration Structure Definitions
 * ============================================================ */

/**
 * @brief  Oscillator initialisation structure.
 *
 * Passed to HAL_CLOCK_ConfigHSE() / HAL_CLOCK_ConfigHSI() to describe
 * which oscillator(s) to configure and the desired PLL settings.
 *
 * Example — configure HSE + PLL × 9 for 72 MHz SYSCLK:
 * @code
 *   CLOCKx_OscInitTypeDef_t osc = {
 *       .OscillatorType = CLOCK_OSCILLATORTYPE_HSE,
 *       .HSEState       = CLOCK_STATE_ON,
 *       .PLLState       = CLOCK_STATE_ON,
 *       .PLLSource      = CLOCK_PLLSOURCE_HSE,
 *       .PLLMul         = CLOCK_PLL_MUL9
 *   };
 * @endcode
 */
typedef struct
{
    CLOCKx_OscType_t  OscillatorType; /*!< Oscillator(s) to configure. See @ref CLOCKx_OscType_t.    */
    CLOCKx_State_t    HSEState;       /*!< Desired HSE state (ON / OFF).                              */
    CLOCKx_State_t    PLLState;       /*!< Desired PLL state (ON / OFF).                              */
    uint32_t          PLLSource;      /*!< PLL input clock. Use CLOCK_PLLSOURCE_* macros.             */
    CLOCKx_PLLMul_t   PLLMul;         /*!< PLL multiplication factor. See @ref CLOCKx_PLLMul_t.       */
} CLOCKx_OscInitTypeDef_t;

/**
 * @brief  Bus clock initialisation structure.
 *
 * Configures the SYSCLK source and the AHB / APB1 / APB2 prescalers.
 *
 * Example — select PLL as SYSCLK, divide APB1 by 2 (36 MHz):
 * @code
 *   CLOCKx_ClkInitTypeDef_t clk = {
 *       .SYSCLKSource   = CLOCK_SYSCLKSOURCE_PLLCLK,
 *       .AHBCLKDivider  = CLOCK_SYSCLK_DIV1,
 *       .APB1CLKDivider = CLOCK_HCLK_DIV2_APB1,
 *       .APB2CLKDivider = CLOCK_HCLK_DIV1_APB2
 *   };
 * @endcode
 */
typedef struct
{
    CLOCKx_SysSrc_t  SYSCLKSource;   /*!< SYSCLK source. See @ref CLOCKx_SysSrc_t.                  */
    CLOCKx_AhbDiv_t  AHBCLKDivider;  /*!< AHB (HCLK) prescaler. See @ref CLOCKx_AhbDiv_t.           */
    CLOCKx_Apb1Div_t APB1CLKDivider; /*!< APB1 (PCLK1) prescaler. See @ref CLOCKx_Apb1Div_t.        */
    CLOCKx_Apb2Div_t APB2CLKDivider; /*!< APB2 (PCLK2) prescaler. See @ref CLOCKx_Apb2Div_t.        */
} CLOCKx_ClkInitTypeDef_t;


/***********************************************************************************************************************
* GLOBAL FUNCTION PROTOTYPES
**********************************************************************************************************************/

HALx_StatusTypeDef HAL_CLOCK_ConfigHSE(void);
HALx_StatusTypeDef HAL_CLOCK_ConfigHSI(void);
void HAL_CLOCK_DeInit(void);
void HALx_SYSTICK_Config(uint32_t ticks);
void HAL_CLOCK_AHB_Peripheral_Enable(RCCx_AHBENR_Index_t PeripheralIndex);
void HAL_CLOCK_AHB_Peripheral_Disable(RCCx_AHBENR_Index_t PeripheralIndex);
void HAL_CLOCK_APB2_Peripheral_Enable(RCCx_APB2ENR_Index_t PeripheralIndex);
void HAL_CLOCK_APB2_Peripheral_Disable(RCCx_APB2ENR_Index_t PeripheralIndex);
void HAL_CLOCK_APB1_Peripheral_Enable(RCCx_APB1ENR_Index_t PeripheralIndex);
void HAL_CLOCK_APB1_Peripheral_Disable(RCCx_APB1ENR_Index_t PeripheralIndex);


#endif /* HAL_CLOCK_H_ */
