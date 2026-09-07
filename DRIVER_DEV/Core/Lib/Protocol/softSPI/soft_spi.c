/***********************************************************************************************************************
 * @file    hal_soft_spi.c
 * @brief   Software (Bit-Banging) SPI Driver Implementation for STM32F103xB.
 * @project STM32F103_BareMetal_Drivers
 * @compiler GCC
 **********************************************************************************************************************/

#include "soft_spi.h"

/***********************************************************************************************************************
 *                                            PRIVATE HELPER FUNCTIONS
 **********************************************************************************************************************/

/**
 * @brief   Generates an approximate software delay using NOP instructions.
 * @param   count: Delay loop count.
 * @retval  None.
 *
 * @note    This is not an accurate microsecond delay unless calibrated for CPU clock and compiler optimization.
 */
static void SoftSPI_Delay(uint32_t count)
{
    while (count > 0U)
    {
        __asm volatile ("nop");
        count--;
    }
}

/**
 * @brief   Writes one GPIO pin using BSRR/BRR registers.
 * @param   GPIOx: Pointer to GPIO peripheral.
 * @param   Pin: GPIO pin index 0..15.
 * @param   PinState: 0 = LOW, non-zero = HIGH.
 * @retval  None.
 */
static inline void SoftSPI_WritePin(GPIOx_typedef_t *GPIOx, uint16_t Pin, uint8_t PinState)
{
    if (PinState != 0U)
    {
        GPIOx->BSRR = (1UL << Pin);
    }
    else
    {
        GPIOx->BRR = (1UL << Pin);
    }
}

/**
 * @brief   Reads one GPIO pin using IDR register.
 * @param   GPIOx: Pointer to GPIO peripheral.
 * @param   Pin: GPIO pin index 0..15.
 * @retval  uint8_t: 0 = LOW, 1 = HIGH.
 */
static inline uint8_t SoftSPI_ReadPin(GPIOx_typedef_t *GPIOx, uint16_t Pin)
{
    return ((GPIOx->IDR & (1UL << Pin)) != 0U) ? 1U : 0U;
}

/**
 * @brief   Configures one STM32F1 GPIO pin through CRL/CRH.
 * @param   GPIOx: Pointer to GPIO peripheral.
 * @param   Pin: GPIO pin index 0..15.
 * @param   Mode: 4-bit GPIO configuration value.
 * @retval  None.
 */
static void SoftSPI_ConfigPinMode(GPIOx_typedef_t *GPIOx,
                                  uint16_t Pin,
                                  uint32_t Mode)
{
    volatile uint32_t *pCR;
    uint32_t pinPosition;
    uint32_t bitOffset;

    /* Pin 0..7 use CRL */
    if (Pin < 8U)
    {
        pCR = &GPIOx->CRL;
        pinPosition = Pin;
    }
    /* Pin 8..15 use CRH */
    else
    {
        pCR = &GPIOx->CRH;
        pinPosition = Pin - 8U;
    }

    /* Each GPIO pin uses 4 configuration bits */
    bitOffset = pinPosition * 4U;

    /* Clear old MODE/CNF configuration */
    *pCR &= ~(0x0FUL << bitOffset);

    /* Write new MODE/CNF configuration */
    *pCR |= ((Mode & 0x0FUL) << bitOffset);
}

/**
 * @brief   Validates Software SPI handle and configuration.
 * @param   hspi: Pointer to Software SPI handle.
 * @retval  HALx_StatusTypeDef: STD_OK or STD_ERROR.
 */
static HALx_StatusTypeDef SoftSPI_ValidateHandle(const SoftSPI_HandleTypeDef *hspi)
{
    if (hspi == NULL)
    {
        return STD_ERROR;
    }
    
    if ((hspi->Init.Pins.SckPort  == NULL) ||
        (hspi->Init.Pins.MosiPort == NULL) ||
        (hspi->Init.Pins.MisoPort == NULL) ||
        (hspi->Init.Pins.CsPort   == NULL))
    {
        return STD_ERROR;
    }

    if ((!IS_SOFT_SPI_PIN(hspi->Init.Pins.SckPin))  ||
        (!IS_SOFT_SPI_PIN(hspi->Init.Pins.MosiPin)) ||
        (!IS_SOFT_SPI_PIN(hspi->Init.Pins.MisoPin)) ||
        (!IS_SOFT_SPI_PIN(hspi->Init.Pins.CsPin)))
    {
        return STD_ERROR;
    }

    if ((!IS_SOFT_SPI_CPOL(hspi->Init.CPOL)) ||
        (!IS_SOFT_SPI_CPHA(hspi->Init.CPHA)) ||
        (!IS_SOFT_SPI_FIRSTBIT(hspi->Init.FirstBit)))
    {
        return STD_ERROR;
    }

    return STD_OK;
}

