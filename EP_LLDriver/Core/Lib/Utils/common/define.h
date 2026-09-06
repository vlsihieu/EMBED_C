/**
 * @file    Define.h
 * @brief   Hardware register map, structure definitions, and peripheral pointers for STM32.
 * @date    May 6, 2026
 * @author  hieuk
 */

#ifndef LIB_COMMON_INC_DEFINE_H_
#define LIB_COMMON_INC_DEFINE_H_

#include <stdint.h> /* Standard C99 data types (uint32_t, uint8_t, ...) */

/* ========================================================================== */
/*                      1. BASE ADDRESS DEFINITIONS                           */
/* ========================================================================== */

/* --- AHB Bus Peripherals --- */
#define DMA1x_Address                0x40020000U
#define DMA2x_Address                0x40020400U
#define RCCx_Address                 0x40021000U
#define Flashx_Address               0x40022000U

/* --- APB2 Bus Peripherals --- */
#define AFIOx_Address                0x40010000U
#define EXTIx_Address                0x40010400U
#define GPIOx_Portx_A_Address        0x40010800U
#define GPIOx_Portx_B_Address        0x40010C00U
#define GPIOx_Portx_C_Address        0x40011000U
#define GPIOx_Portx_D_Address        0x40011400U
#define GPIOx_Portx_E_Address        0x40011800U
#define GPIOx_Portx_F_Address        0x40011C00U
#define GPIOx_Portx_G_Address        0x40012000U
#define TIM1x_timer_Address          0x40012C00U
#define SPI1x_Address                0x40013000U
#define USART1x_Address              0x40013800U

/* --- APB1 Bus Peripherals --- */
#define TIM2x_timer_Address          0x40000000U
#define TIM3x_timer_Address          0x40000400U
#define TIM4x_timer_Address          0x40000800U
#define TIM5x_timer_Address          0x40000C00U
#define TIM6x_timer_Address          0x40001000U
#define TIM7x_timer_Address          0x40001400U
#define TIM12x_timer_Address         0x40001800U
#define TIM13x_timer_Address         0x40001C00U
#define TIM14x_timer_Address         0x40002000U
#define CAN1x_Address                0x40006400U /* CAN1 Register Base Address */
#define SPI2_I2S_Address             0x40003800U
#define SPI3_I2S_Address             0x40003C00U
#define USART2x_Address              0x40004400U
#define USART3x_Address              0x40004800U
#define UART4x_Address               0x40004C00U
#define UART5x_Address               0x40005000U
#define I2C1x_Address                0x40005400U
#define I2C2x_Address                0x40005800U

/* ========================================================================== */
/*                      2. COMMON ENUMS                                       */
/* ========================================================================== */

/**
 * @brief  HAL/Driver operation status definitions.
 */
typedef enum
{
  STD_OK       = 0x00U, /* Operation successful */
  STD_ERROR    = 0x01U, /* Operation failed */
  STD_BUSY     = 0x02U, /* Peripheral is busy */
  STD_TIMEOUT  = 0x03U  /* Operation timeout */
} HALx_StatusTypeDef;



/* ========================================================================== */
/*                      3. PERIPHERAL REGISTER STRUCTURES                     */
/* ========================================================================== */

/**
 * @brief  GPIO register structure definition.
 */
typedef struct
{
   volatile uint32_t CRL;  /* Port configuration register low (Pins 0->7) */
   volatile uint32_t CRH;  /* Port configuration register high (Pins 8->15) */
   volatile uint32_t IDR;  /* Port input data register */
   volatile uint32_t ODR;  /* Port output data register */
   volatile uint32_t BSRR; /* Port bit set/reset register */
   volatile uint32_t BRR;  /* Port bit reset register */
   volatile uint32_t LCKR; /* Port configuration lock register */
} GPIOx_typedef_t;

/**
 * @brief  AFIO (Alternate Function I/O) register structure definition.
 */
