/***********************************************************************************************************************
 * Project Name: STM32F103_BareMetal_Drivers
 *
 * File Name: ringbuffer.c
 *
 * Description: Generic Ring Buffer (Circular Buffer) - Implementation for STM32F103xB devices.
 *
 * Compiler: GCC
 *
 * Revision:
 *              Version         Date                Change History
 *              2.0.0           04/09/2026          Optimized SPSC Ring Buffer for USART ISR/Main
 *
 **********************************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 *                                                    INCLUDES
 **********************************************************************************************************************/

#include "hal_ringbuffer.h"

/***********************************************************************************************************************
 *                                               SOURCE FILE VERSION
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                                FILE VERSION CHECK
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                                 LOCAL MACROS
 **********************************************************************************************************************/

/*
 * Compiler memory barrier.
 *
 * Prevents the compiler from moving normal memory accesses across
 * Head/Tail publication in the Single Producer / Single Consumer model.
 */
#define RING_BUFFER_BARRIER()                  __asm volatile ("" ::: "memory")

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

/***********************************************************************************************************************
 *                                           LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                                 LOCAL FUNCTION
 **********************************************************************************************************************/

/**
 * @brief  Check whether a value is a power-of-two.
 *
 * Power-of-two sizes such as 8, 16, 32, 64, 128 allow the Ring Buffer
 * to calculate the circular index using a fast bit-mask operation
 * instead of the modulo (%) operator.
 *
 * @param[in] Value Input value to check.
 *
 * @retval true  Value is a power-of-two.
 * @retval false Value is not a power-of-two.
 */
static inline bool RingBuffer_IsPowerOfTwo(uint16_t Value)
{
    return ((Value != 0U) &&
            ((Value & (uint16_t)(Value - 1U)) == 0U));
}

/**
 * @brief  Validate the Ring Buffer handle.
 *
 * Checks the control structure, storage pointer and configured size.
 *
 * @param[in] pRingBuf Pointer to RingBuffer_HandleTypeDef structure.
 *
 * @retval true  Ring Buffer handle is valid.
 * @retval false Ring Buffer handle is invalid.
 */
static inline bool RingBuffer_IsValid(const RingBuffer_HandleTypeDef *pRingBuf)
{
    return ((pRingBuf != NULL) &&
            (pRingBuf->pBuffer != NULL) &&
            (pRingBuf->Size >= RING_BUFFER_MIN_SIZE));
}

/**
 * @brief  Convert a monotonic counter to the physical memory index.
 *
 * For power-of-two sizes, a bit-mask is used:
 *
 * Index = Counter & (Size - 1)
 *
 * For other sizes, modulo is used:
 *
 * Index = Counter % Size
 *
 * @param[in] pRingBuf Pointer to RingBuffer_HandleTypeDef structure.
 * @param[in] Counter  Head or Tail monotonic counter.
 *
 * @retval Physical array index.
 */
static inline uint16_t RingBuffer_GetIndex(const RingBuffer_HandleTypeDef *pRingBuf,
                                            uint32_t Counter)
{
    if (pRingBuf->Mask != 0U)
    {
        return (uint16_t)(Counter & (uint32_t)pRingBuf->Mask);
    }

    return (uint16_t)(Counter % (uint32_t)pRingBuf->Size);
}

/**
 * @brief  Get the raw number of currently stored bytes.
 *
 * Head and Tail are monotonic unsigned counters. Therefore current stored data
 * can be calculated directly from their difference.
 *
 * @param[in] pRingBuf Pointer to RingBuffer_HandleTypeDef structure.
 *
 * @retval Number of currently queued bytes.
 */
static inline uint32_t RingBuffer_GetCountRaw(const RingBuffer_HandleTypeDef *pRingBuf)
{
    return (uint32_t)(pRingBuf->Head - pRingBuf->Tail);
}

/***********************************************************************************************************************
 *                                                GLOBAL FUNCTION
 **********************************************************************************************************************/

/**
 * @brief  Initialise the Ring Buffer.
 *
 * Attaches the user-provided memory array to the Ring Buffer handle,
 * configures the total capacity, prepares an optional power-of-two mask,
 * and resets both Head and Tail counters.
 *
 * @param[in,out] pRingBuf Pointer to RingBuffer_HandleTypeDef structure.
 * @param[in]     pBuffer  Pointer to memory array used as Ring Buffer storage.
 * @param[in]     Size     Total usable capacity in bytes.
 *
 * @retval RING_BUFFER_OK    Ring Buffer initialised successfully.
 * @retval RING_BUFFER_ERROR Invalid parameter.
 */
