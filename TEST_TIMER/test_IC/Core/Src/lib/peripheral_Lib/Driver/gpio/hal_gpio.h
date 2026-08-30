/***********************************************************************************************************************
 * Project Name: STM32F103_BareMetal_Drivers
 * 
 * File Name: hal_gpio.h
 *
 * Description: HAL GPIO Driver — Data types, register structures, bitmasks,
 *              and API prototypes for the General Purpose I/O peripheral
 *              on STM32F103xB devices.
 * 
 * Compiler: GCC
 *
 * Revision:
 *              Version         Date                Change History
 *              1.0.0           10/08/2026          Initial release for STM32F103xB GPIO Module
 *
 **********************************************************************************************************************/

#ifndef HAL_GPIO_H_
#define HAL_GPIO_H_

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 *                                                     INCLUDES
 **********************************************************************************************************************/

#include <stdint.h>
#include "stm32f103xb.h"
#include "../../../peripheral_Lib/Utils/common/define.h"
#include "../../../peripheral_Lib/Driver/rcc/hal_clock.h"

/***********************************************************************************************************************
 *                                                     MACROS
 **********************************************************************************************************************/

/**
 * @brief  GPIO pin selection masks.
 * @req    HWR-GPIO-002
 * @req    FR-GPIO-003, FR-GPIO-005
 */
#define GPIOx_PIN_0                 ((uint16_t)0x0001)  /* Pin 0 selected    */
#define GPIOx_PIN_1                 ((uint16_t)0x0002)  /* Pin 1 selected    */
#define GPIOx_PIN_2                 ((uint16_t)0x0004)  /* Pin 2 selected    */
#define GPIOx_PIN_3                 ((uint16_t)0x0008)  /* Pin 3 selected    */
#define GPIOx_PIN_4                 ((uint16_t)0x0010)  /* Pin 4 selected    */
#define GPIOx_PIN_5                 ((uint16_t)0x0020)  /* Pin 5 selected    */
#define GPIOx_PIN_6                 ((uint16_t)0x0040)  /* Pin 6 selected    */
#define GPIOx_PIN_7                 ((uint16_t)0x0080)  /* Pin 7 selected    */
#define GPIOx_PIN_8                 ((uint16_t)0x0100)  /* Pin 8 selected    */
#define GPIOx_PIN_9                 ((uint16_t)0x0200)  /* Pin 9 selected    */
#define GPIOx_PIN_10                ((uint16_t)0x0400)  /* Pin 10 selected   */
#define GPIOx_PIN_11                ((uint16_t)0x0800)  /* Pin 11 selected   */
#define GPIOx_PIN_12                ((uint16_t)0x1000)  /* Pin 12 selected   */
#define GPIOx_PIN_13                ((uint16_t)0x2000)  /* Pin 13 selected   */
#define GPIOx_PIN_14                ((uint16_t)0x4000)  /* Pin 14 selected   */
#define GPIOx_PIN_15                ((uint16_t)0x8000)  /* Pin 15 selected   */
#define GPIOx_PIN_All               ((uint16_t)0xFFFF)  /* All pins selected */

#define GPIOx_PIN_MASK               (0xFFFFu)

/**
 * @brief  GPIO parameter validation macros.
 * @req    FR-GPIO-002, FR-GPIO-003, FR-GPIO-004
 * @req    NFR-GPIO-005
 */
#define ASSET_GPIO_PIN_ACTION(ACTION) (((ACTION) == LOW) || ((ACTION) == HIGH))

#define ASSET_GPIO_PIN(PIN)         (((((uint32_t)PIN) & GPIO_PIN_MASK) != 0x00u) && \
                                     ((((uint32_t)PIN) & ~GPIO_PIN_MASK) == 0x00u))

#define ASSET_GPIO_ALL_INSTANCE(INSTANCE) (((INSTANCE) == GPIOA) || \
                                           ((INSTANCE) == GPIOB) || \
                                           ((INSTANCE) == GPIOC) || \
                                           ((INSTANCE) == GPIOD) || \
                                           ((INSTANCE) == GPIOE))

/**
 * @brief  GPIO configuration lock register definitions.
 * @req    HWR-GPIO-014
 * @req    FR-GPIO-031, FR-GPIO-032
 */
#define GPIOx_LCKR_LCKK_Pos         (16U)
#define GPIOx_LCKR_LCKK_Msk         (0x1UL << GPIOx_LCKR_LCKK_Pos)
#define GPIOx_LCKR_LCKK             GPIOx_LCKR_LCKK_Msk

