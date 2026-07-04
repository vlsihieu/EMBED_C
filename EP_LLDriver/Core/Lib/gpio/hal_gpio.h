/**
 ******************************************************************************
 * @file    hal_gpio.h
 * @author  hieuk
 * @brief   HAL GPIO Driver — Data types, register structures, bitmasks,
 *          and API prototypes for the General Purpose I/O peripheral
 *          on STM32F103xB devices.
 * @date    May 6, 2026
 *
 * @details This driver wraps the GPIO peripheral to provide:
 *          - Pin configuration (input, output, alternate function)
 *          - Pull-up / pull-down setup
 *          - Output speed and mode selection
 *          - Pin read/write helpers
 *          - Interrupt configuration
 *
 ******************************************************************************
 */

#ifndef LIB_GPIO_INC_GPIOX_IP_H_
#define LIB_GPIO_INC_GPIOX_IP_H_

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
* INCLUDES AND DEFINITIONS
**********************************************************************************************************************/

#include <stdint.h>
#include "define.h"
#include "hal_clock.h"

/***********************************************************************************************************************
* LOCAL MACROS
**********************************************************************************************************************/

/**
* @brief Validate GPIO pin state.
*/
#define ASSET_GPIO_PIN_ACTION(ACTION) (((ACTION) == LOW) || ((ACTION) == HIGH))

/**
* @brief Validate GPIO pin mask.
*/
#define ASSET_GPIO_PIN(PIN)         (((((uint32_t)PIN) & GPIO_PIN_MASK ) != 0x00u) && \
                                    ((((uint32_t)PIN) & ~GPIO_PIN_MASK) == 0x00u))

/**
* @brief Check if GPIO instance is valid.
*/
#define ASSET_GPIO_ALL_INSTANCE(INSTANCE) (((INSTANCE) == GPIOAx) || \
                                          ((INSTANCE) == GPIOBx) || \
                                          ((INSTANCE) == GPIOCx) || \
                                          ((INSTANCE) == GPIODx) || \
                                          ((INSTANCE) == GPIOEx))

/**
* @brief GPIO pin x mask
*/
#define GPIOx_PIN_0                 ((uint16_t)0x0001)  ///< Pin 0 selected   
#define GPIOx_PIN_1                 ((uint16_t)0x0002)  ///< Pin 1 selected   
#define GPIOx_PIN_2                 ((uint16_t)0x0004)  ///< Pin 2 selected   
#define GPIOx_PIN_3                 ((uint16_t)0x0008)  ///< Pin 3 selected   
#define GPIOx_PIN_4                 ((uint16_t)0x0010)  ///< Pin 4 selected   
#define GPIOx_PIN_5                 ((uint16_t)0x0020)  ///< Pin 5 selected   
#define GPIOx_PIN_6                 ((uint16_t)0x0040)  ///< Pin 6 selected   
#define GPIOx_PIN_7                 ((uint16_t)0x0080)  ///< Pin 7 selected   
#define GPIOx_PIN_8                 ((uint16_t)0x0100)  ///< Pin 8 selected   
#define GPIOx_PIN_9                 ((uint16_t)0x0200)  ///< Pin 9 selected   
#define GPIOx_PIN_10                ((uint16_t)0x0400)  ///< Pin 10 selected  
#define GPIOx_PIN_11                ((uint16_t)0x0800)  ///< Pin 11 selected  
#define GPIOx_PIN_12                ((uint16_t)0x1000)  ///< Pin 12 selected  
#define GPIOx_PIN_13                ((uint16_t)0x2000)  ///< Pin 13 selected  
#define GPIOx_PIN_14                ((uint16_t)0x4000)  ///< Pin 14 selected  
#define GPIOx_PIN_15                ((uint16_t)0x8000)  ///< Pin 15 selected  
#define GPIOx_PIN_All               ((uint16_t)0xFFFF)  ///< All pins selected

/**
* @brief Bit definition for GPIO_LCKR register
*/
#define GPIOx_LCKR_LCKK_Pos         (16U)
#define GPIOx_LCKR_LCKK_Msk         (0x1UL << GPIOx_LCKR_LCKK_Pos)
#define GPIOx_LCKR_LCKK             GPIOx_LCKR_LCKK_Msk

