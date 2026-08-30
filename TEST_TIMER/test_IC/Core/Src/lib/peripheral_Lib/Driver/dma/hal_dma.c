/***********************************************************************************************************************
 * Project Name: STM32F103_BareMetal_Drivers
 * 
 * File Name:    hal_dma.c
 *
 * Description:  HAL DMA Driver - Implementation for STM32F103xB devices.
 * 
 * Compiler:     GCC
 *
 * Revision:
 *               Version        Date                Change History
 *               1.0.0          21/08/2026          Initial release for STM32F103xB DMA Module
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                                 INCLUDES
 **********************************************************************************************************************/

#include "../../../peripheral_Lib/Driver/dma/hal_dma.h"

/***********************************************************************************************************************
 *                                           SOURCE FILE VERSION
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                            FILE VERSION CHECK
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                                 LOCAL MACROS
 **********************************************************************************************************************/

/* NVIC Interrupt Set Enable Register */
#define NVIC_ISER0                      (*(volatile uint32_t *)(0xE000E100U))

/***********************************************************************************************************************
 *                                                    EXTERN
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

/* Static array storing pointers to active handles for ISR mapping (Channels 1-7) */
static DMAx_HandleTypeDef *gp_dma_handles[7] = {NULL};

/***********************************************************************************************************************
 *                                                GLOBAL VARIABLES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                            LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

static uint8_t prv_DMA_GetChannelIndex(const DMAx_Channel_TypeDef_t *Instance);
static void prv_NVIC_EnableIRQ(uint8_t irq);

/***********************************************************************************************************************
 *                                                 LOCAL FUNCTION
 **********************************************************************************************************************/

/**
  * @brief  Maps channel register instance pointer to its internal 0-based array index.
  * @param  Instance: Pointer to DMA Channel register structure.
  * @retval Channel index (0 for Channel 1 to 6 for Channel 7), or 0xFF if invalid.
  */
static uint8_t prv_DMA_GetChannelIndex(const DMAx_Channel_TypeDef_t *Instance)
{
    if (Instance == DMA1x_Channel1) return 0U;
    if (Instance == DMA1x_Channel2) return 1U;
    if (Instance == DMA1x_Channel3) return 2U;
    if (Instance == DMA1x_Channel4) return 3U;
    if (Instance == DMA1x_Channel5) return 4U;
    if (Instance == DMA1x_Channel6) return 5U;
    if (Instance == DMA1x_Channel7) return 6U;
    return 0xFFU;
}

/**
  * @brief  Enables a specific interrupt vector in the NVIC ISER0 register.
  * @param  irq: IRQ number to enable.
  * @retval None
  */
static void prv_NVIC_EnableIRQ(uint8_t irq) 
{
    NVIC_ISER0 |= (1U << irq);
}

/***********************************************************************************************************************
 *                                                 GLOBAL FUNCTION
 **********************************************************************************************************************/

/**
  * @brief  Initializes the DMA Channel according to the specified parameters in DMAx_InitTypeDef.
  * @param  hdma: Pointer to a DMAx_HandleTypeDef structure containing configuration information.
  * @retval HALx_StatusTypeDef: STD_OK if initialization succeeded, STD_ERROR otherwise.
  */
HALx_StatusTypeDef DMAx_Init(DMAx_HandleTypeDef *hdma) 
{
    uint8_t ch_idx;

    if (hdma == NULL || hdma->Instance == NULL) return STD_ERROR;

    /* Enable DMA1 Clock */
    HAL_CLOCK_AHB_Peripheral_Enable(RCCx_AHBENR_Index_DMA1);

    ch_idx = prv_DMA_GetChannelIndex(hdma->Instance);
    if (ch_idx > 6U) return STD_ERROR;

    gp_dma_handles[ch_idx] = hdma;

    /* Disable Channel before configuration */
    hdma->Instance->CCR &= ~(1U << 0U);

    /* Configure CCR Register */
    uint32_t ccr = 0U;
    ccr |= hdma->Init.Direction;
    ccr |= hdma->Init.PeriphInc;
    ccr |= hdma->Init.MemInc;
    ccr |= hdma->Init.PeriphDataAlignment;
    ccr |= hdma->Init.MemDataAlignment;
    ccr |= hdma->Init.Mode;
    ccr |= hdma->Init.Priority;

    hdma->Instance->CCR = ccr;

    return STD_OK;
}

/**
  * @brief  De-initializes the DMA Channel registers to their default reset values.
  * @param  hdma: Pointer to a DMAx_HandleTypeDef structure containing DMA configuration.
  * @retval HALx_StatusTypeDef: STD_OK if de-initialization succeeded, STD_ERROR otherwise.
  */