/**
 * @brief  GPIO basic operating mode definitions.
 * @req    HWR-GPIO-004, HWR-GPIO-007
 * @req    FR-GPIO-007, FR-GPIO-008, FR-GPIO-009
 * @req    FR-GPIO-010, FR-GPIO-011, FR-GPIO-012
 */
#define GPIOx_MODE_INPUT            (0x00000000U)      /* Input Floating Mode */
#define GPIOx_MODE_AF_INPUT         GPIOx_MODE_INPUT   /* Alternate Function Input Mode */
#define GPIOx_MODE_ANALOG           (0x00000003U)      /* Analog Mode */
#define GPIOx_MODE_OUTPUT_PP        (0x00000001U)      /* General Purpose Output Push-Pull Mode */
#define GPIOx_MODE_OUTPUT_OD        (0x00000011U)      /* General Purpose Output Open-Drain Mode */
#define GPIOx_MODE_AF_PP            (0x00000002U)      /* Alternate Function Output Push-Pull Mode */
#define GPIOx_MODE_AF_OD            (0x00000012U)      /* Alternate Function Output Open-Drain Mode */

/**
 * @brief  GPIO EXTI interrupt and event mode definitions.
 * @req    HWR-GPIO-011, HWR-GPIO-012, HWR-GPIO-013
 * @req    FR-GPIO-016, FR-GPIO-020, FR-GPIO-021
 * @req    FR-GPIO-022, FR-GPIO-023, FR-GPIO-024
 */
#define GPIOx_MODE_IT_RISING             (0x10110000U)  ///< Interrupt Mode with Rising edge trigger
#define GPIOx_MODE_IT_FALLING            (0x10210000U)  ///< Interrupt Mode with Falling edge trigger
#define GPIOx_MODE_IT_RISING_FALLING     (0x10310000U)  ///< Interrupt Mode with Both edge trigger
#define GPIOx_MODE_EVT_RISING            (0x10120000U)  ///< Event Mode with Rising edge trigger
#define GPIOx_MODE_EVT_FALLING           (0x10220000U)  ///< Event Mode with Falling edge trigger
#define GPIOx_MODE_EVT_RISING_FALLING    (0x10320000U)  ///< Event Mode with Both edge trigger

/**
 * @brief  GPIO output speed definitions.
 * @req    HWR-GPIO-008
 * @req    FR-GPIO-015
 */
#define GPIOx_SPEED_FREQ_LOW        (0x02U) /* Max Speed 2MHz  */
#define GPIOx_SPEED_FREQ_MEDIUM     (0x01U) /* Max Speed 10MHz */
#define GPIOx_SPEED_FREQ_HIGH       (0x03U) /* Max Speed 50MHz */

/**
 * @brief  GPIO input pull configuration definitions.
 * @req    HWR-GPIO-007
 * @req    FR-GPIO-012, FR-GPIO-013, FR-GPIO-014, FR-GPIO-017
 */
#define GPIOx_NOPULL                (0x00U)
#define GPIOx_PULLUP                (0x01U)
#define GPIOx_PULLDOWN              (0x02U)

/**
 * @brief  AFIO remap encoding definitions.
 * @req    HWR-GPIO-015
 * @req    FR-GPIO-034, FR-GPIO-035, FR-GPIO-036
 * @req    NFR-GPIO-006
 */
#define GPIOx_AFIO_MAPR_SWJ (24U)      /* SWJ_CFG bit position in AFIO_MAPR */
#define GPIOx_AFIO_REMAP_GROUP_MASK (0xF0U)      /* Bits[7:4]: Peripheral group */

/**
 * @brief  Internal GPIO EXTI/Event mode encoding.
 * @req    HWR-GPIO-012, HWR-GPIO-013
 * @req    FR-GPIO-016, FR-GPIO-020, FR-GPIO-021
 * @req    FR-GPIO-022, FR-GPIO-023, FR-GPIO-024
 */
#define GPIOx_MODE_CATEGORY_MASK    (0x10000000U) /* Bit[28]: Basic GPIO / EXTI-Event */
#define GPIOx_MODE_TRIGGER_MASK     (0x00300000U) /* Bits[21:20]: Trigger selection */
#define GPIOx_MODE_RISING_EDGE      (0x00100000U) /* Rising-edge trigger */
#define GPIOx_MODE_FALLING_EDGE     (0x00200000U) /* Falling-edge trigger */
#define GPIOx_MODE_EVENT            (0x00020000U) /* Event request */