/**
* @brief GPIO Mode configurations (Basic Modes)
*/
#define GPIOx_MODE_INPUT                 (0x00000000U)      ///< Input Floating Mode
#define GPIOx_MODE_AF_INPUT               GPIO_MODE_INPUT   ///< Alternate Function Input Mode
#define GPIOx_MODE_ANALOG                (0x00000003U)      ///< Analog Mode
#define GPIOx_MODE_OUTPUT_PP             (0x00000001U)      ///< General Purpose Output Push-Pull Mode
#define GPIOx_MODE_OUTPUT_OD             (0x00000011U)      ///< General Purpose Output Open-Drain Mode
#define GPIOx_MODE_AF_PP                 (0x00000002U)      ///< Alternate Function Output Push-Pull Mode
#define GPIOx_MODE_AF_OD                 (0x00000012U)      ///< Alternate Function Output Open-Drain Mode

/**
* @brief GPIO Mode configurations (HAL standard covering EXTI)
 * Elements values convention: 0xX0yz00YZ
 *    - X  : GPIO mode or EXTI Mode
 *    - y  : External IT or Event trigger detection
 *    - z  : IO configuration on External IT or Event
 *    - Y  : Output type (Push Pull or Open Drain)
 *    - Z  : IO Direction mode (Input, Output, Alternate or Analog)
*/
#define GPIOx_MODE_IT_RISING             (0x10110000U)  ///< Interrupt Mode with Rising edge trigger
#define GPIOx_MODE_IT_FALLING            (0x10210000U)  ///< Interrupt Mode with Falling edge trigger
#define GPIOx_MODE_IT_RISING_FALLING     (0x10310000U)  ///< Interrupt Mode with Both edge trigger
#define GPIOx_MODE_EVT_RISING            (0x10120000U)  ///< Event Mode with Rising edge trigger
#define GPIOx_MODE_EVT_FALLING           (0x10220000U)  ///< Event Mode with Falling edge trigger
#define GPIOx_MODE_EVT_RISING_FALLING    (0x10320000U)  ///< Event Mode with Both edge trigger

/**
* @brief GPIO output speed definitions
* @details STM32F1 GPIO MODE[1:0] configuration
*          Refer to RM0008 Section 9.2.1
*/
#define GPIOx_SPEED_FREQ_LOW             (0x02U) ///< Max Speed 2MHz 
#define GPIOx_SPEED_FREQ_MEDIUM          (0x01U) ///< Max Speed 10MHz
#define GPIOx_SPEED_FREQ_HIGH            (0x03U) ///< Max Speed 50MHz

/**
* @brief GPIO Pull-Up / Pull-Down configurations
* @note  Used only in input mode with pull-up/pull-down enabled
*        (CNF[1:0] = 10, MODE[1:0] = 00)
*        Pull state is selected through ODR register:
*        - ODR = 1 : Pull-Up
*        - ODR = 0 : Pull-Down
*/
#define GPIOx_NOPULL                     (0x00U)
#define GPIOx_PULLUP                     (0x01U)
#define GPIOx_PULLDOWN                   (0x02U)


/**
* @brief AFIO mask
*/
#define POSITION_GPIOx_AFIO_MAPR_SWJ      (24U)        ///< SWJ_CFG bit position in AFIO_MAPR
#define GPIOx_AFIO_REMAP_GROUP_MASK       (0xF0U)      ///< Bits[7:4]: Peripheral group

/**
* @brief GPIO Mode Encoding
*/
#define GPIOx_MODE_CATEGORY_MASK          (0x10000000U) ///< Bit[28]: Basic GPIO / EXTI-Event
#define GPIOx_MODE_TRIGGER_MASK           (0x00300000U) ///< Bits[21:20]: Trigger selection
#define GPIOx_MODE_RISING_EDGE            (0x00100000U) ///< Rising-edge trigger
#define GPIOx_MODE_FALLING_EDGE           (0x00200000U) ///< Falling-edge trigger
#define GPIOx_MODE_EVENT                  (0x00020000U) ///< Event request

