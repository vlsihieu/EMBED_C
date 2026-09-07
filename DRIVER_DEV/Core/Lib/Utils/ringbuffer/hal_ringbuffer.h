/***********************************************************************************************************************
 * Project Name: STM32F103_BareMetal_Drivers
 *
 * File Name: hal_ringbuffer.h
 *
 * Description: Generic Ring Buffer (Circular Buffer) Driver Header for STM32F103xB devices.
 *
 * Compiler: GCC
 *
 * Revision:
 *              Version         Date                Change History
 *              2.0.0           04/09/2026          Optimized SPSC Ring Buffer for USART ISR/Main
 *
 **********************************************************************************************************************/

#ifndef HAL_RINGBUFFER_H
#define HAL_RINGBUFFER_H

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 *                                                    INCLUDES
 **********************************************************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/***********************************************************************************************************************
 *                                                 GLOBAL MACROS
 **********************************************************************************************************************/

#define RING_BUFFER_MIN_SIZE                  (2U)

/***********************************************************************************************************************
 *                                      GLOBAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
 **********************************************************************************************************************/

/**
 * @brief  Ring Buffer operation status.
 */
typedef enum
{
    RING_BUFFER_OK = 0U,
    RING_BUFFER_ERROR,
    RING_BUFFER_FULL,
    RING_BUFFER_EMPTY
} RingBuffer_StatusTypeDef;

/**
 * @brief  Ring Buffer control structure.
 *
 * This implementation follows the Single Producer / Single Consumer model.
 *
 * Typical USART RX usage:
 *
 * USART RX Interrupt -> Producer -> Ring Buffer -> Consumer -> Main/Application
 *
 * Head is modified only by the producer.
 * Tail is modified only by the consumer.
 *
 * The current number of stored bytes is calculated from:
 *
 * Count = Head - Tail
 *
 * @note   If Size is a power-of-two value such as 64, 128, 256 or 512,
 *         the driver uses a bit-mask instead of modulo for faster index calculation.
 */
typedef struct
{
    uint8_t          *pBuffer;      /*!< Pointer to memory buffer. */

    uint16_t          Size;         /*!< Total usable capacity in bytes. */

    uint16_t          Mask;         /*!< Size - 1 when Size is power-of-two, otherwise 0. */

    volatile uint32_t Head;         /*!< Producer-owned write counter. */

    volatile uint32_t Tail;         /*!< Consumer-owned read counter. */

} RingBuffer_HandleTypeDef;

/***********************************************************************************************************************
 *                                           GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

/**
 * @brief  Initialise the Ring Buffer.
 *
 * Attaches the user-provided memory array to the Ring Buffer handle,
 * stores the configured size, prepares the optional power-of-two mask,
 * and resets Head and Tail counters.
 *
 * @param[in,out] pRingBuf Pointer to RingBuffer_HandleTypeDef structure.
 * @param[in]     pBuffer  Pointer to memory array used as Ring Buffer storage.
 * @param[in]     Size     Total usable capacity of the memory buffer in bytes.
 *
 * @retval RING_BUFFER_OK    Ring Buffer initialised successfully.
 * @retval RING_BUFFER_ERROR Invalid parameter.
 */
RingBuffer_StatusTypeDef RingBuffer_Init(RingBuffer_HandleTypeDef *pRingBuf,
                                         uint8_t *pBuffer,
                                         uint16_t Size);

/**
 * @brief  Flush all currently stored data from the Ring Buffer.
 *
 * Moves Tail to the current Head position so all queued data is discarded
 * without modifying the producer-owned Head counter.
 *
 * @param[in,out] pRingBuf Pointer to RingBuffer_HandleTypeDef structure.
 *
 * @retval RING_BUFFER_OK    Buffer flushed successfully.
 * @retval RING_BUFFER_ERROR Invalid Ring Buffer handle.
 */
RingBuffer_StatusTypeDef RingBuffer_Flush(RingBuffer_HandleTypeDef *pRingBuf);

/**
 * @brief  Write one byte to the Ring Buffer.
 *
 * Stores one byte at the current Head position and advances Head.
 * Intended for the single producer, for example a USART RX interrupt.
 *
 * @param[in,out] pRingBuf Pointer to RingBuffer_HandleTypeDef structure.
 * @param[in]     Data     Byte value to write.
 *
 * @retval RING_BUFFER_OK    Byte written successfully.
 * @retval RING_BUFFER_FULL  Ring Buffer is full.
 * @retval RING_BUFFER_ERROR Invalid Ring Buffer handle.
 */
RingBuffer_StatusTypeDef RingBuffer_WriteChar(RingBuffer_HandleTypeDef *pRingBuf,
                                               uint8_t Data);