/**
 * @brief  AFIO_MAPR peripheral remap bit definitions.
 * @req    HWR-GPIO-015
 * @req    FR-GPIO-034, FR-GPIO-035, FR-GPIO-036, FR-GPIO-037
 * @req    NFR-GPIO-004, NFR-GPIO-006
 */
#define GPIOx_BIT_AFIO_MAPR_SP1_REMAP               (0x1UL << 0U)
#define GPIOx_BIT_AFIO_MAPR_I2C1_REMAP              (0x1UL << 1U)
#define GPIOx_BIT_AFIO_MAPR_USART1_REMAP            (0x1UL << 2U)
#define GPIOx_BIT_AFIO_MAPR_USART2_REMAP            (0x1UL << 2U)
#define GPIOx_BIT_AFIO_MAPR_USART3_REMAP            (0x03U << 4U)
#define GPIOx_BIT_AFIO_MAPR_USART3_PARTIAL_REMAP    (0x01U << 4U)
#define GPIOx_BIT_AFIO_MAPR_TIM1_REMAP              (0x03U << 6U)
#define GPIOx_BIT_AFIO_MAPR_TIM1_PARTIAL_REMAP      (0x01U << 6U)
#define GPIOx_BIT_AFIO_MAPR_TIM2_REMAP              (0x03U << 8U)
#define GPIOx_BIT_AFIO_MAPR_TIM2_PARTIAL1_REMAP     (0x01U << 8U)
#define GPIOx_BIT_AFIO_MAPR_TIM2_PARTIAL2_REMAP     (0x02U << 8U)
#define GPIOx_BIT_AFIO_MAPR_TIM3_REMAP              (0x03U << 10U)
#define GPIOx_BIT_AFIO_MAPR_TIM3_PARITIAL_REMAP     (0x02U << 10U)
#define GPIOx_BIT_AFIO_MAPR_TIM4_REMAP              (0x01U << 12U)
#define GPIOx_BIT_AFIO_MAPR_CAN_REMAP               (0x03U << 13U)
#define GPIOx_BIT_AFIO_MAPR_PARTIAL_CAN_REMAP       (0x02U << 13U)

/**
 * @brief  GPIO CRL/CRH MODE and CNF field definitions.
 * @req    HWR-GPIO-003, HWR-GPIO-004, HWR-GPIO-007
 * @req    FR-GPIO-006, FR-GPIO-007, FR-GPIO-008, FR-GPIO-009
 * @req    FR-GPIO-010, FR-GPIO-011, FR-GPIO-012, FR-GPIO-013, FR-GPIO-014
 */
#define GPIOx_BIT_MODE_OUTPUT_PP                 (0x00UL << 2U)
#define GPIOx_BIT_MODE_OUTPUT_OD                 (0x01UL << 2U)
#define GPIOx_BIT_MODE_AF_PP                     (0x02UL << 2U)
#define GPIOx_BIT_MODE_AF_OD                     (0x03UL << 2U)
#define GPIOx_BIT_MODE_ANALOG                    (0x00UL << 2U)
#define GPIOx_BIT_MODE_FLOATING_IN               (0x01UL << 2U)
#define GPIOx_BIT_MODE_PP_IN                     (0x02UL << 2U)

/***********************************************************************************************************************
 *                                       TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
 **********************************************************************************************************************/

/**
 * @brief  GPIO Pin State enum.
 * @req    FR-GPIO-027, FR-GPIO-028, FR-GPIO-030
 */
typedef enum
{
    GPIOx_PinState_LOW = 0,
    GPIOx_PinState_HIGH
} GPIOx_PinState_t;

/**
 * @brief  GPIO Status enum.
 * @req    FR-GPIO-032
 * @req    NFR-GPIO-009
 */
typedef enum
{
    GPIOx_StatusTypeDef_OK       = 0x00U,
    GPIOx_StatusTypeDef_ERROR    = 0x01U,
    GPIOx_StatusTypeDef_BUSY     = 0x02U,
    GPIOx_StatusTypeDef_TIMEOUT  = 0x03U
} GPIOx_StatusTypeDef_t;

/**
 * @brief  AFIO Pin Remap configuration enum.
 * @req    HWR-GPIO-015
 * @req    FR-GPIO-034, FR-GPIO-035, FR-GPIO-036, FR-GPIO-037
 * @req    NFR-GPIO-006
 */