RingBuffer_StatusTypeDef RingBuffer_Init(RingBuffer_HandleTypeDef *pRingBuf,
                                         uint8_t *pBuffer,
                                         uint16_t Size)
{
    if ((pRingBuf == NULL) ||
        (pBuffer == NULL) ||
        (Size < RING_BUFFER_MIN_SIZE))
    {
        return RING_BUFFER_ERROR;
    }

    /* Attach the memory array and save the configured capacity. */
    pRingBuf->pBuffer = pBuffer;
    pRingBuf->Size    = Size;

    /* Prepare fast index mask when Size is power-of-two. */
    pRingBuf->Mask = RingBuffer_IsPowerOfTwo(Size) ?
                     (uint16_t)(Size - 1U) :
                     0U;

    /* Reset producer and consumer counters. */
    pRingBuf->Head = 0U;
    pRingBuf->Tail = 0U;

    return RING_BUFFER_OK;
}

/**
 * @brief  Flush all currently stored data from the Ring Buffer.
 *
 * Moves Tail to the current Head position. This discards all queued data
 * without changing the producer-owned Head counter.
 *
 * @param[in,out] pRingBuf Pointer to RingBuffer_HandleTypeDef structure.
 *
 * @retval RING_BUFFER_OK    Ring Buffer flushed successfully.
 * @retval RING_BUFFER_ERROR Invalid Ring Buffer handle.
 */
RingBuffer_StatusTypeDef RingBuffer_Flush(RingBuffer_HandleTypeDef *pRingBuf)
{
    uint32_t head;

    if (!RingBuffer_IsValid(pRingBuf))
    {
        return RING_BUFFER_ERROR;
    }

    /* Read the latest producer position. */
    head = pRingBuf->Head;

    RING_BUFFER_BARRIER();

    /* Discard all queued bytes. */
    pRingBuf->Tail = head;

    return RING_BUFFER_OK;
}

/**
 * @brief  Write one byte to the Ring Buffer.
 *
 * Stores one byte at the current Head position and then publishes the new Head.
 * This function is intended to be called by the single producer.
 *
 * Typical USART RX use:
 *
 * USART RX IRQ -> RingBuffer_WriteChar()
 *
 * @param[in,out] pRingBuf Pointer to RingBuffer_HandleTypeDef structure.
 * @param[in]     Data     Byte value to write.
 *
 * @retval RING_BUFFER_OK    Byte written successfully.
 * @retval RING_BUFFER_FULL  Ring Buffer is full.
 * @retval RING_BUFFER_ERROR Invalid Ring Buffer handle.
 */
RingBuffer_StatusTypeDef RingBuffer_WriteChar(RingBuffer_HandleTypeDef *pRingBuf,
                                               uint8_t Data)
{
    uint32_t head;
    uint32_t tail;
    uint16_t index;

    if (!RingBuffer_IsValid(pRingBuf))
    {
        return RING_BUFFER_ERROR;
    }

    /* Snapshot producer and consumer positions. */
    head = pRingBuf->Head;
    tail = pRingBuf->Tail;

    /* Buffer is full when stored data count reaches configured capacity. */
    if ((uint32_t)(head - tail) >= (uint32_t)pRingBuf->Size)
    {
        return RING_BUFFER_FULL;
    }

    /* Convert Head sequence counter to physical array position. */
    index = RingBuffer_GetIndex(pRingBuf, head);

    /* Store the byte first. */
    pRingBuf->pBuffer[index] = Data;

    RING_BUFFER_BARRIER();

    /* Publish the newly written byte to the consumer. */
    pRingBuf->Head = head + 1U;

    return RING_BUFFER_OK;
}

/**
 * @brief  Read one byte from the Ring Buffer.
 *
 * Reads the oldest queued byte from the current Tail position and then
 * advances Tail. This function is intended to be called by the single consumer.
 *
 * Typical USART RX use:
 *
 * Main/Application -> RingBuffer_ReadChar()
 *
 * @param[in,out] pRingBuf Pointer to RingBuffer_HandleTypeDef structure.
 * @param[out]    pData    Pointer to destination byte.
 *
 * @retval RING_BUFFER_OK    Byte read successfully.
 * @retval RING_BUFFER_EMPTY Ring Buffer is empty.
 * @retval RING_BUFFER_ERROR Invalid parameter.
 */
