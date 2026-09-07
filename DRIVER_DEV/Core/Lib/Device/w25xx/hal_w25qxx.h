/***********************************************************************************************************************
 * Project Name: STM32F103_BareMetal_Drivers
 *
 * File Name: w25qxx.h
 *
 * Description: Complete W25Qxx SPI Flash Memory Driver Header — Extended Instruction Set
 *              and advanced feature set for Winbond W25Qxx series flash devices.
 *
 * Compiler: GCC
 *
 * Revision:
 *              Version         Date                Change History
 *              2.0.0           22/08/2026          Full feature set integration
 *
 **********************************************************************************************************************/

#ifndef W25QXX_H
#define W25QXX_H

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 *                                                  INCLUDES
 **********************************************************************************************************************/

#include "hal_spi.h"
#include "hal_gpio.h"

/***********************************************************************************************************************
 *                                                    MACROS
 **********************************************************************************************************************/

/* --- W25Qxx Complete Instruction Set --- */
#define W25QXX_CMD_WRITE_ENABLE            (0x06U)
#define W25QXX_CMD_WRITE_DISABLE           (0x04U)
#define W25QXX_CMD_READ_STATUS_REG1        (0x05U)
#define W25QXX_CMD_READ_STATUS_REG2        (0x35U)
#define W25QXX_CMD_READ_STATUS_REG3        (0x15U)
#define W25QXX_CMD_WRITE_STATUS_REG1       (0x01U)
#define W25QXX_CMD_WRITE_STATUS_REG2       (0x31U)
#define W25QXX_CMD_WRITE_STATUS_REG3       (0x11U)
#define W25QXX_CMD_READ_DATA               (0x03U)
#define W25QXX_CMD_FAST_READ               (0x0BU)
#define W25QXX_CMD_PAGE_PROGRAM            (0x02U)
#define W25QXX_CMD_SECTOR_ERASE_4K         (0x20U)
#define W25QXX_CMD_BLOCK_ERASE_32K         (0x52U)
#define W25QXX_CMD_BLOCK_ERASE_64K         (0xD8U)
#define W25QXX_CMD_CHIP_ERASE              (0xC7U)
#define W25QXX_CMD_ERASE_SUSPEND           (0x75U)
#define W25QXX_CMD_ERASE_RESUME            (0x7AU)
#define W25QXX_CMD_POWER_DOWN              (0xB9U)
#define W25QXX_CMD_RELEASE_POWER_DOWN      (0xABU)
#define W25QXX_CMD_DEVICE_ID               (0xABU)
#define W25QXX_CMD_JEDEC_ID                (0x9FU)
#define W25QXX_CMD_READ_UNIQUE_ID          (0x4BU)
#define W25QXX_CMD_READ_SFDP               (0x5AU)
#define W25QXX_CMD_ERASE_SECURITY_REG      (0x44U)
#define W25QXX_CMD_PROGRAM_SECURITY_REG    (0x42U)
#define W25QXX_CMD_READ_SECURITY_REG       (0x48U)
#define W25QXX_CMD_ENTER_4BYTE_MODE        (0xB7U)
#define W25QXX_CMD_EXIT_4BYTE_MODE         (0xE9U)

/* --- Status Register Bits --- */
#define W25QXX_SR1_BUSY                    (0x01U << 0U)
#define W25QXX_SR1_WEL                     (0x01U << 1U)
#define W25QXX_SR1_BP0                     (0x01U << 2U)
#define W25QXX_SR1_BP1                     (0x01U << 3U)
#define W25QXX_SR1_BP2                     (0x01U << 4U)
#define W25QXX_SR1_TB                      (0x01U << 5U)
#define W25QXX_SR1_SEC                     (0x01U << 6U)
#define W25QXX_SR1_SRP0                    (0x01U << 7U)

#define W25QXX_SR2_SR_LOCK                 (0x01U << 0U)
#define W25QXX_SR2_QE                      (0x01U << 1U)
#define W25QXX_SR2_LB1                     (0x01U << 3U)
#define W25QXX_SR2_LB2                     (0x01U << 4U)
#define W25QXX_SR2_LB3                     (0x01U << 5U)
#define W25QXX_SR2_CMP                     (0x01U << 6U)
#define W25QXX_SR2_SUS                     (0x01U << 7U)

/* --- Hardware Properties --- */
#define W25QXX_PAGE_SIZE                   (256U)
#define W25QXX_SECTOR_SIZE                 (4096U)
#define W25QXX_BLOCK_SIZE_32K              (32768U)
#define W25QXX_BLOCK_SIZE_64K              (65536U)

/***********************************************************************************************************************
 *                                                    ENUMS
 **********************************************************************************************************************/

typedef enum
{
    W25Q10_ID                          = 0x4011U,
    W25Q20_ID                          = 0x4012U,
    W25Q40_ID                          = 0x4013U,
    W25Q80_ID                          = 0x4014U,
    W25Q16_ID                          = 0x4015U,
    W25Q32_ID                          = 0x4016U,
    W25Q64_ID                          = 0x4017U,
    W25Q128_ID                         = 0x4018U,
    W25Q256_ID                         = 0x4019U,
    W25QXX_UNKNOWN_ID                  = 0xFFFFU
} W25Qxx_ID_t;

typedef enum
{
    W25QXX_OK                          = 0x00U,
    W25QXX_ERROR                       = 0x01U,
    W25QXX_BUSY                        = 0x02U,
    W25QXX_TIMEOUT                     = 0x03U
} W25Qxx_StatusTypeDef_t;

/***********************************************************************************************************************
 *                                                  STRUCTURES
 **********************************************************************************************************************/