/**
 * @brief  Read one byte from the Ring Buffer.
 *
 * Reads the oldest stored byte at the current Tail position and advances Tail.
 * Intended for the single consumer, for example the main application.
 *
 * @param[in,out] pRingBuf Pointer to RingBuffer_HandleTypeDef structure.
 * @param[out]    pData    Pointer to destination byte.
 *
 * @retval RING_BUFFER_OK    Byte read successfully.
 * @retval RING_BUFFER_EMPTY Ring Buffer is empty.
 * @retval RING_BUFFER_ERROR Invalid parameter.
 */
RingBuffer_StatusTypeDef RingBuffer_ReadChar(RingBuffer_HandleTypeDef *pRingBuf,
                                              uint8_t *pData);

/**
 * @brief  Write an array of bytes to the Ring Buffer.
 *
 * The function uses an all-or-nothing policy. If the Ring Buffer does not
 * contain enough free space, no data is written.
 *
 * @param[in,out] pRingBuf Pointer to RingBuffer_HandleTypeDef structure.
 * @param[in]     pData    Pointer to source data buffer.
 * @param[in]     Length   Number of bytes to write.
 *
 * @retval RING_BUFFER_OK    All bytes written successfully.
 * @retval RING_BUFFER_FULL  Not enough free space.
 * @retval RING_BUFFER_ERROR Invalid parameter.
 */
RingBuffer_StatusTypeDef RingBuffer_Write(RingBuffer_HandleTypeDef *pRingBuf,
                                           const uint8_t *pData,
                                           uint16_t Length);

/**
 * @brief  Read an array of bytes from the Ring Buffer.
 *
 * The function uses an all-or-nothing policy. If the requested number of bytes
 * is not available, no data is removed from the Ring Buffer.
 *
 * @param[in,out] pRingBuf Pointer to RingBuffer_HandleTypeDef structure.
 * @param[out]    pData    Pointer to destination buffer.
 * @param[in]     Length   Number of bytes to read.
 *
 * @retval RING_BUFFER_OK    All requested bytes read successfully.
 * @retval RING_BUFFER_EMPTY Not enough stored data.
 * @retval RING_BUFFER_ERROR Invalid parameter.
 */
RingBuffer_StatusTypeDef RingBuffer_Read(RingBuffer_HandleTypeDef *pRingBuf,
                                          uint8_t *pData,
                                          uint16_t Length);

/**
 * @brief  Read the next byte without removing it from the Ring Buffer.
 *
 * @param[in]  pRingBuf Pointer to RingBuffer_HandleTypeDef structure.
 * @param[out] pData    Pointer to destination byte.
 *
 * @retval RING_BUFFER_OK    Byte read successfully.
 * @retval RING_BUFFER_EMPTY Ring Buffer is empty.
 * @retval RING_BUFFER_ERROR Invalid parameter.
 */
RingBuffer_StatusTypeDef RingBuffer_Peek(const RingBuffer_HandleTypeDef *pRingBuf,
                                          uint8_t *pData);

/**
 * @brief  Get the current number of stored bytes.
 *
 * @param[in] pRingBuf Pointer to RingBuffer_HandleTypeDef structure.
 *
 * @retval Number of currently stored bytes.
 */
uint16_t RingBuffer_GetCount(const RingBuffer_HandleTypeDef *pRingBuf);

/**
 * @brief  Get the remaining writable space.
 *
 * @param[in] pRingBuf Pointer to RingBuffer_HandleTypeDef structure.
 *
 * @retval Number of free bytes.
 */
uint16_t RingBuffer_GetFreeSpace(const RingBuffer_HandleTypeDef *pRingBuf);

/**
 * @brief  Get the configured Ring Buffer capacity.
 *
 * @param[in] pRingBuf Pointer to RingBuffer_HandleTypeDef structure.
 *
 * @retval Ring Buffer capacity in bytes.
 */
uint16_t RingBuffer_GetCapacity(const RingBuffer_HandleTypeDef *pRingBuf);

/**
 * @brief  Check whether the Ring Buffer is empty.
 *
 * @param[in] pRingBuf Pointer to RingBuffer_HandleTypeDef structure.
 *
 * @retval true  Ring Buffer is empty.
 * @retval false Ring Buffer contains data.
 */
bool RingBuffer_IsEmpty(const RingBuffer_HandleTypeDef *pRingBuf);

/**
 * @brief  Check whether the Ring Buffer is full.
 *
 * @param[in] pRingBuf Pointer to RingBuffer_HandleTypeDef structure.
 *
 * @retval true  Ring Buffer is full.
 * @retval false Ring Buffer still has free space.
 */
bool RingBuffer_IsFull(const RingBuffer_HandleTypeDef *pRingBuf);

#ifdef __cplusplus
}
#endif

#endif /* HAL_RINGBUFFER_H */

/*--------------------------------------------------- End Of File -----------------------------------------------------*/