HALx_StatusTypeDef DMAx_DeInit(DMAx_HandleTypeDef *hdma)
{
    if ((hdma == NULL) || (hdma->Instance == NULL))
    {
        return STD_ERROR;
    }

    /* Disable Channel */
    hdma->Instance->CCR &= ~DMAx_CCR_EN;

    /* Reset Channel Registers */
    hdma->Instance->CCR = 0U;
    hdma->Instance->CNDTR = 0U;
    hdma->Instance->CPAR = 0U;
    hdma->Instance->CMAR = 0U;

    /* Unregister global handle pointer */
    uint8_t ch_idx = prv_DMA_GetChannelIndex(hdma->Instance);
    if (ch_idx <= 6U) 
    {
        gp_dma_handles[ch_idx] = NULL;
    }

    return STD_OK;
}

/**
  * @brief  Starts the DMA transfer in Polling/Normal mode (without Interrupts).
  * @param  hdma: Pointer to DMA handle structure.
  * @param  SrcAddress: Memory or Peripheral Source address.
  * @param  DstAddress: Memory or Peripheral Destination address.
  * @param  DataLength: The total length of data to be transferred.
  * @retval HALx_StatusTypeDef: STD_OK if transfer started successfully, STD_ERROR otherwise.
  */
HALx_StatusTypeDef DMAx_Start(DMAx_HandleTypeDef *hdma, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength)
{
    if ((hdma == NULL) || (DataLength == 0U)) 
    {
        return STD_ERROR;
    }

    /* Disable Channel */
    hdma->Instance->CCR &= ~DMAx_CCR_EN;

    /* Configure Data Counter */
    hdma->Instance->CNDTR = DataLength;

    /* Set Addresses based on Transfer Direction */
    if ((hdma->Init.Direction == DMAx_DIRECTION_MEM_TO_PERIPH) || 
        (hdma->Init.Direction == DMAx_DIRECTION_MEM_TO_MEM)) 
    {
        hdma->Instance->CMAR = SrcAddress;
        hdma->Instance->CPAR = DstAddress;
    } 
    else 
    {
        hdma->Instance->CPAR = SrcAddress;
        hdma->Instance->CMAR = DstAddress;
    }

    /* Enable Channel */
    hdma->Instance->CCR |= DMAx_CCR_EN;

    return STD_OK;
}

/**
  * @brief  Starts the DMA transfer in Interrupt mode.
  * @param  hdma: Pointer to DMA handle structure.
  * @param  SrcAddress: Memory or Peripheral Source address.
  * @param  DstAddress: Memory or Peripheral Destination address.
  * @param  DataLength: The total length of data to be transferred.
  * @retval HALx_StatusTypeDef: STD_OK if start succeeded, STD_ERROR otherwise.
  */
HALx_StatusTypeDef DMAx_Start_IT(DMAx_HandleTypeDef *hdma, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength)
{
    if ((hdma == NULL) || (DataLength == 0U)) 
    {
        return STD_ERROR;
    }

    uint8_t ch_idx = prv_DMA_GetChannelIndex(hdma->Instance);
    if (ch_idx > 6U) 
    {
        return STD_ERROR;
    }

    /* Disable Channel */
    hdma->Instance->CCR &= ~DMAx_CCR_EN;

    /* Configure Data Counter */
    hdma->Instance->CNDTR = DataLength;

    /* Set Addresses based on Transfer Direction */
    if ((hdma->Init.Direction == DMAx_DIRECTION_MEM_TO_PERIPH) || 
        (hdma->Init.Direction == DMAx_DIRECTION_MEM_TO_MEM)) 
    {
        hdma->Instance->CMAR = SrcAddress;
        hdma->Instance->CPAR = DstAddress;
    } 
    else 
    {
        hdma->Instance->CPAR = SrcAddress;
        hdma->Instance->CMAR = DstAddress;
    }

    /* Enable Interrupts (Transfer Complete & Transfer Error) */
    hdma->Instance->CCR |= (DMAx_IT_TC | DMAx_IT_TE);
    if (hdma->XferHalfCpltCallback != NULL) 
    {
        hdma->Instance->CCR |= DMAx_IT_HT;
    }

    /* Enable Interrupt in NVIC (DMA1_Channel1 IRQn is 11) */
    prv_NVIC_EnableIRQ(11U + ch_idx);

    /* Enable Channel */
    hdma->Instance->CCR |= DMAx_CCR_EN;

    return STD_OK;
}

/**
  * @brief  Stops the DMA transfer by disabling the channel.
  * @param  hdma: Pointer to DMA handle structure.
  * @retval HALx_StatusTypeDef: STD_OK if stop succeeded, STD_ERROR if handle is NULL.
  */
HALx_StatusTypeDef DMAx_Stop(DMAx_HandleTypeDef *hdma) 
{
    if (hdma == NULL) 
    {
        return STD_ERROR;
    }

    hdma->Instance->CCR &= ~DMAx_CCR_EN;
    return STD_OK;
}