/**
* @brief Register GPIO_AFIO_MAPR definitions
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
* @brief Register GPIO_MODE definitions
*/
#define GPIOx_BIT_MODE_OUTPUT_PP                (0x00UL << 2U)
#define GPIOx_BIT_MODE_OUTPUT_OD                (0x01UL << 2U)
#define GPIOx_BIT_MODE_AF_PP                    (0x02UL << 2U)
#define GPIOx_BIT_MODE_AF_OD                    (0x03UL << 2U)
#define GPIOx_BIT_MODE_ANALOG                   (0x00UL << 2U)
#define GPIOx_BIT_MODE_FLOATING_IN              (0x01UL << 2U)
#define GPIOx_BIT_MODE_PP_IN                    (0x02UL << 2U)

/***********************************************************************************************************************
* LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
**********************************************************************************************************************/

/**
* @brief GPIO pin state definition.
*/
typedef enum {
   GPIOx_PinState_LOW = 0,
   GPIOx_PinState_HIGH
} GPIOx_PinState_t;

/**
* @brief GPIO status return type.
*/
typedef enum
{
   GPIOx_StatusTypeDef_OK       = 0x00U,  ///< Operation successful
   GPIOx_StatusTypeDef_ERROR    = 0x01U,  ///< Operation failed
   GPIOx_StatusTypeDef_BUSY     = 0x02U,  ///< GPIO is busy
   GPIOx_StatusTypeDef_TIMEOUT  = 0x03U   ///< Operation timed out
} GPIOx_StatusTypeDef_t;

/**
 * @brief AFIO Function Remap definitions
 */
typedef enum 
{

   AFIOx_Remap_SWJ_FULL             = 0x00U, ///< Full SWJ (JTAG-DP + SW-DP) - Default after Reset 
   AFIOx_Remap_SWJ_NO_JNTRST        = 0x01U, ///< Full SWJ (JTAG-DP + SW-DP) but disable NJTRST pin (releases PB4)
   AFIOx_Remap_SWJ_NONJTAG          = 0x02U, ///< Disable JTAG-DP, keep SW-DP (releases PA15, PB3, PB4 for GPIO)
   AFIOx_Remap_SWJ_DISABLE          = 0x04U, ///< Disable both SWD and JTAG (releases PA13, PA14, PA15, PB3, PB4) - USE WITH CAUTION
   AFIOx_Remap_SPI1_NONE            = 0x10U, ///< SPI1: NSS/PA4, SCK/PA5, MISO/PA6, MOSI/PA7
   AFIOx_Remap_SPI1_FULL            = 0x11U, ///< SPI1: NSS/PA15, SCK/PB3, MISO/PB4, MOSI/PB5
   AFIOx_Remap_I2C1_NONE            = 0x20U, ///< I2C1: SCL/PB6, SDA/PB7
   AFIOx_Remap_I2C1_FULL            = 0x21U, ///< I2C1: SCL/PB8, SDA/PB9
   AFIOx_Remap_USART1_NONE          = 0x30U, ///< USART1: TX/PA9, RX/PA10
   AFIOx_Remap_USART1_FULL          = 0x31U, ///< USART1: TX/PB6, RX/PB7
   AFIOx_Remap_USART2_NONE          = 0x40U, ///< USART2: CTS/PA0, RTS/PA1, TX/PA2, RX/PA3, CK/PA4
   AFIOx_Remap_USART2_FULL          = 0x41U, ///< USART2: CTS/PD3, RTS/PD4, TX/PD5, RX/PD6, CK/PD7
   AFIOx_Remap_USART3_NONE          = 0x50U, ///< USART3: TX/PB10, RX/PB11, CK/PB12, CTS/PB13, RTS/PB14
   AFIOx_Remap_USART3_PARTIAL       = 0x51U, ///< USART3 Partial: TX/PC10, RX/PC11, CK/PC12 (Only on large packages)
   AFIOx_Remap_USART3_FULL          = 0x52U, ///< USART3 Full: TX/PD8, RX/PD9, CK/PD10, CTS/PD11, RTS/PD12
   AFIOx_Remap_TIM1_NONE            = 0x60U, ///< TIM1: CH1/PA8, CH2/PA9, CH3/PA10, CH4/PA11, BKIN/PB12, CH1N/PB13, CH2N/PB14, CH3N/PB15
   AFIOx_Remap_TIM1_PARTIAL         = 0x61U, ///< TIM1 Partial: CH1/PA8, CH2/PA9, CH3/PA10, CH4/PA11, BKIN/PA6, CH1N/PA7, CH2N/PB0, CH3N/PB1
   AFIOx_Remap_TIM1_FULL            = 0x62U, ///< TIM1 Full: CH1/PE9, CH2/PE11, CH3/PE13, CH4/PE14, BKIN/PE15, CH1N/PE8, CH2N/PE10, CH3N/PE12
   AFIOx_Remap_TIM2_NONE            = 0x70U, ///< TIM2: CH1/ETR/PA0, CH2/PA1, CH3/PA2, CH4/PA3
   AFIOx_Remap_TIM2_PARTIAL_1       = 0x71U, ///< TIM2 Partial 1: CH1/ETR/PA15, CH2/PB3, CH3/PA2, CH4/PA3
   AFIOx_Remap_TIM2_PARTIAL_2       = 0x72U, ///< TIM2 Partial 2: CH1/ETR/PA0, CH2/PA1, CH3/PB10, CH4/PB11
   AFIOx_Remap_TIM2_FULL            = 0x73U, ///< TIM2 Full: CH1/ETR/PA15, CH2/PB3, CH3/PB10, CH4/PB11
   AFIOx_Remap_TIM3_NONE            = 0x80U, ///< TIM3: CH1/PA6, CH2/PA7, CH3/PB0, CH4/PB1
   AFIOx_Remap_TIM3_PARTIAL         = 0x81U, ///< TIM3 Partial: CH1/PB4, CH2/PB5, CH3/PB0, CH4/PB1
   AFIOx_Remap_TIM3_FULL            = 0x82U, ///< TIM3 Full: CH1/PC6, CH2/PC7, CH3/PC8, CH4/PC9
   AFIOx_Remap_TIM4_NONE            = 0x90U, ///< TIM4: CH1/PB6, CH2/PB7, CH3/PB8, CH4/PB9
   AFIOx_Remap_TIM4_FULL            = 0x91U, ///< TIM4 Full: CH1/PD12, CH2/PD13, CH3/PD14, CH4/PD15
   AFIOx_Remap_CAN_NONE             = 0xA0U, ///< CAN: RX/PA11, TX/PA12
   AFIOx_Remap_CAN_PARTIAL          = 0xA1U, ///< CAN Partial: RX/PB8, TX/PB9
   AFIOx_Remap_CAN_FULL             = 0xA2U  ///< CAN Full: RX/PD0, TX/PD1
} AFIOx_Remap_t;