typedef enum
{
    AFIOx_Remap_SWJ_FULL             = 0x00U,
    AFIOx_Remap_SWJ_NO_JNTRST        = 0x01U,
    AFIOx_Remap_SWJ_NONJTAG          = 0x02U,
    AFIOx_Remap_SWJ_DISABLE          = 0x04U,
    AFIOx_Remap_SPI1_NONE            = 0x10U,
    AFIOx_Remap_SPI1_FULL            = 0x11U,
    AFIOx_Remap_I2C1_NONE            = 0x20U,
    AFIOx_Remap_I2C1_FULL            = 0x21U,
    AFIOx_Remap_USART1_NONE          = 0x30U,
    AFIOx_Remap_USART1_FULL          = 0x31U,
    AFIOx_Remap_USART2_NONE          = 0x40U,
    AFIOx_Remap_USART2_FULL          = 0x41U,
    AFIOx_Remap_USART3_NONE          = 0x50U,
    AFIOx_Remap_USART3_PARTIAL       = 0x51U,
    AFIOx_Remap_USART3_FULL          = 0x52U,
    AFIOx_Remap_TIM1_NONE            = 0x60U,
    AFIOx_Remap_TIM1_PARTIAL         = 0x61U,
    AFIOx_Remap_TIM1_FULL            = 0x62U,
    AFIOx_Remap_TIM2_NONE            = 0x70U,
    AFIOx_Remap_TIM2_PARTIAL_1       = 0x71U,
    AFIOx_Remap_TIM2_PARTIAL_2       = 0x72U,
    AFIOx_Remap_TIM2_FULL            = 0x73U,
    AFIOx_Remap_TIM3_NONE            = 0x80U,
    AFIOx_Remap_TIM3_PARTIAL         = 0x81U,
    AFIOx_Remap_TIM3_FULL            = 0x82U,
    AFIOx_Remap_TIM4_NONE            = 0x90U,
    AFIOx_Remap_TIM4_FULL            = 0x91U,
    AFIOx_Remap_CAN_NONE             = 0xA0U,
    AFIOx_Remap_CAN_PARTIAL          = 0xA1U,
    AFIOx_Remap_CAN_FULL             = 0xA2U
} AFIOx_Remap_t;

/**
 * @brief  GPIO Init structure definition.
 * @req    FR-GPIO-001, FR-GPIO-003, FR-GPIO-004
 * @req    FR-GPIO-015, FR-GPIO-017
 * @req    HWR-GPIO-007, HWR-GPIO-008
 */
typedef struct
{
    uint32_t Pin;       /*!< Specifies the GPIO pins to be configured. */
    uint32_t Mode;      /*!< Specifies the operating mode for the selected pins. */
    uint32_t Pull;      /*!< Specifies the Pull-up or Pull-Down activation for the selected pins. */
    uint32_t Speed;     /*!< Specifies the speed for the selected pins. */
} GPIOx_InitTypeDef_t;

/***********************************************************************************************************************
 *                                           GLOBAL VARIABLE DECLARATIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                                FUNCTION PROTOTYPES
 **********************************************************************************************************************/

void GPIO_Init_t(GPIOx_typedef_t *GPIOx, GPIOx_InitTypeDef_t *GPIO_Init);
void GPIO_DeInit_t(GPIOx_typedef_t *GPIOx, uint16_t GPIO_Pin);
void GPIO_WritePin_t(GPIOx_typedef_t *GPIOx, uint16_t GPIO_Pin, GPIOx_PinState_t State);
void GPIO_Toggle_t(GPIOx_typedef_t *GPIOx, uint16_t GPIO_Pin);
void AFIO_RemapConfig_t(AFIOx_Remap_t Remap);
void GPIO_EXTI_IRQHandler_t(uint16_t GPIO_Pin);
void GPIO_EXTI_Callback_t(uint16_t GPIO_Pin);
GPIOx_PinState_t GPIO_ReadPin_t(GPIOx_typedef_t *GPIOx, uint16_t GPIO_Pin);
GPIOx_StatusTypeDef_t GPIO_LockPin_t(GPIOx_typedef_t *GPIOx, uint16_t GPIO_Pin);

#ifdef __cplusplus
}
#endif

#endif /* HAL_GPIO_H_ */

/*--------------------------------------------------- End Of File -----------------------------------------------------*/