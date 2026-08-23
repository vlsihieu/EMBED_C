/***********************************************************************************************************************
 * Project Name: STM32F103_BareMetal_Drivers
 * 
 * File Name: hal_timer.h
 *
 * Description: Implementation of HAL Timer Driver (Base, PWM, IC, Encoder) for STM32F103C8T6
 * 
 *
 * Compiler: GCC
 *
 * Revision:
 *              Version         Date                Change History
 *              1.0.0           10/08/2026          Initial release for STM32F103C8T6 Timer Module
 *
 **********************************************************************************************************************/

#ifndef HAL_TIMER_H_
#define HAL_TIMER_H_

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 *                                                     INCLUDES
 **********************************************************************************************************************/

#include <stddef.h>
#include <stdint.h>
#include "hal_clock.h"
#include "define.h"

/***********************************************************************************************************************
 *                                                 SOURCE FILE VERSION
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                                 FILE VERSION CHECK
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                                   LOCAL MACROS
 **********************************************************************************************************************/

 /** @name Counter Modes
 * @{
 */
#define TIM_COUNTERMODE_UP              (0x00000000U) /**< Up-counting mode. */
#define TIM_COUNTERMODE_DOWN            (0x00000010U) /**< Down-counting mode. */
#define TIM_COUNTERMODE_CENTERALIGNED1  (0x00000020U) /**< Center-aligned mode 1. */
/** @} */

/** @name Timer Channels
 * @{
 */
#define TIM_CHANNEL_1                   (0x00000000U) /**< Timer channel 1. */
#define TIM_CHANNEL_2                   (0x00000004U) /**< Timer channel 2. */
#define TIM_CHANNEL_3                   (0x00000008U) /**< Timer channel 3. */
#define TIM_CHANNEL_4                   (0x0000000CU) /**< Timer channel 4. */
/** @} */

/** @name Output Compare Modes
 * @{
 */
#define TIM_OCMODE_TIMING               (0x00000000U) /**< Output compare timing mode. */
#define TIM_OCMODE_ACTIVE               (0x00000010U) /**< Force active level on match. */
#define TIM_OCMODE_INACTIVE             (0x00000020U) /**< Force inactive level on match. */
#define TIM_OCMODE_TOGGLE               (0x00000030U) /**< Toggle output level on match. */
#define TIM_OCMODE_PWM1                 (0x00000060U) /**< PWM mode 1. */
#define TIM_OCMODE_PWM2                 (0x00000070U) /**< PWM mode 2. */
/** @} */

/** @name Output Compare Polarities
 * @{
 */
#define TIM_OCPOLARITY_HIGH             (0x00000000U) /**< Active-high output polarity. */
#define TIM_OCPOLARITY_LOW              (0x00000002U) /**< Active-low output polarity. */
/** @} */

/** @name Input Capture Polarities
 * @{
 */
#define TIM_ICPOLARITY_RISING           (0x00000000U) /**< Capture on rising edge. */
#define TIM_ICPOLARITY_FALLING          (0x00000002U) /**< Capture on falling edge. */
#define TIM_ICPOLARITY_BOTHEDGE         (0x0000000AU) /**< Capture on both edges. */
/** @} */

/** @name Input Capture Selections
 * @{
 */
#define TIM_ICSELECTION_DIRECTTI        (0x00000001U) /**< Input mapped to its own channel. */
#define TIM_ICSELECTION_INDIRECTTI      (0x00000002U) /**< Input mapped to the opposite channel. */
/** @} */

/** @name Encoder Modes
 * @{
 */
#define TIM_ENCODERMODE_TI1             (0x00000001U) /**< Count using TI1 edges. */
#define TIM_ENCODERMODE_TI2             (0x00000002U) /**< Count using TI2 edges. */
#define TIM_ENCODERMODE_TI12            (0x00000003U) /**< Count using TI1 and TI2 edges. */
/** @} */

/**
 * @brief Check whether a timer instance is supported by this driver.
 *
 * @param[in] INSTANCE Timer peripheral instance.
 *
 * @return Non-zero if the instance is supported; otherwise, zero.
 */
#define IS_TIMx_ALL_INSTANCE(INSTANCE)   	(((INSTANCE) == TIM2x) || \
                                         	 ((INSTANCE) == TIM3x) || \
											 ((INSTANCE) == TIM4x) || \
											 ((INSTANCE) == TIM5x))

/***********************************************************************************************************************
 *                                       LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
 **********************************************************************************************************************/

 /**
 * @brief Timer base configuration structure.
 */
typedef struct
{
    uint32_t Prescaler;          /**< Prescaler value applied to the timer clock. */
    uint32_t CounterMode;        /**< Counter mode; use a TIM_COUNTERMODE_x value. */
    uint32_t Period;             /**< Auto-reload value that defines the timer period. */
    uint32_t ClockDivision;      /**< Timer clock division setting. */
    uint32_t RepetitionCounter;  /**< Repetition counter value, when supported. */
} TIMx_Base_InitTypeDef;

/**
 * @brief Timer output compare and PWM channel configuration structure.
 */