/**
 * @brief   Returns the idle SCK level from CPOL.
 */
static inline uint8_t SoftSPI_GetIdleClock(const SoftSPI_HandleTypeDef *hspi)
{
    return (hspi->Init.CPOL == SOFT_SPI_CPOL_HIGH) ? 1U : 0U;
}

/**
 * @brief   Returns the active SCK level from CPOL.
 */
static inline uint8_t SoftSPI_GetActiveClock(const SoftSPI_HandleTypeDef *hspi)
{
    return (hspi->Init.CPOL == SOFT_SPI_CPOL_HIGH) ? 0U : 1U;
}

/***********************************************************************************************************************
 *                                           PUBLIC DRIVER APIS
 **********************************************************************************************************************/

/**
 * @brief   Initializes Software SPI GPIO pins and default line levels.
 * @param   hspi: Pointer to Software SPI handle.
 * @retval  HALx_StatusTypeDef: STD_OK or STD_ERROR.
 */
HALx_StatusTypeDef SoftSPI_Init(SoftSPI_HandleTypeDef *hspi)
{
    uint8_t sckIdle;

    if (SoftSPI_ValidateHandle(hspi) != STD_OK)
    {
        return STD_ERROR;
    }

    /*
     * GPIO configuration:
     * 0x1 = General purpose output push-pull, max 10 MHz.
     * 0x4 = Floating input.
     *
     * GPIO clocks must already be enabled by RCC/clock driver.
     */
    SoftSPI_ConfigPinMode(hspi->Init.Pins.SckPort,
                          hspi->Init.Pins.SckPin,
                          SOFT_SPI_GPIO_OUTPUT_PP_10MHZ);

    SoftSPI_ConfigPinMode(hspi->Init.Pins.MosiPort,
                          hspi->Init.Pins.MosiPin,
                          SOFT_SPI_GPIO_OUTPUT_PP_10MHZ);

    SoftSPI_ConfigPinMode(hspi->Init.Pins.CsPort,
                          hspi->Init.Pins.CsPin,
                          SOFT_SPI_GPIO_OUTPUT_PP_10MHZ);

    SoftSPI_ConfigPinMode(hspi->Init.Pins.MisoPort,
                          hspi->Init.Pins.MisoPin,
                          SOFT_SPI_GPIO_INPUT_FLOATING);

    /* CS is active LOW, therefore idle state is HIGH. */
    SoftSPI_WritePin(hspi->Init.Pins.CsPort,
                     hspi->Init.Pins.CsPin,
                     1U);

    /* SCK idle state is defined by CPOL. */
    sckIdle = SoftSPI_GetIdleClock(hspi);
    SoftSPI_WritePin(hspi->Init.Pins.SckPort,
                     hspi->Init.Pins.SckPin,
                     sckIdle);

    /* MOSI uses a deterministic LOW level while bus is idle. */
    SoftSPI_WritePin(hspi->Init.Pins.MosiPort,
                     hspi->Init.Pins.MosiPin,
                     0U);

    hspi->State = SOFT_SPI_STATE_READY;

    return STD_OK;
}

/**
 * @brief   Controls the active-low CS/NSS line.
 * @param   hspi: Pointer to Software SPI handle.
 * @param   NewState: ENABLE selects the slave, DISABLE releases the slave.
 * @retval  HALx_StatusTypeDef: STD_OK or STD_ERROR.
 */