typedef struct
{
   volatile uint32_t EVCR;    /* Event control register */
   volatile uint32_t MAPR;    /* AF remap and debug I/O configuration register */
   volatile uint32_t EXTICR1; /* External interrupt configuration register 1 (Pins 0->3) */
   volatile uint32_t EXTICR2; /* External interrupt configuration register 2 (Pins 4->7) */
   volatile uint32_t EXTICR3; /* External interrupt configuration register 3 (Pins 8->11) */
   volatile uint32_t EXTICR4; /* External interrupt configuration register 4 (Pins 12->15) */
   volatile uint32_t MAPR2;   /* AF remap and debug I/O configuration register 2 */
} AFIOx_typedef_t;

/**
 * @brief  TIM (Timer) register structure definition.
 */
typedef struct
{
    volatile uint32_t CR1;   /* Control register 1 */
    volatile uint32_t CR2;   /* Control register 2 */
    volatile uint32_t SMCR;  /* Slave mode control register */
    volatile uint32_t DIER;  /* DMA/Interrupt enable register */
    volatile uint32_t SR;    /* Status register */
    volatile uint32_t EGR;   /* Event generation register */
    volatile uint32_t CCMR1; /* Capture/compare mode register 1 */
    volatile uint32_t CCMR2; /* Capture/compare mode register 2 */
    volatile uint32_t CCER;  /* Capture/compare enable register */
    volatile uint32_t CNT;   /* Counter register */
    volatile uint32_t PSC;   /* Prescaler register */
    volatile uint32_t ARR;   /* Auto-reload register */
    volatile uint32_t RCR;   /* Repetition counter register */
    volatile uint32_t CCR1;  /* Capture/compare register 1 */
    volatile uint32_t CCR2;  /* Capture/compare register 2 */
    volatile uint32_t CCR3;  /* Capture/compare register 3 */
    volatile uint32_t CCR4;  /* Capture/compare register 4 */
    volatile uint32_t BDTR;  /* Break and dead-time register */
    volatile uint32_t DCR;   /* DMA control register */
    volatile uint32_t DMAR;  /* DMA address for full transfer */
} TIMx_TypeDef_t;

/**
 * @brief  DMA Channel register structure definition.
 */
typedef struct {
    volatile uint32_t CCR;      /* DMA channel configuration register */
    volatile uint32_t CNDTR;    /* DMA channel number of data register */
    volatile uint32_t CPAR;     /* DMA channel peripheral address register */
    volatile uint32_t CMAR;     /* DMA channel memory address register */
    volatile uint32_t RESERVED; /* Reserved memory space */
} DMAx_Channel_TypeDef_t;

/**
 * @brief  DMA Global register structure definition.
 */
typedef struct {
    volatile uint32_t ISR;  /* DMA interrupt status register */
    volatile uint32_t IFCR; /* DMA interrupt flag clear register */
} DMAx_TypeDef_t;

/**
 * @brief  USART/UART register structure definition.
 */
typedef struct {
    volatile uint32_t SR;   /* Status register */
    volatile uint32_t DR;   /* Data register */
    volatile uint32_t BRR;  /* Baud rate register */
    volatile uint32_t CR1;  /* Control register 1 */
    volatile uint32_t CR2;  /* Control register 2 */
    volatile uint32_t CR3;  /* Control register 3 */
    volatile uint32_t GTPR; /* Guard time and prescaler register */
} USARTx_TypeDef_t;

/**
 * @brief  SPI register structure definition.
 */
typedef struct
{
  volatile uint32_t CR1;     /* Control register 1 */
  volatile uint32_t CR2;     /* Control register 2 */
  volatile uint32_t SR;      /* Status register */
  volatile uint32_t DR;      /* Data register */
  volatile uint32_t CRCPR;   /* CRC polynomial register */
  volatile uint32_t RXCRCR;  /* Rx CRC register */
  volatile uint32_t TXCRCR;  /* Tx CRC register */
  volatile uint32_t I2SCFGR; /* I2S configuration register */
} SPIx_TypeDef;

/**
 * @brief  I2C register structure definition.
 */