RingBuffer_StatusTypeDef RingBuffer_ReadChar(RingBuffer_HandleTypeDef *pRingBuf,
                                              uint8_t *pData)
{
    uint32_t head;
    uint32_t tail;
    uint16_t index;

    if ((!RingBuffer_IsValid(pRingBuf)) || (pData == NULL))
    {
        return RING_BUFFER_ERROR;
    }

    /* Snapshot consumer and producer positions. */
    tail = pRingBuf->Tail;
    head = pRingBuf->Head;

    /* Head equals Tail when no unread data exists. */
    if (head == tail)
    {
        return RING_BUFFER_EMPTY;
    }

    RING_BUFFER_BARRIER();

    /* Convert Tail sequence counter to physical array position. */
    index = RingBuffer_GetIndex(pRingBuf, tail);

    /* Read the oldest queued byte. */
    *pData = pRingBuf->pBuffer[index];

    RING_BUFFER_BARRIER();

    /* Release the consumed position back to the producer. */
    pRingBuf->Tail = tail + 1U;

    return RING_BUFFER_OK;
}

/**
 * @brief  Write an array of bytes to the Ring Buffer.
 *
 * Uses all-or-nothing behaviour. If the requested number of bytes does not
 * fit in the remaining free space, no data is written.
 *
 * @param[in,out] pRingBuf Pointer to RingBuffer_HandleTypeDef structure.
 * @param[in]     pData    Pointer to source data buffer.
 * @param[in]     Length   Number of bytes to write.
 *
 * @retval RING_BUFFER_OK    Data written successfully.
 * @retval RING_BUFFER_FULL  Not enough free space.
 * @retval RING_BUFFER_ERROR Invalid parameter.
 */
RingBuffer_StatusTypeDef RingBuffer_Write(RingBuffer_HandleTypeDef *pRingBuf,
                                           const uint8_t *pData,
                                           uint16_t Length)
{
    uint32_t head;
    uint32_t tail;
    uint32_t free_space;
    uint16_t i;

    if ((!RingBuffer_IsValid(pRingBuf)) || (pData == NULL) || (Length == 0U))
    {
        return RING_BUFFER_ERROR;
    }

    /* Snapshot Head and Tail once. */
    head = pRingBuf->Head;
    tail = pRingBuf->Tail;

    /* Calculate remaining writable capacity. */
    free_space = (uint32_t)pRingBuf->Size -
                 (uint32_t)(head - tail);

    if (free_space < (uint32_t)Length)
    {
        return RING_BUFFER_FULL;
    }

    /* Copy data using a local Head to reduce volatile accesses. */
    for (i = 0U; i < Length; i++)
    {
        pRingBuf->pBuffer[RingBuffer_GetIndex(pRingBuf, head)] = pData[i];
        head++;
    }

    RING_BUFFER_BARRIER();

    /* Publish the complete block at one time. */
    pRingBuf->Head = head;

    return RING_BUFFER_OK;
}

/**
 * @brief  Read an array of bytes from the Ring Buffer.
 *
 * Uses all-or-nothing behaviour. If the requested number of bytes is not
 * available, no data is consumed.
 *
 * @param[in,out] pRingBuf Pointer to RingBuffer_HandleTypeDef structure.
 * @param[out]    pData    Pointer to destination data buffer.
 * @param[in]     Length   Number of bytes to read.
 *
 * @retval RING_BUFFER_OK    Data read successfully.
 * @retval RING_BUFFER_EMPTY Not enough stored data.
 * @retval RING_BUFFER_ERROR Invalid parameter.
 */
RingBuffer_StatusTypeDef RingBuffer_Read(RingBuffer_HandleTypeDef *pRingBuf,
                                          uint8_t *pData,
                                          uint16_t Length)
{
    uint32_t head;
    uint32_t tail;
    uint32_t available;
    uint16_t i;

    /* Check parameters. */
    if ((!RingBuffer_IsValid(pRingBuf)) ||
        (pData == NULL) ||
        (Length == 0U))
    {
        return RING_BUFFER_ERROR;
    }

    /* Snapshot Tail and Head once. */
    tail = pRingBuf->Tail;
    head = pRingBuf->Head;

    /* Calculate the number of currently readable bytes. */
    available = (uint32_t)(head - tail);

    if (available < (uint32_t)Length)
    {
        return RING_BUFFER_EMPTY;
    }

    RING_BUFFER_BARRIER();

    /* Copy data using a local Tail to reduce volatile accesses. */
    for (i = 0U; i < Length; i++)
    {
        pData[i] = pRingBuf->pBuffer[RingBuffer_GetIndex(pRingBuf, tail)];
        tail++;
    }

    RING_BUFFER_BARRIER();

    /* Publish complete consumption at one time. */
    pRingBuf->Tail = tail;

    return RING_BUFFER_OK;
}