HALx_StatusTypeDef SoftSPI_ChipSelect(SoftSPI_HandleTypeDef *hspi, FunctionalState NewState)
{
    if (SoftSPI_ValidateHandle(hspi) != STD_OK)
    {
        return STD_ERROR;
    }

    if (NewState == ENABLE)
    {
        /* Select slave: CS LOW. */
        SoftSPI_WritePin(hspi->Init.Pins.CsPort,
                         hspi->Init.Pins.CsPin,
                         0U);
    }
    else
    {
        /* Release slave: CS HIGH. */
        SoftSPI_WritePin(hspi->Init.Pins.CsPort,
                         hspi->Init.Pins.CsPin,
                         1U);
    }

    return STD_OK;
}

/**
 * @brief   Transmits and receives one byte through Software SPI.
 *
 * Generates 8 software SCK cycles. For each cycle, one bit is
 * transmitted through MOSI and one bit is sampled from MISO.
 * The sampling edge depends on the configured CPHA value.
 *
 * @param   hspi: Pointer to Software SPI handle.
 * @param   txData: Byte to transmit.
 * @retval  uint8_t: Byte received from MISO.
 */
uint8_t SoftSPI_TransmitReceiveByte(SoftSPI_HandleTypeDef *hspi, uint8_t txData)
{
    uint8_t rxData = 0U;
    uint8_t sckIdle;
    uint8_t sckActive;
    uint8_t bitPos;
    uint8_t mosiBit;
    uint8_t i;
    uint32_t delay;

    /* Validate Software SPI handle and configuration. */
    if (SoftSPI_ValidateHandle(hspi) != STD_OK)
    {
        return 0U;
    }

    /* Mark driver as busy during transfer. */
    hspi->State = SOFT_SPI_STATE_BUSY;

    /* Get clock levels from CPOL and configured software delay. */
    sckIdle   = SoftSPI_GetIdleClock(hspi);
    sckActive = SoftSPI_GetActiveClock(hspi);
    delay     = hspi->Init.DelayUs;

    /*
     * Transfer one byte = 8 SCK cycles.
     *
     * CPHA = 0:
     *   MOSI setup -> leading edge -> sample MISO -> trailing edge
     *
     * CPHA = 1:
     *   leading edge -> MOSI setup -> trailing edge -> sample MISO
     */
    for (i = 0U; i < 8U; i++)
    {
        /* Select current bit according to MSB-first or LSB-first mode. */
        bitPos = (hspi->Init.FirstBit == SOFT_SPI_FIRSTBIT_MSB)
                 ? (uint8_t)(7U - i)
                 : i;

        /* Extract one bit from transmit byte for MOSI. */
        mosiBit = (uint8_t)((txData >> bitPos) & 0x01U);

        if (hspi->Init.CPHA == SOFT_SPI_CPHA_1EDGE)
        {
            /*
             * CPHA = 0:
             * Data must be valid before the first clock edge.
             */

            /* Put transmit bit on MOSI. */
            SoftSPI_WritePin(hspi->Init.Pins.MosiPort,
                             hspi->Init.Pins.MosiPin,
                             mosiBit);

            /* Allow MOSI data to become stable before sampling edge. */
            if (delay > 0U)
            {
                SoftSPI_Delay(delay);
            }

            /* Generate leading edge: SCK idle -> active. */
            SoftSPI_WritePin(hspi->Init.Pins.SckPort,
                             hspi->Init.Pins.SckPin,
                             sckActive);

            /* Sample receive bit from MISO on the leading edge. */
            if (SoftSPI_ReadPin(hspi->Init.Pins.MisoPort,
                                hspi->Init.Pins.MisoPin) != 0U)
            {
                rxData |= (uint8_t)(1U << bitPos);
            }

            /* Keep clock active for half of the bit period. */
            if (delay > 0U)
            {
                SoftSPI_Delay(delay);
            }

            /* Generate trailing edge: SCK active -> idle. */
            SoftSPI_WritePin(hspi->Init.Pins.SckPort,
                             hspi->Init.Pins.SckPin,
                             sckIdle);
        }
        else
        {
            /*
             * CPHA = 1:
             * First edge starts the bit period.
             */

            /* Generate leading edge: SCK idle -> active. */
            SoftSPI_WritePin(hspi->Init.Pins.SckPort,
                             hspi->Init.Pins.SckPin,
                             sckActive);

            /* Put transmit bit on MOSI after the leading edge. */
            SoftSPI_WritePin(hspi->Init.Pins.MosiPort,
                             hspi->Init.Pins.MosiPin,
                             mosiBit);

            /* Allow MOSI/MISO data to become stable. */
            if (delay > 0U)
            {
                SoftSPI_Delay(delay);
            }

            /* Generate trailing edge: SCK active -> idle. */
            SoftSPI_WritePin(hspi->Init.Pins.SckPort,
                             hspi->Init.Pins.SckPin,
                             sckIdle);

            /* Sample receive bit from MISO on the trailing edge. */
            if (SoftSPI_ReadPin(hspi->Init.Pins.MisoPort,
                                hspi->Init.Pins.MisoPin) != 0U)
            {
                rxData |= (uint8_t)(1U << bitPos);
            }

            /* Complete remaining half of the bit period. */
            if (delay > 0U)
            {
                SoftSPI_Delay(delay);
            }
        }
    }

    /* Ensure SCK returns to the idle level defined by CPOL. */
    SoftSPI_WritePin(hspi->Init.Pins.SckPort,
                     hspi->Init.Pins.SckPin,
                     sckIdle);

    /* Transfer completed. */
    hspi->State = SOFT_SPI_STATE_READY;

    /* Return the complete received byte. */
    return rxData;
}