/**
* @brief GPIO Initialization structure
*/
typedef struct
{
   uint32_t Pin;       ///< Specifies the GPIO pins to be configured.
   uint32_t Mode;      ///< Specifies the operating mode for the selected pins.
   uint32_t Pull;      ///< Specifies the Pull-up or Pull-Down activation for the selected pins.
   uint32_t Speed;     ///< Specifies the speed for the selected pins.
} GPIOx_InitTypeDef_t;

/***********************************************************************************************************************
* GLOBAL FUNCTION PROTOTYPES
**********************************************************************************************************************/

void GPIO_Init_t(GPIOx_typedef_t *GPIOx, GPIOx_InitTypeDef_t *GPIO_Init);
void GPIO_DeInit_t(GPIOx_typedef_t *GPIOx, uint16_t GPIO_Pin);
void GPIO_WritePin_t(GPIOx_typedef_t *GPIOx, uint16_t GPIO_Pin, GPIOx_PinState_t State);
void GPIO_Toggle_t(GPIOx_typedef_t *GPIOx, uint16_t GPIO_Pin);
void AFIO_RemapConfig_t(AFIOx_Remap_t Remap);
void GPIO_EXTI_IRQHandler_t(uint16_t GPIO_Pin);
void GPIO_EXTI_Callback_t(uint16_t GPIO_Pin);
GPIOx_PinState_t        GPIO_ReadPin_t(GPIOx_typedef_t *GPIOx, uint16_t GPIO_Pin);
GPIOx_StatusTypeDef_t   GPIO_LockPin_t(GPIOx_typedef_t *GPIOx, uint16_t GPIO_Pin);

#ifdef __cplusplus
}
#endif

#endif /* LIB_GPIO_INC_GPIOX_IP_H_ */
