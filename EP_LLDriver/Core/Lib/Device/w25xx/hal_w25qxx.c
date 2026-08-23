/***********************************************************************************************************************
 * Project Name: STM32F103_BareMetal_Drivers
 *
 * File Name: w25qxx.c
 *
 * Description: Complete W25Qxx SPI Flash Memory Driver Implementation.
 *
 * Compiler: GCC
 *
 * Revision:
 *              Version         Date                Change History
 *              2.0.0           22/08/2026          Full feature set implementation
 *
 **********************************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include "w25qxx.h"

#define W25QXX_SPI_TIMEOUT_DEFAULT         (100000U)
#define W25QXX_BUSY_TIMEOUT                (5000000U)

/***********************************************************************************************************************
 *                                           LOCAL FUNCTIONS
 **********************************************************************************************************************/

static void W25Qxx_CS_Select(W25Qxx_HandleTypeDef_t *hw25qxx)
{
    GPIO_WritePin_t(hw25qxx->CS_Port, hw25qxx->CS_Pin, GPIOx_PinState_LOW);
}

static void W25Qxx_CS_Deselect(W25Qxx_HandleTypeDef_t *hw25qxx)
{
    GPIO_WritePin_t(hw25qxx->CS_Port, hw25qxx->CS_Pin, GPIOx_PinState_HIGH);
}

static W25Qxx_StatusTypeDef_t W25Qxx_WriteEnable(W25Qxx_HandleTypeDef_t *hw25qxx)
{
    uint8_t cmd = W25QXX_CMD_WRITE_ENABLE;
    SPIx_StatusTypeDef_t spi_status;

    W25Qxx_CS_Select(hw25qxx);
    spi_status = SPI_Transmit_t(hw25qxx->SPIx, &cmd, 1U, W25QXX_SPI_TIMEOUT_DEFAULT);
    W25Qxx_CS_Deselect(hw25qxx);

    return (spi_status == SPIx_STATUS_OK) ? W25QXX_OK : W25QXX_ERROR;
}

static W25Qxx_StatusTypeDef_t W25Qxx_WaitUntilReady(W25Qxx_HandleTypeDef_t *hw25qxx, uint32_t Timeout)
{
    uint8_t status = 0U;
    uint32_t tick = Timeout;

    while (tick > 0U)
    {
        if (W25Qxx_ReadStatusRegister1_t(hw25qxx, &status) != W25QXX_OK)
        {
            return W25QXX_ERROR;
        }

        if ((status & W25QXX_SR1_BUSY) == 0U)
        {
            return W25QXX_OK;
        }

        tick--;
    }

    return W25QXX_TIMEOUT;
}

/***********************************************************************************************************************
 *                                          GLOBAL FUNCTIONS
 **********************************************************************************************************************/

W25Qxx_StatusTypeDef_t W25Qxx_Init_t(W25Qxx_HandleTypeDef_t *hw25qxx, SPI_TypeDef *SPIx, GPIOx_typedef_t *CS_Port, uint16_t CS_Pin)
{
    uint32_t full_id = 0U;
    GPIOx_InitTypeDef_t GPIO_InitStruct;

    if ((hw25qxx == NULL) || (SPIx == NULL) || (CS_Port == NULL))
    {
        return W25QXX_ERROR;
    }

    hw25qxx->SPIx    = SPIx;
    hw25qxx->CS_Port = CS_Port;
    hw25qxx->CS_Pin  = CS_Pin;

    GPIO_InitStruct.Pin   = CS_Pin;
    GPIO_InitStruct.Mode  = GPIOx_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIOx_SPEED_50MHZ;
    GPIO_InitStruct.Pull  = GPIOx_NOPULL;
    GPIO_Init_t(CS_Port, &GPIO_InitStruct);

    W25Qxx_CS_Deselect(hw25qxx);

    if (W25Qxx_ReadID_t(hw25qxx, &full_id) != W25QXX_OK)
    {
        return W25QXX_ERROR;
    }

    hw25qxx->ID = (W25Qxx_ID_t)(full_id & 0xFFFFU);

    switch (hw25qxx->ID)
    {
        case W25Q16_ID:  hw25qxx->SectorCount = 512U;  break;
        case W25Q32_ID:  hw25qxx->SectorCount = 1024U; break;
        case W25Q64_ID:  hw25qxx->SectorCount = 2048U; break;
        case W25Q128_ID: hw25qxx->SectorCount = 4096U; break;
        case W25Q256_ID: hw25qxx->SectorCount = 8192U; break;
        default:
            hw25qxx->SectorCount = 0U;
            hw25qxx->CapacityInBytes = 0U;
            return W25QXX_ERROR;
    }

    hw25qxx->CapacityInBytes = hw25qxx->SectorCount * W25QXX_SECTOR_SIZE;

    return W25QXX_OK;
}