typedef struct
{
    volatile uint32_t CR1;   /* I2C Control register 1 */
    volatile uint32_t CR2;   /* I2C Control register 2 */
    volatile uint32_t OAR1;  /* I2C Own address register 1 */
    volatile uint32_t OAR2;  /* I2C Own address register 2 */
    volatile uint32_t DR;    /* I2C Data register */
    volatile uint32_t SR1;   /* I2C Status register 1 */
    volatile uint32_t SR2;   /* I2C Status register 2 */
    volatile uint32_t CCR;   /* I2C Clock control register */
    volatile uint32_t TRISE; /* I2C TRISE register */
} I2Cx_TypeDef_t;

/**
 * @brief CAN Peripheral Register Map structure for STM32F103 (bxCAN)
 */
typedef struct
{
    volatile uint32_t MCR;       /*!< Master Control Register */
    volatile uint32_t MSR;       /*!< Master Status Register */
    volatile uint32_t TSR;       /*!< Transmit Status Register */
    volatile uint32_t RF0R;      /*!< Receive FIFO 0 Register */
    volatile uint32_t RF1R;      /*!< Receive FIFO 1 Register */
    volatile uint32_t IER;       /*!< Interrupt Enable Register */
    volatile uint32_t ESR;       /*!< Error Status Register */
    volatile uint32_t BTR;       /*!< Bit Timing Register */
    uint32_t          RESERVED0[88];
    struct {
        volatile uint32_t TIR;   /*!< Tx Mailbox Identifier Register */
        volatile uint32_t TDTR;  /*!< Tx Mailbox Data Length Control and Time Stamp Register */
        volatile uint32_t TDLR;  /*!< Tx Mailbox Data Low Register */
        volatile uint32_t TDHR;  /*!< Tx Mailbox Data High Register */
    } TxMailbox[3];              /*!< 3 Transmit Mailboxes */
    struct {
        volatile uint32_t RIR;   /*!< Rx FIFO Mailbox Identifier Register */
        volatile uint32_t RDTR;  /*!< Rx FIFO Mailbox Data Length Control and Time Stamp Register */
        volatile uint32_t RDLR;  /*!< Rx FIFO Mailbox Data Low Register */
        volatile uint32_t RDHR;  /*!< Rx FIFO Mailbox Data High Register */
    } FIFOMailbox[2];            /*!< 2 Receive FIFOs */
    uint32_t          RESERVED1[12];
    volatile uint32_t FMR;       /*!< Filter Master Register */
    volatile uint32_t FM1R;      /*!< Filter Mode Register */
    uint32_t          RESERVED2;
    volatile uint32_t FS1R;      /*!< Filter Scale Register */
    uint32_t          RESERVED3;
    volatile uint32_t FFA1R;     /*!< Filter FIFO Assignment Register */
    uint32_t          RESERVED4;
    volatile uint32_t FA1R;      /*!< Filter Activation Register */
    uint32_t          RESERVED5[8];
    struct {
        volatile uint32_t FR1;   /*!< Filter Bank Register 1 */
        volatile uint32_t FR2;   /*!< Filter Bank Register 2 */
    } FilterRegister[14];        /*!< 14 Filter banks for STM32F103 medium-density */
} CANx_TypeDef_t;

/* ========================================================================== */
/*                      4. PERIPHERAL POINTER MACROS                          */
/* ========================================================================== */

/* --- GPIO Pointers --- */
#define GPIOAx          ((GPIOx_typedef_t *)GPIOx_Portx_A_Address)
#define GPIOBx          ((GPIOx_typedef_t *)GPIOx_Portx_B_Address)
#define GPIOCx          ((GPIOx_typedef_t *)GPIOx_Portx_C_Address)
#define GPIODx          ((GPIOx_typedef_t *)GPIOx_Portx_D_Address)
#define GPIOEx          ((GPIOx_typedef_t *)GPIOx_Portx_E_Address)
#define AFIOx           ((AFIOx_typedef_t *)AFIOx_Address)

/* --- Timer Pointers --- */
#define TIM1x           ((TIMx_TypeDef_t *) TIM1x_timer_Address)
#define TIM2x           ((TIMx_TypeDef_t *) TIM2x_timer_Address)
#define TIM3x           ((TIMx_TypeDef_t *) TIM3x_timer_Address)
#define TIM4x           ((TIMx_TypeDef_t *) TIM4x_timer_Address)
#define TIM5x           ((TIMx_TypeDef_t *) TIM5x_timer_Address)
#define TIM6x           ((TIMx_TypeDef_t *) TIM6x_timer_Address)
#define TIM7x           ((TIMx_TypeDef_t *) TIM7x_timer_Address)
#define TIM12x          ((TIMx_TypeDef_t *) TIM12x_timer_Address)
#define TIM13x          ((TIMx_TypeDef_t *) TIM13x_timer_Address)
#define TIM14x          ((TIMx_TypeDef_t *) TIM14x_timer_Address)