/**
 * @brief  Read the next byte without removing it from the Ring Buffer.
 *
 * Reads the byte at the current Tail position but does not advance Tail.
 *
 * @param[in]  pRingBuf Pointer to RingBuffer_HandleTypeDef structure.
 * @param[out] pData    Pointer to destination byte.
 *
 * @retval RING_BUFFER_OK    Byte read successfully.
 * @retval RING_BUFFER_EMPTY Ring Buffer is empty.
 * @retval RING_BUFFER_ERROR Invalid parameter.
 */
RingBuffer_StatusTypeDef RingBuffer_Peek(const RingBuffer_HandleTypeDef *pRingBuf,
                                          uint8_t *pData)
{
    uint32_t head;
    uint32_t tail;

    if ((!RingBuffer_IsValid(pRingBuf)) || (pData == NULL))
    {
        return RING_BUFFER_ERROR;
    }

    tail = pRingBuf->Tail;
    head = pRingBuf->Head;

    if (head == tail)
    {
        return RING_BUFFER_EMPTY;
    }

    RING_BUFFER_BARRIER();

    /* Read next byte without advancing Tail. */
    *pData = pRingBuf->pBuffer[RingBuffer_GetIndex(pRingBuf, tail)];

    return RING_BUFFER_OK;
}

/**
 * @brief  Get the current number of stored bytes.
 *
 * The value is calculated from the difference between Head and Tail instead of
 * using a shared Count variable.
 *
 * @param[in] pRingBuf Pointer to RingBuffer_HandleTypeDef structure.
 *
 * @retval Number of currently stored bytes.
 */
uint16_t RingBuffer_GetCount(const RingBuffer_HandleTypeDef *pRingBuf)
{
    uint32_t count;

    if (!RingBuffer_IsValid(pRingBuf))
    {
        return 0U;
    }

    count = RingBuffer_GetCountRaw(pRingBuf);

    /* Defensive clamp for invalid concurrent usage. */
    if (count > (uint32_t)pRingBuf->Size)
    {
        count = (uint32_t)pRingBuf->Size;
    }

    return (uint16_t)count;
}

/**
 * @brief  Get the remaining writable space.
 *
 * @param[in] pRingBuf Pointer to RingBuffer_HandleTypeDef structure.
 *
 * @retval Number of free bytes.
 */
uint16_t RingBuffer_GetFreeSpace(const RingBuffer_HandleTypeDef *pRingBuf)
{
    if (!RingBuffer_IsValid(pRingBuf))
    {
        return 0U;
    }

    return (uint16_t)(pRingBuf->Size -
                      RingBuffer_GetCount(pRingBuf));
}

/**
 * @brief  Get the configured Ring Buffer capacity.
 *
 * @param[in] pRingBuf Pointer to RingBuffer_HandleTypeDef structure.
 *
 * @retval Configured usable capacity in bytes.
 */
uint16_t RingBuffer_GetCapacity(const RingBuffer_HandleTypeDef *pRingBuf)
{
    if (!RingBuffer_IsValid(pRingBuf))
    {
        return 0U;
    }

    return pRingBuf->Size;
}

/**
 * @brief  Check whether the Ring Buffer is empty.
 *
 * @param[in] pRingBuf Pointer to RingBuffer_HandleTypeDef structure.
 *
 * @retval true  Ring Buffer is empty.
 * @retval false Ring Buffer contains data.
 */
bool RingBuffer_IsEmpty(const RingBuffer_HandleTypeDef *pRingBuf)
{
    if (!RingBuffer_IsValid(pRingBuf))
    {
        return true;
    }

    return (pRingBuf->Head == pRingBuf->Tail);
}

/**
 * @brief  Check whether the Ring Buffer is full.
 *
 * @param[in] pRingBuf Pointer to RingBuffer_HandleTypeDef structure.
 *
 * @retval true  Ring Buffer is full.
 * @retval false Ring Buffer still has free space.
 */
bool RingBuffer_IsFull(const RingBuffer_HandleTypeDef *pRingBuf)
{
    if (!RingBuffer_IsValid(pRingBuf))
    {
        return false;
    }

    return (RingBuffer_GetCountRaw(pRingBuf) >=
            (uint32_t)pRingBuf->Size);
}

#ifdef __cplusplus
}
#endif

/*--------------------------------------------------- End Of File -----------------------------------------------------*/