W25Qxx_StatusTypeDef_t W25Qxx_ReadID_t(W25Qxx_HandleTypeDef_t *hw25qxx, uint32_t *pID)
{
    uint8_t cmd = W25QXX_CMD_JEDEC_ID;
    uint8_t rx_buf[3] = {0U};

    if (pID == NULL) return W25QXX_ERROR;

    W25Qxx_CS_Select(hw25qxx);
    SPI_Transmit_t(hw25qxx->SPIx, &cmd, 1U, W25QXX_SPI_TIMEOUT_DEFAULT);
    SPI_Receive_t(hw25qxx->SPIx, rx_buf, 3U, W25QXX_SPI_TIMEOUT_DEFAULT);
    W25Qxx_CS_Deselect(hw25qxx);

    *pID = ((uint32_t)rx_buf[0] << 16U) | ((uint32_t)rx_buf[1] << 8U) | ((uint32_t)rx_buf[2]);

    return W25QXX_OK;
}

W25Qxx_StatusTypeDef_t W25Qxx_ReadDeviceID_t(W25Qxx_HandleTypeDef_t *hw25qxx, uint8_t *pDeviceID)
{
    uint8_t tx_buf[4] = {W25QXX_CMD_DEVICE_ID, 0x00U, 0x00U, 0x00U};

    if (pDeviceID == NULL) return W25QXX_ERROR;

    W25Qxx_CS_Select(hw25qxx);
    SPI_Transmit_t(hw25qxx->SPIx, tx_buf, 4U, W25QXX_SPI_TIMEOUT_DEFAULT);
    SPI_Receive_t(hw25qxx->SPIx, pDeviceID, 1U, W25QXX_SPI_TIMEOUT_DEFAULT);
    W25Qxx_CS_Deselect(hw25qxx);

    return W25QXX_OK;
}

W25Qxx_StatusTypeDef_t W25Qxx_ReadUniqueID_t(W25Qxx_HandleTypeDef_t *hw25qxx, uint8_t *pUniqueID8Bytes)
{
    uint8_t tx_buf[5] = {W25QXX_CMD_READ_UNIQUE_ID, 0x00U, 0x00U, 0x00U, 0x00U};

    if (pUniqueID8Bytes == NULL) return W25QXX_ERROR;

    W25Qxx_CS_Select(hw25qxx);
    SPI_Transmit_t(hw25qxx->SPIx, tx_buf, 5U, W25QXX_SPI_TIMEOUT_DEFAULT);
    SPI_Receive_t(hw25qxx->SPIx, pUniqueID8Bytes, 8U, W25QXX_SPI_TIMEOUT_DEFAULT);
    W25Qxx_CS_Deselect(hw25qxx);

    return W25QXX_OK;
}

W25Qxx_StatusTypeDef_t W25Qxx_ReadStatusRegister1_t(W25Qxx_HandleTypeDef_t *hw25qxx, uint8_t *pStatus)
{
    uint8_t cmd = W25QXX_CMD_READ_STATUS_REG1;

    if (pStatus == NULL) return W25QXX_ERROR;

    W25Qxx_CS_Select(hw25qxx);
    SPI_Transmit_t(hw25qxx->SPIx, &cmd, 1U, W25QXX_SPI_TIMEOUT_DEFAULT);
    SPI_Receive_t(hw25qxx->SPIx, pStatus, 1U, W25QXX_SPI_TIMEOUT_DEFAULT);
    W25Qxx_CS_Deselect(hw25qxx);

    return W25QXX_OK;
}

W25Qxx_StatusTypeDef_t W25Qxx_ReadStatusRegister2_t(W25Qxx_HandleTypeDef_t *hw25qxx, uint8_t *pStatus)
{
    uint8_t cmd = W25QXX_CMD_READ_STATUS_REG2;

    if (pStatus == NULL) return W25QXX_ERROR;

    W25Qxx_CS_Select(hw25qxx);
    SPI_Transmit_t(hw25qxx->SPIx, &cmd, 1U, W25QXX_SPI_TIMEOUT_DEFAULT);
    SPI_Receive_t(hw25qxx->SPIx, pStatus, 1U, W25QXX_SPI_TIMEOUT_DEFAULT);
    W25Qxx_CS_Deselect(hw25qxx);

    return W25QXX_OK;
}