/* --- DMA Pointers --- */
#define DMA1x            ((DMAx_TypeDef_t *)DMA1x_Address)
#define DMA1x_Channel1   ((DMAx_Channel_TypeDef_t *)(DMA1x_Address + 0x0008U))
#define DMA1x_Channel2   ((DMAx_Channel_TypeDef_t *)(DMA1x_Address + 0x001CU))
#define DMA1x_Channel3   ((DMAx_Channel_TypeDef_t *)(DMA1x_Address + 0x0030U))
#define DMA1x_Channel4   ((DMAx_Channel_TypeDef_t *)(DMA1x_Address + 0x0044U))
#define DMA1x_Channel5   ((DMAx_Channel_TypeDef_t *)(DMA1x_Address + 0x0058U))
#define DMA1x_Channel6   ((DMAx_Channel_TypeDef_t *)(DMA1x_Address + 0x006CU))
#define DMA1x_Channel7   ((DMAx_Channel_TypeDef_t *)(DMA1x_Address + 0x0080U))

/* --- UART/USART Pointers --- */
#define USART1x          ((USARTx_TypeDef_t *)USART1x_Address)
#define USART2x          ((USARTx_TypeDef_t *)USART2x_Address)
#define USART3x          ((USARTx_TypeDef_t *)USART3x_Address)

/* --- SPI Pointers (Struct Pointer Cast) --- */
#define SPI1x            ((SPIx_TypeDef *)SPI1x_Address)
#define SPI2x            ((SPIx_TypeDef *)SPI2_I2S_Address)
#define SPI3x            ((SPIx_TypeDef *)SPI3_I2S_Address)

/* --- CAN Pointer --- */
#define CAN1x            ((CANx_TypeDef_t *)CAN1x_Address)

/* --- EXTI Direct Register Access --- */
#define EXTI_IMR         (*(volatile uint32_t *)(EXTIx_Address + 0x00U)) /* Interrupt Mask Register */
#define EXTI_EMR         (*(volatile uint32_t *)(EXTIx_Address + 0x04U)) /* Event Mask Register */
#define EXTI_RTSR        (*(volatile uint32_t *)(EXTIx_Address + 0x08U)) /* Rising Trigger Selection Register */
#define EXTI_FTSR        (*(volatile uint32_t *)(EXTIx_Address + 0x0CU)) /* Falling Trigger Selection Register */
#define EXTI_SWIER       (*(volatile uint32_t *)(EXTIx_Address + 0x10U)) /* Software Interrupt Event Register */
#define EXTI_PR          (*(volatile uint32_t *)(EXTIx_Address + 0x14U)) /* Pending Register */

/* --- I2C Pointers --- */
#define I2C1x                         ((I2Cx_TypeDef_t *)I2C1x_Address)
#define I2C2x                         ((I2Cx_TypeDef_t *)I2C2x_Address)

/* ========================================================================== */
/*                      5. ASSERT MACRO DEFINITION                            */
/* ========================================================================== */

#ifdef USE_FULL_ASSERT
/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr If expr is false, it calls assert_failed function
  *         which reports the name of the source file and the source
  *         line number of the call that failed.
  *         If expr is true, it returns no value.
  */
#define assert_param(expr) ((expr) ? (void)0U : assert_failed((uint8_t *)__FILE__, __LINE__))

void assert_failed(uint8_t* file, uint32_t line);
#else
#define assert_param(expr) ((void)0U)
#endif /* USE_FULL_ASSERT */

/* --- Parameter check helper macros --- */
#define IS_CANx_ALL_INSTANCE(INSTANCE) (((INSTANCE) == CAN1x))

#endif /* LIB_COMMON_INC_DEFINE_H_ */