/**
  * @brief  Stops the DMA transfer and disables all associated interrupts.
  * @param  hdma: Pointer to DMA handle structure.
  * @retval HALx_StatusTypeDef: STD_OK if stop succeeded, STD_ERROR if handle is NULL.
  */
HALx_StatusTypeDef DMAx_Stop_IT(DMAx_HandleTypeDef *hdma) 
{
    if (hdma == NULL) 
    {
        return STD_ERROR;
    }

    /* Disable Interrupt Flags & Channel */
    hdma->Instance->CCR &= ~(DMAx_IT_TC | DMAx_IT_HT | DMAx_IT_TE | DMAx_CCR_EN);
    return STD_OK;
}

/**
  * @brief  Returns the number of remaining data units in current DMA transfer counter.
  * @param  hdma: Pointer to DMA handle structure.
  * @retval uint16_t: Remaining data counter value (CNDTR), or STD_ERROR if handle is NULL.
  */
uint16_t DMAx_GetCounter(DMAx_HandleTypeDef *hdma) 
{
    if (hdma == NULL) 
    {
        return STD_ERROR;
    }

    return (uint16_t)(hdma->Instance->CNDTR);
}

/**
  * @brief  Generic DMA Interrupt Request Handler. Evaluates flags and executes registered callbacks.
  * @param  hdma: Pointer to DMA handle structure.
  * @retval None
  */
void DMAx_IRQHandler(DMAx_HandleTypeDef *hdma) 
{
    uint8_t ch_idx = prv_DMA_GetChannelIndex(hdma->Instance);
    if (ch_idx > 6U) 
    {
        return;
    }

    uint32_t shift = ch_idx * 4U;
    uint32_t isr = DMA1->ISR;

    /* Transfer Complete Interrupt Handling */
    if ((isr & (1U << (shift + 1U))) != 0U) 
    {
        DMA1->IFCR = (1U << (shift + 1U)); /* Clear GIF & TC Flag */
        
        if (hdma->Init.Mode != DMAx_MODE_CIRCULAR) 
        {
            hdma->Instance->CCR &= ~DMAx_CCR_EN; /* Auto disable in Normal mode */
        }
        
        if (hdma->XferCpltCallback != NULL)
        {
            hdma->XferCpltCallback(hdma);
        }
    }

    /* Half Transfer Interrupt Handling */
    if ((isr & (1U << (shift + 2U))) != 0U) 
    {
        DMA1->IFCR = (1U << (shift + 2U)); /* Clear HT Flag */
        
        if (hdma->XferHalfCpltCallback != NULL) 
        {
            hdma->XferHalfCpltCallback(hdma);
        }
    }

    /* Transfer Error Interrupt Handling */
    if ((isr & (1U << (shift + 3U))) != 0U) 
    {
        DMA1->IFCR = (1U << (shift + 3U)); /* Clear TE Flag */
        hdma->Instance->CCR &= ~DMAx_CCR_EN; /* Disable channel on error */
        
        if (hdma->XferErrorCallback != NULL) 
        {
            hdma->XferErrorCallback(hdma);
        }
    }
}

/**
  * @brief ISR Vector Handler for DMA1 Channel 1.
  */
void DMA1_Channel1_IRQHandler(void) 
{ 
    if (gp_dma_handles[0]) DMAx_IRQHandler(gp_dma_handles[0]); 
}

/**
  * @brief ISR Vector Handler for DMA1 Channel 2.
  */
void DMA1_Channel2_IRQHandler(void) 
{ 
    if (gp_dma_handles[1]) DMAx_IRQHandler(gp_dma_handles[1]); 
}

/**
  * @brief ISR Vector Handler for DMA1 Channel 3.
  */
void DMA1_Channel3_IRQHandler(void) 
{ 
    if (gp_dma_handles[2]) DMAx_IRQHandler(gp_dma_handles[2]); 
}

/**
  * @brief ISR Vector Handler for DMA1 Channel 4.
  */
void DMA1_Channel4_IRQHandler(void) 
{ 
    if (gp_dma_handles[3]) DMAx_IRQHandler(gp_dma_handles[3]); 
}

/**
  * @brief ISR Vector Handler for DMA1 Channel 5.
  */
void DMA1_Channel5_IRQHandler(void) 
{ 
    if (gp_dma_handles[4]) DMAx_IRQHandler(gp_dma_handles[4]); 
}

/**
  * @brief ISR Vector Handler for DMA1 Channel 6.
  */
void DMA1_Channel6_IRQHandler(void) 
{ 
    if (gp_dma_handles[5]) DMAx_IRQHandler(gp_dma_handles[5]); 
}

/**
  * @brief ISR Vector Handler for DMA1 Channel 7.
  */
void DMA1_Channel7_IRQHandler(void) 
{ 
    if (gp_dma_handles[6]) DMAx_IRQHandler(gp_dma_handles[6]); 
}

/*--------------------------------------------------- End Of File -----------------------------------------------------*/