W25Qxx_StatusTypeDef_t W25Qxx_ReadStatusRegister3_t(W25Qxx_HandleTypeDef_t *hw25qxx, uint8_t *pStatus)
{
    uint8_t cmd = W25QXX_CMD_READ_STATUS_REG3;

    if (pStatus == NULL) return W25QXX_ERROR;

    W25Qxx_CS_Select(hw25qxx);
    SPI_Transmit_t(hw25qxx->SPIx, &cmd, 1U, W25QXX_SPI_TIMEOUT_DEFAULT);
    SPI_Receive_t(hw25qxx->SPIx, pStatus, 1U, W25QXX_SPI_TIMEOUT_DEFAULT);
    W25Qxx_CS_Deselect(hw25qxx);

    return W25QXX_OK;
}

W25Qxx_StatusTypeDef_t W25Qxx_WriteStatusRegister1_t(W25Qxx_HandleTypeDef_t *hw25qxx, uint8_t Status)
{
    uint8_t tx_buf[2] = {W25QXX_CMD_WRITE_STATUS_REG1, Status};

    if (W25Qxx_WriteEnable(hw25qxx) != W25QXX_OK) return W25QXX_ERROR;

    W25Qxx_CS_Select(hw25qxx);
    SPI_Transmit_t(hw25qxx->SPIx, tx_buf, 2U, W25QXX_SPI_TIMEOUT_DEFAULT);
    W25Qxx_CS_Deselect(hw25qxx);

    return W25Qxx_WaitUntilReady(hw25qxx, W25QXX_BUSY_TIMEOUT);
}

W25Qxx_StatusTypeDef_t W25Qxx_EraseSector_t(W25Qxx_HandleTypeDef_t *hw25qxx, uint32_t SectorAddress)
{
    uint8_t tx_buf[4] = {
        W25QXX_CMD_SECTOR_ERASE_4K,
        (uint8_t)((SectorAddress >> 16U) & 0xFFU),
        (uint8_t)((SectorAddress >> 8U) & 0xFFU),
        (uint8_t)(SectorAddress & 0xFFU)
    };

    if (W25Qxx_WriteEnable(hw25qxx) != W25QXX_OK) return W25QXX_ERROR;

    W25Qxx_CS_Select(hw25qxx);
    SPI_Transmit_t(hw25qxx->SPIx, tx_buf, 4U, W25QXX_SPI_TIMEOUT_DEFAULT);
    W25Qxx_CS_Deselect(hw25qxx);

    return W25Qxx_WaitUntilReady(hw25qxx, W25QXX_BUSY_TIMEOUT);
}

W25Qxx_StatusTypeDef_t W25Qxx_EraseBlock32K_t(W25Qxx_HandleTypeDef_t *hw25qxx, uint32_t BlockAddress)
{
    uint8_t tx_buf[4] = {
        W25QXX_CMD_BLOCK_ERASE_32K,
        (uint8_t)((BlockAddress >> 16U) & 0xFFU),
        (uint8_t)((BlockAddress >> 8U) & 0xFFU),
        (uint8_t)(BlockAddress & 0xFFU)
    };

    if (W25Qxx_WriteEnable(hw25qxx) != W25QXX_OK) return W25QXX_ERROR;

    W25Qxx_CS_Select(hw25qxx);
    SPI_Transmit_t(hw25qxx->SPIx, tx_buf, 4U, W25QXX_SPI_TIMEOUT_DEFAULT);
    W25Qxx_CS_Deselect(hw25qxx);

    return W25Qxx_WaitUntilReady(hw25qxx, W25QXX_BUSY_TIMEOUT * 2U);
}

W25Qxx_StatusTypeDef_t W25Qxx_EraseBlock64K_t(W25Qxx_HandleTypeDef_t *hw25qxx, uint32_t BlockAddress)
{
    uint8_t tx_buf[4] = {
        W25QXX_CMD_BLOCK_ERASE_64K,
        (uint8_t)((BlockAddress >> 16U) & 0xFFU),
        (uint8_t)((BlockAddress >> 8U) & 0xFFU),
        (uint8_t)(BlockAddress & 0xFFU)
    };

    if (W25Qxx_WriteEnable(hw25qxx) != W25QXX_OK) return W25QXX_ERROR;

    W25Qxx_CS_Select(hw25qxx);
    SPI_Transmit_t(hw25qxx->SPIx, tx_buf, 4U, W25QXX_SPI_TIMEOUT_DEFAULT);
    W25Qxx_CS_Deselect(hw25qxx);

    return W25Qxx_WaitUntilReady(hw25qxx, W25QXX_BUSY_TIMEOUT * 4U);
}

