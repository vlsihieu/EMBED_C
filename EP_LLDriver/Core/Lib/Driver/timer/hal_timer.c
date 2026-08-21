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

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 *                                                     INCLUDES
 **********************************************************************************************************************/

#include "hal_timer.h"

/***********************************************************************************************************************
 *                                                 SOURCE FILE VERSION
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                                 FILE VERSION CHECK
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                                   LOCAL MACROS
 **********************************************************************************************************************/

#define NVIC_ISER0     (*(volatile uint32_t *)(0xE000E100U))
#define NVIC_ISER1     (*(volatile uint32_t *)(0xE000E104U))
#define NVIC_ICER0     (*(volatile uint32_t *)(0xE000E180U))
#define NVIC_ICER1     (*(volatile uint32_t *)(0xE000E184U))

/***********************************************************************************************************************
 *                                                      EXTERN
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                       LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                                  LOCAL CONSTANTS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                                  LOCAL VARIABLES
 **********************************************************************************************************************/

/* Timer handles used by the interrupt vector handlers. */
static TIMx_HandleTypeDef *gp_tim_handles[5];

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

/**
 * @brief Enable the peripheral clock for the selected timer.
 *
 * @param[in] TIMx Pointer to the timer peripheral registers.
 */
static void prv_TIM_EnableClock(TIMx_TypeDef_t *TIMx)
{
	if (TIMx == TIM1x)
	{
		HAL_CLOCK_APB2_Peripheral_Enable(RCCx_APB2ENR_Index_TIM1);
	}
	else if (TIMx == TIM2x)
    {
        HAL_CLOCK_APB1_Peripheral_Enable(RCCx_APB1ENR_Index_TIM2);

    }
    else if (TIMx == TIM3x)
    {
        HAL_CLOCK_APB1_Peripheral_Enable(RCCx_APB1ENR_Index_TIM3);
    }
    else if (TIMx == TIM4x)
    {
        HAL_CLOCK_APB1_Peripheral_Enable(RCCx_APB1ENR_Index_TIM4);
    }
    else
    {
        /* Unsupported timer instance. */
    }
}

/**
 * @brief Convert a timer instance into its handle-table index.
 *
 * @param[in] TIMx Pointer to the timer peripheral registers.
 *
 * @return Timer index in the range 1 to 4, or 0 for an unsupported instance.
 */
static uint8_t prv_TIM_GetIndex(TIMx_TypeDef_t *TIMx)
{
    uint8_t index = 0U;

    if (TIMx == TIM1x)
    {
        index = 1U;
    }
    else if (TIMx == TIM2x)
    {
        index = 2U;
    }
    else if (TIMx == TIM3x)
    {
        index = 3U;
    }
    else if (TIMx == TIM4x)
    {
        index = 4U;
    }
    else
    {
        /* Keep the invalid index value. */
    }

    return index;
}

/**
 * @brief Enable an interrupt request in the NVIC.
 *
 * @param[in] irq Interrupt request number.
 */
static void prv_NVIC_EnableIRQ(uint8_t irq)
{
    if (irq < 32U)
    {
        NVIC_ISER0 |= (1U << irq);
    }
    else
    {
        NVIC_ISER1 |= (1U << (irq - 32U));
    }
}

/**
 * @brief Disable an interrupt request in the NVIC.
 *
 * @param[in] irq Interrupt request number.
 */
static void prv_NVIC_DisableIRQ(uint8_t irq)
{
    if (irq < 32U)
    {
        NVIC_ICER0 |= (1U << irq);
    }
    else
    {
        NVIC_ICER1 |= (1U << (irq - 32U));
    }
}

/**
 * @brief Initialize the timer time base.
 *
 * @param[in,out] htim Pointer to the timer handle.
 *
 * @return HAL_TIM_OK if initialization succeeds; otherwise, HAL_TIM_ERROR.
 */