/**
 * @brief   Transmits a data buffer through Software SPI.
 * @param   hspi: Pointer to Software SPI handle.
 * @param   pData: Pointer to transmit buffer.
 * @param   Size: Number of bytes to transmit.
 * @retval  HALx_StatusTypeDef: STD_OK or STD_ERROR.
 */
HALx_StatusTypeDef SoftSPI_Transmit(SoftSPI_HandleTypeDef *hspi,
                                    const uint8_t *pData,
                                    uint16_t Size)
{
    uint16_t i;

    if ((SoftSPI_ValidateHandle(hspi) != STD_OK) ||
        (pData == NULL) ||
        (Size == 0U))
    {
        return STD_ERROR;
    }

    for (i = 0U; i < Size; i++)
    {
        /*
         * SPI always receives while transmitting.
         * The received byte is intentionally discarded here.
         */
        (void)SoftSPI_TransmitReceiveByte(hspi, pData[i]);
    }

    return STD_OK;
}

/**
 * @brief   Receives a data buffer through Software SPI by transmitting dummy 0xFF.
 * @param   hspi: Pointer to Software SPI handle.
 * @param   pData: Pointer to receive buffer.
 * @param   Size: Number of bytes to receive.
 * @retval  HALx_StatusTypeDef: STD_OK or STD_ERROR.
 */
HALx_StatusTypeDef SoftSPI_Receive(SoftSPI_HandleTypeDef *hspi,
                                   uint8_t *pData,
                                   uint16_t Size)
{
    uint16_t i;

    if ((SoftSPI_ValidateHandle(hspi) != STD_OK) ||
        (pData == NULL) ||
        (Size == 0U))
    {
        return STD_ERROR;
    }

    for (i = 0U; i < Size; i++)
    {
        /*
         * Master must generate SCK to receive data.
         * Dummy 0xFF provides eight clock cycles without forcing MOSI LOW.
         */
        pData[i] = SoftSPI_TransmitReceiveByte(hspi, 0xFFU);
    }

    return STD_OK;
}

/**
 * @brief   Performs a full-duplex buffer transfer.
 * @param   hspi: Pointer to Software SPI handle.
 * @param   pTxData: Pointer to transmit buffer.
 * @param   pRxData: Pointer to receive buffer.
 * @param   Size: Number of bytes to exchange.
 * @retval  HALx_StatusTypeDef: STD_OK or STD_ERROR.
 */
HALx_StatusTypeDef SoftSPI_TransmitReceive(SoftSPI_HandleTypeDef *hspi,
                                           const uint8_t *pTxData,
                                           uint8_t *pRxData,
                                           uint16_t Size)
{
    uint16_t i;

    if ((SoftSPI_ValidateHandle(hspi) != STD_OK) ||
        (pTxData == NULL) ||
        (pRxData == NULL) ||
        (Size == 0U))
    {
        return STD_ERROR;
    }

    for (i = 0U; i < Size; i++)
    {
        pRxData[i] = SoftSPI_TransmitReceiveByte(hspi, pTxData[i]);
    }

    return STD_OK;
}