W25Qxx_StatusTypeDef_t W25Qxx_EraseChip_t(W25Qxx_HandleTypeDef_t *hw25qxx)
{
    uint8_t cmd = W25QXX_CMD_CHIP_ERASE;

    if (W25Qxx_WriteEnable(hw25qxx) != W25QXX_OK) return W25QXX_ERROR;

    W25Qxx_CS_Select(hw25qxx);
    SPI_Transmit_t(hw25qxx->SPIx, &cmd, 1U, W25QXX_SPI_TIMEOUT_DEFAULT);
    W25Qxx_CS_Deselect(hw25qxx);

    return W25Qxx_WaitUntilReady(hw25qxx, W25QXX_BUSY_TIMEOUT * 20U);
}

W25Qxx_StatusTypeDef_t W25Qxx_Suspend_t(W25Qxx_HandleTypeDef_t *hw25qxx)
{
    uint8_t cmd = W25QXX_CMD_ERASE_SUSPEND;

    W25Qxx_CS_Select(hw25qxx);
    SPI_Transmit_t(hw25qxx->SPIx, &cmd, 1U, W25QXX_SPI_TIMEOUT_DEFAULT);
    W25Qxx_CS_Deselect(hw25qxx);

    return W25QXX_OK;
}

W25Qxx_StatusTypeDef_t W25Qxx_Resume_t(W25Qxx_HandleTypeDef_t *hw25qxx)
{
    uint8_t cmd = W25QXX_CMD_ERASE_RESUME;

    W25Qxx_CS_Select(hw25qxx);
    SPI_Transmit_t(hw25qxx->SPIx, &cmd, 1U, W25QXX_SPI_TIMEOUT_DEFAULT);
    W25Qxx_CS_Deselect(hw25qxx);

    return W25QXX_OK;
}

W25Qxx_StatusTypeDef_t W25Qxx_WritePage_t(W25Qxx_HandleTypeDef_t *hw25qxx, uint8_t *pBuffer, uint32_t PageAddress, uint16_t Offset, uint16_t Size)
{
    uint8_t tx_buf[4];
    uint32_t target_addr = PageAddress + Offset;

    if ((pBuffer == NULL) || (Size == 0U) || ((Offset + Size) > W25QXX_PAGE_SIZE)) return W25QXX_ERROR;
    if (W25Qxx_WriteEnable(hw25qxx) != W25QXX_OK) return W25QXX_ERROR;

    tx_buf[0] = W25QXX_CMD_PAGE_PROGRAM;
    tx_buf[1] = (uint8_t)((target_addr >> 16U) & 0xFFU);
    tx_buf[2] = (uint8_t)((target_addr >> 8U) & 0xFFU);
    tx_buf[3] = (uint8_t)(target_addr & 0xFFU);

    W25Qxx_CS_Select(hw25qxx);
    SPI_Transmit_t(hw25qxx->SPIx, tx_buf, 4U, W25QXX_SPI_TIMEOUT_DEFAULT);
    SPI_Transmit_t(hw25qxx->SPIx, pBuffer, Size, W25QXX_SPI_TIMEOUT_DEFAULT);
    W25Qxx_CS_Deselect(hw25qxx);

    return W25Qxx_WaitUntilReady(hw25qxx, W25QXX_BUSY_TIMEOUT);
}

W25Qxx_StatusTypeDef_t W25Qxx_WriteData_t(W25Qxx_HandleTypeDef_t *hw25qxx, uint8_t *pBuffer, uint32_t WriteAddr, uint32_t Size)
{
    uint32_t page_addr = 0U;
    uint16_t page_offset = 0U;
    uint16_t bytes_to_write = 0U;

    if ((pBuffer == NULL) || (Size == 0U)) return W25QXX_ERROR;

    while (Size > 0U)
    {
        page_addr   = WriteAddr & ~(W25QXX_PAGE_SIZE - 1U);
        page_offset = (uint16_t)(WriteAddr & (W25QXX_PAGE_SIZE - 1U));
        bytes_to_write = W25QXX_PAGE_SIZE - page_offset;

        if (bytes_to_write > Size) bytes_to_write = (uint16_t)Size;

        if (W25Qxx_WritePage_t(hw25qxx, pBuffer, page_addr, page_offset, bytes_to_write) != W25QXX_OK)
        {
            return W25QXX_ERROR;
        }

        WriteAddr += bytes_to_write;
        pBuffer   += bytes_to_write;
        Size      -= bytes_to_write;
    }

    return W25QXX_OK;
}