HALx_StatusTypeDef HAL_TIM_Base_Init(TIMx_HandleTypeDef *htim)
{
    uint32_t tmpcr1;
    uint8_t index;

    if ((htim == NULL) || (!IS_TIMx_ALL_INSTANCE(htim->Instance)))
    {
        return STD_ERROR;
    }

    prv_TIM_EnableClock(htim->Instance);

    htim->Instance->PSC = htim->Init.Prescaler;
    htim->Instance->ARR = htim->Init.Period;
    htim->Instance->RCR = htim->Init.RepetitionCounter;

    tmpcr1 = htim->Instance->CR1;
    tmpcr1 &= ~(0x00000070U);
    tmpcr1 |= htim->Init.CounterMode;
    htim->Instance->CR1 = tmpcr1;

    /* Generate an update event to load the prescaler and auto-reload values. */
    htim->Instance->EGR |= (1U << 0U);

    index = prv_TIM_GetIndex(htim->Instance);
    if (index != 0U)
    {
        gp_tim_handles[index] = htim;
    }

    return STD_OK;
}

/**
 * @brief Start the timer counter in polling mode.
 *
 * @param[in,out] htim Pointer to the timer handle.
 *
 * @return HAL_TIM_OK if the timer is started; otherwise, HAL_TIM_ERROR.
 */
HALx_StatusTypeDef HAL_TIM_Base_Start(TIMx_HandleTypeDef *htim)
{
    if ((htim == NULL) || (!IS_TIMx_ALL_INSTANCE(htim->Instance)))
    {
        return STD_ERROR;
    }

    htim->Instance->CR1 |= (1U << 0U);

    return STD_OK;
}

/**
 * @brief Stop the timer counter in polling mode.
 *
 * @param[in,out] htim Pointer to the timer handle.
 *
 * @return HAL_TIM_OK if the timer is stopped; otherwise, HAL_TIM_ERROR.
 */
HALx_StatusTypeDef HAL_TIM_Base_Stop(TIMx_HandleTypeDef *htim)
{
    if ((htim == NULL) || (!IS_TIMx_ALL_INSTANCE(htim->Instance)))
    {
        return STD_ERROR;
    }

    htim->Instance->CR1 &= ~(1U << 0U);

    return STD_OK;
}

/**
 * @brief Start the timer counter with update interrupts enabled.
 *
 * @param[in,out] htim Pointer to the timer handle.
 *
 * @return HAL_TIM_OK if the timer is started; otherwise, HAL_TIM_ERROR.
 */
HALx_StatusTypeDef HAL_TIM_Base_Start_IT(TIMx_HandleTypeDef *htim)
{
    uint8_t irq;

    if ((htim == NULL) || (!IS_TIMx_ALL_INSTANCE(htim->Instance)))
    {
        return STD_ERROR;
    }

    htim->Instance->SR &= ~(1U << 0U);
    htim->Instance->DIER |= (1U << 0U);

    irq = (htim->Instance == TIM1x) ? 25U :
          (htim->Instance == TIM2x) ? 28U :
          (htim->Instance == TIM3x) ? 29U : 30U;
    prv_NVIC_EnableIRQ(irq);

    htim->Instance->CR1 |= (1U << 0U);

    return STD_OK;
}

/**
 * @brief Stop the timer counter and disable update interrupts.
 *
 * @param[in,out] htim Pointer to the timer handle.
 *
 * @return HAL_TIM_OK if the timer is stopped; otherwise, HAL_TIM_ERROR.
 */
HALx_StatusTypeDef HAL_TIM_Base_Stop_IT(TIMx_HandleTypeDef *htim)
{
    uint8_t irq;

    if ((htim == NULL) || (!IS_TIMx_ALL_INSTANCE(htim->Instance)))
    {
        return STD_ERROR;
    }

    htim->Instance->DIER &= ~(1U << 0U);
    htim->Instance->CR1 &= ~(1U << 0U);

    irq = (htim->Instance == TIM1x) ? 25U :
          (htim->Instance == TIM2x) ? 28U :
          (htim->Instance == TIM3x) ? 29U : 30U;
    prv_NVIC_DisableIRQ(irq);

    return STD_OK;
}

/**
 * @brief Initialize the timer for PWM output operation.
 *
 * @param[in,out] htim Pointer to the timer handle.
 *
 * @return Status returned by HAL_TIM_Base_Init().
 */
HALx_StatusTypeDef HAL_TIM_PWM_Init(TIMx_HandleTypeDef *htim)
{
    return HAL_TIM_Base_Init(htim);
}