typedef struct
{
    SPIx_TypeDef         *SPIx;
    GPIOx_typedef_t     *CS_Port;       
    uint16_t            CS_Pin;         
    W25Qxx_ID_t         ID;             
    uint32_t            SectorCount;    
    uint32_t            CapacityInBytes;
} W25Qxx_HandleTypeDef_t;

/***********************************************************************************************************************
 *                                           GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

/* --- Initialization & Base Controls --- */
W25Qxx_StatusTypeDef_t W25Qxx_Init_t(W25Qxx_HandleTypeDef_t *hw25qxx, SPIx_TypeDef *SPIx, GPIOx_typedef_t *CS_Port, uint16_t CS_Pin);
W25Qxx_StatusTypeDef_t W25Qxx_ReadID_t(W25Qxx_HandleTypeDef_t *hw25qxx, uint32_t *pID);
W25Qxx_StatusTypeDef_t W25Qxx_ReadDeviceID_t(W25Qxx_HandleTypeDef_t *hw25qxx, uint8_t *pDeviceID);
W25Qxx_StatusTypeDef_t W25Qxx_ReadUniqueID_t(W25Qxx_HandleTypeDef_t *hw25qxx, uint8_t *pUniqueID8Bytes);

/* --- Status Registers Control --- */
W25Qxx_StatusTypeDef_t W25Qxx_ReadStatusRegister1_t(W25Qxx_HandleTypeDef_t *hw25qxx, uint8_t *pStatus);
W25Qxx_StatusTypeDef_t W25Qxx_ReadStatusRegister2_t(W25Qxx_HandleTypeDef_t *hw25qxx, uint8_t *pStatus);
W25Qxx_StatusTypeDef_t W25Qxx_ReadStatusRegister3_t(W25Qxx_HandleTypeDef_t *hw25qxx, uint8_t *pStatus);
W25Qxx_StatusTypeDef_t W25Qxx_WriteStatusRegister1_t(W25Qxx_HandleTypeDef_t *hw25qxx, uint8_t Status);

/* --- Erase Operations --- */
W25Qxx_StatusTypeDef_t W25Qxx_EraseSector_t(W25Qxx_HandleTypeDef_t *hw25qxx, uint32_t SectorAddress);
W25Qxx_StatusTypeDef_t W25Qxx_EraseBlock32K_t(W25Qxx_HandleTypeDef_t *hw25qxx, uint32_t BlockAddress);
W25Qxx_StatusTypeDef_t W25Qxx_EraseBlock64K_t(W25Qxx_HandleTypeDef_t *hw25qxx, uint32_t BlockAddress);
W25Qxx_StatusTypeDef_t W25Qxx_EraseChip_t(W25Qxx_HandleTypeDef_t *hw25qxx);
W25Qxx_StatusTypeDef_t W25Qxx_Suspend_t(W25Qxx_HandleTypeDef_t *hw25qxx);
W25Qxx_StatusTypeDef_t W25Qxx_Resume_t(W25Qxx_HandleTypeDef_t *hw25qxx);

/* --- Read / Write Operations --- */
W25Qxx_StatusTypeDef_t W25Qxx_WritePage_t(W25Qxx_HandleTypeDef_t *hw25qxx, uint8_t *pBuffer, uint32_t PageAddress, uint16_t Offset, uint16_t Size);
W25Qxx_StatusTypeDef_t W25Qxx_WriteData_t(W25Qxx_HandleTypeDef_t *hw25qxx, uint8_t *pBuffer, uint32_t WriteAddr, uint32_t Size);
W25Qxx_StatusTypeDef_t W25Qxx_ReadData_t(W25Qxx_HandleTypeDef_t *hw25qxx, uint8_t *pBuffer, uint32_t ReadAddr, uint32_t Size);
W25Qxx_StatusTypeDef_t W25Qxx_FastRead_t(W25Qxx_HandleTypeDef_t *hw25qxx, uint8_t *pBuffer, uint32_t ReadAddr, uint32_t Size);

/* --- Power Management --- */
W25Qxx_StatusTypeDef_t W25Qxx_PowerDown_t(W25Qxx_HandleTypeDef_t *hw25qxx);
W25Qxx_StatusTypeDef_t W25Qxx_ReleasePowerDown_t(W25Qxx_HandleTypeDef_t *hw25qxx);

/* --- Security Registers (OTP Area) --- */
W25Qxx_StatusTypeDef_t W25Qxx_EraseSecurityRegister_t(W25Qxx_HandleTypeDef_t *hw25qxx, uint8_t RegisterNum);
W25Qxx_StatusTypeDef_t W25Qxx_ProgramSecurityRegister_t(W25Qxx_HandleTypeDef_t *hw25qxx, uint8_t RegisterNum, uint8_t AddressOffset, uint8_t *pBuffer, uint16_t Size);
W25Qxx_StatusTypeDef_t W25Qxx_ReadSecurityRegister_t(W25Qxx_HandleTypeDef_t *hw25qxx, uint8_t RegisterNum, uint8_t AddressOffset, uint8_t *pBuffer, uint16_t Size);

/* --- 4-Byte Addressing Mode (For W25Q256+) --- */
W25Qxx_StatusTypeDef_t W25Qxx_Enter4ByteAddressMode_t(W25Qxx_HandleTypeDef_t *hw25qxx);
W25Qxx_StatusTypeDef_t W25Qxx_Exit4ByteAddressMode_t(W25Qxx_HandleTypeDef_t *hw25qxx);

#ifdef __cplusplus
}
#endif

#endif /* W25QXX_H */