W25Qxx_StatusTypeDef_t W25Qxx_ReadData_t(W25Qxx_HandleTypeDef_t *hw25qxx, uint8_t *pBuffer, uint32_t ReadAddr, uint32_t Size)
{
    uint8_t tx_buf[4] = {
        W25QXX_CMD_READ_DATA,
        (uint8_t)((ReadAddr >> 16U) & 0xFFU),
        (uint8_t)((ReadAddr >> 8U) & 0xFFU),
        (uint8_t)(ReadAddr & 0xFFU)
    };

    if ((pBuffer == NULL) || (Size == 0U)) return W25QXX_ERROR;
    if (W25Qxx_WaitUntilReady(hw25qxx, W25QXX_BUSY_TIMEOUT) != W25QXX_OK) return W25QXX_ERROR;

    W25Qxx_CS_Select(hw25qxx);
    SPI_Transmit_t(hw25qxx->SPIx, tx_buf, 4U, W25QXX_SPI_TIMEOUT_DEFAULT);
    SPI_Receive_t(hw25qxx->SPIx, pBuffer, (uint16_t)Size, W25QXX_SPI_TIMEOUT_DEFAULT);
    W25Qxx_CS_Deselect(hw25qxx);

    return W25QXX_OK;
}

W25Qxx_StatusTypeDef_t W25Qxx_FastRead_t(W25Qxx_HandleTypeDef_t *hw25qxx, uint8_t *pBuffer, uint32_t ReadAddr, uint32_t Size)
{
    uint8_t tx_buf[5] = {
        W25QXX_CMD_FAST_READ,
        (uint8_t)((ReadAddr >> 16U) & 0xFFU),
        (uint8_t)((ReadAddr >> 8U) & 0xFFU),
        (uint8_t)(ReadAddr & 0xFFU),
        0x00U /* Dummy Byte */
    };

    if ((pBuffer == NULL) || (Size == 0U)) return W25QXX_ERROR;
    if (W25Qxx_WaitUntilReady(hw25qxx, W25QXX_BUSY_TIMEOUT) != W25QXX_OK) return W25QXX_ERROR;

    W25Qxx_CS_Select(hw25qxx);
    SPI_Transmit_t(hw25qxx->SPIx, tx_buf, 5U, W25QXX_SPI_TIMEOUT_DEFAULT);
    SPI_Receive_t(hw25qxx->SPIx, pBuffer, (uint16_t)Size, W25QXX_SPI_TIMEOUT_DEFAULT);
    W25Qxx_CS_Deselect(hw25qxx);

    return W25QXX_OK;
}

W25Qxx_StatusTypeDef_t W25Qxx_PowerDown_t(W25Qxx_HandleTypeDef_t *hw25qxx)
{
    uint8_t cmd = W25QXX_CMD_POWER_DOWN;

    W25Qxx_CS_Select(hw25qxx);
    SPI_Transmit_t(hw25qxx->SPIx, &cmd, 1U, W25QXX_SPI_TIMEOUT_DEFAULT);
    W25Qxx_CS_Deselect(hw25qxx);

    return W25QXX_OK;
}

W25Qxx_StatusTypeDef_t W25Qxx_ReleasePowerDown_t(W25Qxx_HandleTypeDef_t *hw25qxx)
{
    uint8_t cmd = W25QXX_CMD_RELEASE_POWER_DOWN;

    W25Qxx_CS_Select(hw25qxx);
    SPI_Transmit_t(hw25qxx->SPIx, &cmd, 1U, W25QXX_SPI_TIMEOUT_DEFAULT);
    W25Qxx_CS_Deselect(hw25qxx);

    return W25QXX_OK;
}

W25Qxx_StatusTypeDef_t W25Qxx_EraseSecurityRegister_t(W25Qxx_HandleTypeDef_t *hw25qxx, uint8_t RegisterNum)
{
    uint8_t tx_buf[4];

    if ((RegisterNum < 1U) || (RegisterNum > 3U)) return W25QXX_ERROR;
    if (W25Qxx_WriteEnable(hw25qxx) != W25QXX_OK) return W25QXX_ERROR;

    tx_buf[0] = W25QXX_CMD_ERASE_SECURITY_REG;
    tx_buf[1] = 0x00U;
    tx_buf[2] = (uint8_t)(RegisterNum << 4U);
    tx_buf[3] = 0x00U;

    W25Qxx_CS_Select(hw25qxx);
    SPI_Transmit_t(hw25qxx->SPIx, tx_buf, 4U, W25QXX_SPI_TIMEOUT_DEFAULT);
    W25Qxx_CS_Deselect(hw25qxx);

    return W25Qxx_WaitUntilReady(hw25qxx, W25QXX_BUSY_TIMEOUT);
}