/**
 * @brief Configure a timer channel for PWM output.
 *
 * @param[in,out] htim    Pointer to the timer handle.
 * @param[in]     sConfig Pointer to the output compare configuration.
 * @param[in]     Channel Timer channel; use a TIM_CHANNEL_x value.
 *
 * @return HAL_TIM_OK if configuration succeeds; otherwise, HAL_TIM_ERROR.
 */
HALx_StatusTypeDef HAL_TIM_PWM_ConfigChannel(
    TIMx_HandleTypeDef *htim,
    TIMx_OC_InitTypeDef *sConfig,
    uint32_t Channel)
{
    __IO uint32_t *pCCMR;
    uint8_t shift;

    if ((htim == NULL) || (sConfig == NULL))
    {
        return STD_ERROR;
    }

    pCCMR = (Channel <= TIM_CHANNEL_2) ?
            &htim->Instance->CCMR1 : &htim->Instance->CCMR2;
    shift = ((Channel == TIM_CHANNEL_1) || (Channel == TIM_CHANNEL_3)) ? 0U : 8U;

    *pCCMR &= ~(0xFFU << shift);
    *pCCMR |= ((sConfig->OCMode | (1U << 3U)) << shift);

    htim->Instance->CCER &= ~(1U << (Channel + 1U));
    htim->Instance->CCER |= (sConfig->OCPolarity << Channel);

    *(&htim->Instance->CCR1 + (Channel >> 2U)) = sConfig->Pulse;

    if (htim->Instance == TIM1x)
    {
        /* Enable the main output for the advanced-control timer. */
        htim->Instance->BDTR |= (1U << 15U);
    }

    return STD_OK;
}

/**
 * @brief Start PWM generation on the selected channel.
 *
 * @param[in,out] htim    Pointer to the timer handle.
 * @param[in]     Channel Timer channel; use a TIM_CHANNEL_x value.
 *
 * @return HAL_TIM_OK.
 */
HALx_StatusTypeDef HAL_TIM_PWM_Start(
    TIMx_HandleTypeDef *htim,
    uint32_t Channel)
{
    htim->Instance->CCER |= (1U << Channel);
    htim->Instance->CR1 |= (1U << 0U);

    return STD_OK;
}

/**
 * @brief Stop PWM generation on the selected channel.
 *
 * @param[in,out] htim    Pointer to the timer handle.
 * @param[in]     Channel Timer channel; use a TIM_CHANNEL_x value.
 *
 * @return HAL_TIM_OK.
 */
HALx_StatusTypeDef HAL_TIM_PWM_Stop(
    TIMx_HandleTypeDef *htim,
    uint32_t Channel)
{
    htim->Instance->CCER &= ~(1U << Channel);

    return STD_OK;
}

/**
 * @brief Initialize the timer for input capture operation.
 *
 * @param[in,out] htim Pointer to the timer handle.
 *
 * @return Status returned by HAL_TIM_Base_Init().
 */
HALx_StatusTypeDef HAL_TIM_IC_Init(TIMx_HandleTypeDef *htim)
{
    return HAL_TIM_Base_Init(htim);
}

/**
 * @brief Configure a timer channel for input capture operation.
 *
 * @param[in,out] htim    Pointer to the timer handle.
 * @param[in]     sConfig Pointer to the input capture configuration.
 * @param[in]     Channel Timer channel; use a TIM_CHANNEL_x value.
 *
 * @return HAL_TIM_OK if configuration succeeds; otherwise, HAL_TIM_ERROR.
 */
HALx_StatusTypeDef HAL_TIM_IC_ConfigChannel(
    TIMx_HandleTypeDef *htim,
    TIMx_IC_InitTypeDef *sConfig,
    uint32_t Channel)
{
    __IO uint32_t *pCCMR;
    uint8_t shift;

    if ((htim == NULL) || (sConfig == NULL))
    {
        return STD_ERROR;
    }

    pCCMR = (Channel <= TIM_CHANNEL_2) ?
            &htim->Instance->CCMR1 : &htim->Instance->CCMR2;
    shift = ((Channel == TIM_CHANNEL_1) || (Channel == TIM_CHANNEL_3)) ? 0U : 8U;

    *pCCMR &= ~(0xFFU << shift);
    *pCCMR |= ((sConfig->ICSelection | (sConfig->ICFilter << 4U)) << shift);

    htim->Instance->CCER &= ~(0x0BU << Channel);
    htim->Instance->CCER |= (sConfig->ICPolarity << Channel);

    return STD_OK;
}