typedef struct
{
    uint32_t OCMode;      /**< Output compare mode; use a TIM_OCMODE_x value. */
    uint32_t Pulse;       /**< Capture/compare value that defines the output pulse. */
    uint32_t OCPolarity;  /**< Output polarity; use a TIM_OCPOLARITY_x value. */
    uint32_t OCFastMode;  /**< Output compare fast-mode setting. */
} TIMx_OC_InitTypeDef;

/**
 * @brief Timer input capture channel configuration structure.
 */
typedef struct
{
    uint32_t ICPolarity;  /**< Capture polarity; use a TIM_ICPOLARITY_x value. */
    uint32_t ICSelection; /**< Input mapping; use a TIM_ICSELECTION_x value. */
    uint32_t ICPrescaler; /**< Input capture prescaler setting. */
    uint32_t ICFilter;    /**< Digital input filter setting. */
} TIMx_IC_InitTypeDef;

/**
 * @brief Timer encoder interface configuration structure.
 */
typedef struct
{
    uint32_t EncoderMode;  /**< Encoder mode; use a TIM_ENCODERMODE_x value. */
    uint32_t IC1Polarity;  /**< Input channel 1 capture polarity. */
    uint32_t IC1Selection; /**< Input channel 1 mapping. */
    uint32_t IC1Prescaler; /**< Input channel 1 prescaler. */
    uint32_t IC1Filter;    /**< Input channel 1 digital filter. */
    uint32_t IC2Polarity;  /**< Input channel 2 capture polarity. */
    uint32_t IC2Selection; /**< Input channel 2 mapping. */
    uint32_t IC2Prescaler; /**< Input channel 2 prescaler. */
    uint32_t IC2Filter;    /**< Input channel 2 digital filter. */
} TIMx_Encoder_InitTypeDef;

/**
 * @brief Timer handle structure.
 */
typedef struct __TIM_HandleTypeDef
{
	TIMx_TypeDef_t         *Instance; /**< Timer peripheral register base address. */
    TIMx_Base_InitTypeDef  Init;     /**< Timer base configuration. */
} TIMx_HandleTypeDef;

/***********************************************************************************************************************
 *                                                      EXTERN
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                                  LOCAL CONSTANTS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                                  LOCAL VARIABLES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                                 GLOBAL VARIABLES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                             LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                                  LOCAL FUNCTION
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                                  GLOBAL FUNCTION
 **********************************************************************************************************************/

HALx_StatusTypeDef HAL_TIM_Base_Init(TIMx_HandleTypeDef *htim);
HALx_StatusTypeDef HAL_TIM_Base_DeInit(TIMx_HandleTypeDef *htim);
HALx_StatusTypeDef HAL_TIM_Base_Start(TIMx_HandleTypeDef *htim);
HALx_StatusTypeDef HAL_TIM_Base_Stop(TIMx_HandleTypeDef *htim);
HALx_StatusTypeDef HAL_TIM_Base_Start_IT(TIMx_HandleTypeDef *htim);
HALx_StatusTypeDef HAL_TIM_Base_Stop_IT(TIMx_HandleTypeDef *htim);
HALx_StatusTypeDef HAL_TIM_PWM_Init(TIMx_HandleTypeDef *htim);
HALx_StatusTypeDef HAL_TIM_PWM_ConfigChannel(TIMx_HandleTypeDef *htim,TIMx_OC_InitTypeDef *sConfig, uint32_t Channel);
HALx_StatusTypeDef HAL_TIM_PWM_Start(TIMx_HandleTypeDef *htim,uint32_t Channel);
HALx_StatusTypeDef HAL_TIM_PWM_Stop(TIMx_HandleTypeDef *htim,uint32_t Channel);
HALx_StatusTypeDef HAL_TIM_IC_Init(TIMx_HandleTypeDef *htim);
HALx_StatusTypeDef HAL_TIM_IC_ConfigChannel(TIMx_HandleTypeDef *htim,TIMx_IC_InitTypeDef *sConfig,uint32_t Channel);
HALx_StatusTypeDef HAL_TIM_IC_Start(TIMx_HandleTypeDef *htim,uint32_t Channel);
HALx_StatusTypeDef HAL_TIM_IC_Start_IT(TIMx_HandleTypeDef *htim, uint32_t Channel);
uint32_t HAL_TIM_ReadCapturedValue(TIMx_HandleTypeDef *htim, uint32_t Channel);
HALx_StatusTypeDef HAL_TIM_Encoder_Init(TIMx_HandleTypeDef *htim, TIMx_Encoder_InitTypeDef *sConfig);
HALx_StatusTypeDef HAL_TIM_Encoder_Start(TIMx_HandleTypeDef *htim, uint32_t Channel);
HALx_StatusTypeDef HAL_TIM_Encoder_Stop(TIMx_HandleTypeDef *htim, uint32_t Channel);
void HAL_TIM_DelayUs(TIMx_HandleTypeDef *htim, uint32_t Us);
void HAL_TIM_DelayMs(TIMx_HandleTypeDef *htim, uint32_t Ms);
void HAL_TIM_IRQHandler(TIMx_HandleTypeDef *htim);
void HALx_TIM_PeriodElapsedCallback(TIMx_HandleTypeDef *htim);
void HALx_TIM_IC_CaptureCallback(TIMx_HandleTypeDef *htim);

#ifdef __cplusplus
}
#endif

#endif /* HAL_TIMER_H_ */

/*--------------------------------------------------- End Of File -----------------------------------------------------*/