W25Qxx_StatusTypeDef_t W25Qxx_ProgramSecurityRegister_t(W25Qxx_HandleTypeDef_t *hw25qxx, uint8_t RegisterNum, uint8_t AddressOffset, uint8_t *pBuffer, uint16_t Size)
{
    uint8_t tx_buf[4];

    if ((RegisterNum < 1U) || (RegisterNum > 3U) || (pBuffer == NULL) || (Size == 0U)) return W25QXX_ERROR;
    if (W25Qxx_WriteEnable(hw25qxx) != W25QXX_OK) return W25QXX_ERROR;

    tx_buf[0] = W25QXX_CMD_PROGRAM_SECURITY_REG;
    tx_buf[1] = 0x00U;
    tx_buf[2] = (uint8_t)(RegisterNum << 4U);
    tx_buf[3] = AddressOffset;

    W25Qxx_CS_Select(hw25qxx);
    SPI_Transmit_t(hw25qxx->SPIx, tx_buf, 4U, W25QXX_SPI_TIMEOUT_DEFAULT);
    SPI_Transmit_t(hw25qxx->SPIx, pBuffer, Size, W25QXX_SPI_TIMEOUT_DEFAULT);
    W25Qxx_CS_Deselect(hw25qxx);

    return W25Qxx_WaitUntilReady(hw25qxx, W25QXX_BUSY_TIMEOUT);
}

W25Qxx_StatusTypeDef_t W25Qxx_ReadSecurityRegister_t(W25Qxx_HandleTypeDef_t *hw25qxx, uint8_t RegisterNum, uint8_t AddressOffset, uint8_t *pBuffer, uint16_t Size)
{
    uint8_t tx_buf[5];

    if ((RegisterNum < 1U) || (RegisterNum > 3U) || (pBuffer == NULL) || (Size == 0U)) return W25QXX_ERROR;

    tx_buf[0] = W25QXX_CMD_READ_SECURITY_REG;
    tx_buf[1] = 0x00U;
    tx_buf[2] = (uint8_t)(RegisterNum << 4U);
    tx_buf[3] = AddressOffset;
    tx_buf[4] = 0x00U; /* Dummy Byte */

    W25Qxx_CS_Select(hw25qxx);
    SPI_Transmit_t(hw25qxx->SPIx, tx_buf, 5U, W25QXX_SPI_TIMEOUT_DEFAULT);
    SPI_Receive_t(hw25qxx->SPIx, pBuffer, Size, W25QXX_SPI_TIMEOUT_DEFAULT);
    W25Qxx_CS_Deselect(hw25qxx);

    return W25QXX_OK;
}

W25Qxx_StatusTypeDef_t W25Qxx_Enter4ByteAddressMode_t(W25Qxx_HandleTypeDef_t *hw25qxx)
{
    uint8_t cmd = W25QXX_CMD_ENTER_4BYTE_MODE;

    if (W25Qxx_WriteEnable(hw25qxx) != W25QXX_OK) return W25QXX_ERROR;

    W25Qxx_CS_Select(hw25qxx);
    SPI_Transmit_t(hw25qxx->SPIx, &cmd, 1U, W25QXX_SPI_TIMEOUT_DEFAULT);
    W25Qxx_CS_Deselect(hw25qxx);

    return W25QXX_OK;
}

W25Qxx_StatusTypeDef_t W25Qxx_Exit4ByteAddressMode_t(W25Qxx_HandleTypeDef_t *hw25qxx)
{
    uint8_t cmd = W25QXX_CMD_EXIT_4BYTE_MODE;

    if (W25Qxx_WriteEnable(hw25qxx) != W25QXX_OK) return W25QXX_ERROR;

    W25Qxx_CS_Select(hw25qxx);
    SPI_Transmit_t(hw25qxx->SPIx, &cmd, 1U, W25QXX_SPI_TIMEOUT_DEFAULT);
    W25Qxx_CS_Deselect(hw25qxx);

    return W25QXX_OK;
}

#ifdef __cplusplus
}
#endif