/**
 * @brief Start input capture on the selected channel in polling mode.
 *
 * @param[in,out] htim    Pointer to the timer handle.
 * @param[in]     Channel Timer channel; use a TIM_CHANNEL_x value.
 *
 * @return HAL_TIM_OK.
 */
HALx_StatusTypeDef HAL_TIM_IC_Start(
    TIMx_HandleTypeDef *htim,
    uint32_t Channel)
{
    htim->Instance->CCER |= (1U << Channel);
    htim->Instance->CR1 |= (1U << 0U);

    return STD_OK;
}

/**
 * @brief Start input capture on the selected channel with interrupts enabled.
 *
 * @param[in,out] htim    Pointer to the timer handle.
 * @param[in]     Channel Timer channel; use a TIM_CHANNEL_x value.
 *
 * @return HAL_TIM_OK.
 */
HALx_StatusTypeDef HAL_TIM_IC_Start_IT(
    TIMx_HandleTypeDef *htim,
    uint32_t Channel)
{
    uint8_t irq;

    /* Enable the capture/compare interrupt for the selected channel. */
    htim->Instance->DIER |= (1U << ((Channel >> 2U) + 1U));
    (void)HAL_TIM_IC_Start(htim, Channel);

    irq = (htim->Instance == TIM1x) ? 27U :
          (htim->Instance == TIM2x) ? 28U :
          (htim->Instance == TIM3x) ? 29U : 30U;
    prv_NVIC_EnableIRQ(irq);

    return STD_OK;
}

/**
 * @brief Read the most recently captured timer counter value.
 *
 * @param[in] htim    Pointer to the timer handle.
 * @param[in] Channel Timer channel; use a TIM_CHANNEL_x value.
 *
 * @return Captured counter value from the selected channel.
 */
uint32_t HAL_TIM_ReadCapturedValue(
    TIMx_HandleTypeDef *htim,
    uint32_t Channel)
{
    return *(&htim->Instance->CCR1 + (Channel >> 2U));
}

/**
 * @brief Initialize the timer encoder interface.
 *
 * @param[in,out] htim    Pointer to the timer handle.
 * @param[in]     sConfig Pointer to the encoder configuration.
 *
 * @return HAL_TIM_OK if initialization succeeds; otherwise, HAL_TIM_ERROR.
 */
HALx_StatusTypeDef HAL_TIM_Encoder_Init(
    TIMx_HandleTypeDef *htim,
    TIMx_Encoder_InitTypeDef *sConfig)
{
    if ((htim == NULL) || (sConfig == NULL))
    {
        return STD_ERROR;
    }

    (void)HAL_TIM_Base_Init(htim);

    htim->Instance->SMCR &= ~(0x07U);
    htim->Instance->SMCR |= sConfig->EncoderMode;

    /* Configure channels 1 and 2 as encoder inputs. */
    htim->Instance->CCMR1 &= ~(0xFFFFU);
    htim->Instance->CCMR1 |=
        (sConfig->IC1Selection | (sConfig->IC1Filter << 4U)) |
        ((sConfig->IC2Selection | (sConfig->IC2Filter << 4U)) << 8U);

    htim->Instance->CCER &= ~(0xAAU);
    htim->Instance->CCER |=
        sConfig->IC1Polarity | (sConfig->IC2Polarity << 4U);

    return STD_OK;
}

/**
 * @brief Start encoder counting using channels 1 and 2.
 *
 * @param[in,out] htim    Pointer to the timer handle.
 * @param[in]     Channel Reserved by this implementation.
 *
 * @return HAL_TIM_OK.
 */
HALx_StatusTypeDef HAL_TIM_Encoder_Start(
    TIMx_HandleTypeDef *htim,
    uint32_t Channel)
{
    (void)Channel;

    /* Enable capture/compare channels 1 and 2. */
    htim->Instance->CCER |= (1U << 0U) | (1U << 4U);
    htim->Instance->CR1 |= (1U << 0U);

    return STD_OK;
}

/**
 * @brief Stop encoder counting using channels 1 and 2.
 *
 * @param[in,out] htim    Pointer to the timer handle.
 * @param[in]     Channel Reserved by this implementation.
 *
 * @return HAL_TIM_OK.
 */
HALx_StatusTypeDef HAL_TIM_Encoder_Stop(
    TIMx_HandleTypeDef *htim,
    uint32_t Channel)
{
    (void)Channel;

    htim->Instance->CCER &= ~((1U << 0U) | (1U << 4U));
    htim->Instance->CR1 &= ~(1U << 0U);

    return STD_OK;
}

/**
 * @brief Generate a blocking delay in microseconds using the selected timer.
 *
 * @param[in,out] htim Pointer to the timer handle.
 * @param[in]     Us   Delay duration in microseconds.
 */
void HAL_TIM_DelayUs(TIMx_HandleTypeDef *htim, uint32_t Us)
{
    htim->Instance->CNT = 0U;

    while (htim->Instance->CNT < Us)
    {
        /* Wait until the requested counter value is reached. */
    }
}

/**
 * @brief Generate a blocking delay in milliseconds using the selected timer.
 *
 * @param[in,out] htim Pointer to the timer handle.
 * @param[in]     Ms   Delay duration in milliseconds.
 */
void HAL_TIM_DelayMs(TIMx_HandleTypeDef *htim, uint32_t Ms)
{
    uint32_t index;

    for (index = 0U; index < Ms; index++)
    {
        HAL_TIM_DelayUs(htim, 1000U);
    }
}

/**
 * @brief Handle timer update and capture/compare interrupt events.
 *
 * @param[in,out] htim Pointer to the timer handle that generated the interrupt.
 */
void HAL_TIM_IRQHandler(TIMx_HandleTypeDef *htim)
{
    uint8_t index;

    /* Process capture/compare interrupt flags for channels 1 to 4. */
    for (index = 0U; index < 4U; index++)
    {
        if (((htim->Instance->SR & (1U << (index + 1U))) != 0U) &&
            ((htim->Instance->DIER & (1U << (index + 1U))) != 0U))
        {
            htim->Instance->SR &= ~(1U << (index + 1U));
            HALx_TIM_IC_CaptureCallback(htim);
        }
    }

    /* Process the timer update interrupt flag. */
    if (((htim->Instance->SR & (1U << 0U)) != 0U) &&
        ((htim->Instance->DIER & (1U << 0U)) != 0U))
    {
        htim->Instance->SR &= ~(1U << 0U);
        HALx_TIM_PeriodElapsedCallback(htim);
    }
}

/**
 * @brief Default weak callback for a timer period-elapsed event.
 *
 * @param[in,out] htim Pointer to the timer handle that generated the event.
 */
void HALx_TIM_PeriodElapsedCallback(TIMx_HandleTypeDef *htim)
{
    (void)htim;
}

/**
 * @brief Default weak callback for a timer input-capture event.
 *
 * @param[in,out] htim Pointer to the timer handle that generated the event.
 */
void HALx_TIM_IC_CaptureCallback(TIMx_HandleTypeDef *htim)
{
    (void)htim;
}

/** @brief Handle the TIM1 update interrupt vector. */
void TIM1_UP_IRQHandler(void)
{
    if (gp_tim_handles[1] != NULL)
    {
        HAL_TIM_IRQHandler(gp_tim_handles[1]);
    }
}

/** @brief Handle the TIM1 capture/compare interrupt vector. */
void TIM1_CC_IRQHandler(void)
{
    if (gp_tim_handles[1] != NULL)
    {
        HAL_TIM_IRQHandler(gp_tim_handles[1]);
    }
}

/** @brief Handle the TIM2 global interrupt vector. */
void TIM2_IRQHandler(void)
{
    if (gp_tim_handles[2] != NULL)
    {
        HAL_TIM_IRQHandler(gp_tim_handles[2]);
    }
}

/** @brief Handle the TIM3 global interrupt vector. */
void TIM3_IRQHandler(void)
{
    if (gp_tim_handles[3] != NULL)
    {
        HAL_TIM_IRQHandler(gp_tim_handles[3]);
    }
}

/** @brief Handle the TIM4 global interrupt vector. */
void TIM4_IRQHandler(void)
{
    if (gp_tim_handles[4] != NULL)
    {
        HAL_TIM_IRQHandler(gp_tim_handles[4]);
    }
}

#ifdef __cplusplus
}
#endif

/*--------------------------------------------------- End Of File -----------------------------------------------------*/
