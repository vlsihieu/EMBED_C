/**
 ******************************************************************************
 * @file    hal_gpio.c
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

#include "hal_gpio.h"

/***********************************************************************************************************************
* LOCAL HARDWARE REGISTERS MAPPING
**********************************************************************************************************************/

/* EXTI peripheral register map base */
#define EXTI_BASE_ADDR              (0x40010400U)
#define EXTI_IMR                    (*(volatile uint32_t *)(EXTI_BASE_ADDR + 0x00U))
#define EXTI_EMR                    (*(volatile uint32_t *)(EXTI_BASE_ADDR + 0x04U))
#define EXTI_RTSR                   (*(volatile uint32_t *)(EXTI_BASE_ADDR + 0x08U))
#define EXTI_FTSR                   (*(volatile uint32_t *)(EXTI_BASE_ADDR + 0x0CU))
#define EXTI_SWIER                  (*(volatile uint32_t *)(EXTI_BASE_ADDR + 0x10U))
#define EXTI_PR                     (*(volatile uint32_t *)(EXTI_BASE_ADDR + 0x14U))

/* RCC Clock configuration registers to auto-enable AFIO */
#define RCC_BASE_ADDR               (0x40021000U)
#define RCC_APB2ENR                 (*(volatile uint32_t *)(RCC_BASE_ADDR + 0x18U))

/***********************************************************************************************************************
* GLOBAL VARIABLE IMPLEMENTATION
**********************************************************************************************************************/

/***********************************************************************************************************************
* GLOBAL FUNCTIONS IMPLEMENTATION
**********************************************************************************************************************/

/**
 * @brief  Initializes the GPIO pins according to the specified parameters
 *         in the GPIO_Init structure.
 * @param  GPIOx: where x can be (A..E) to select the GPIO peripheral.
 * @param  GPIO_Init: pointer to a GPIO_InitTypeDef structure that contains
 *         the configuration information for the specified GPIO pins.
 * @retval None
 */
void GPIO_Init_t(GPIOx_typedef_t *GPIOx, GPIOx_InitTypeDef_t *GPIO_Init)
{

   /* Position index used to iterate through GPIO pins (0 -> 15) */
   uint32_t position = 0x00U;

   /* Bit mask representing the current pin position */
   uint32_t ioposition = 0x00U;

   /* Stores the current selected pin extracted from GPIO_Pin */
   uint32_t iocurrent = 0x00U;

   /* Stores MODE and CNF configuration bits for the current pin */
   uint32_t configbits = 0x00U;

   /* Pointer to GPIO configuration register (CRL for pins 0–7, CRH for pins 8–15) */
   volatile uint32_t *configregister;

   /* Check the parameters */
   assert_param(IS_GPIO_ALL_INSTANCE(GPIOx));
   assert_param(IS_GPIO_PIN(GPIO_Init->Pin));
   assert_param(IS_GPIO_MODE(GPIO_Init->Mode));

   /* Iterate through each bit in the Pin parameter to configure */
   for (position = 0U; position < 16U; position++)
   {
        /* assume position = 3 -> ioposition = 0x0008 */
       ioposition = (0x01U << position);
       
       /* assume pin = 0x0008 -> iocurrent = 0x0008 &  0x0008 = 0x0008 */
       iocurrent = (uint32_t)(GPIO_Init->Pin) & ioposition;

       /* Valid pin */
       if (iocurrent == ioposition)
       {
           /* Determine whether the control register is CRL (Pins 0–7) or CRH (Pins 8–15) */
           configregister = (position < 8U) ? &GPIOx->CRL : &GPIOx->CRH;
           uint32_t bitshift = (position < 8U) ? (position * 4U) : ((position - 8U) * 4U);

           /* Clear the old 4-bit configuration (CNF and MODE) of the current pin */
           *configregister &= ~(0x0FU << bitshift);

           /**
            * @note  GPIO mode encoding:
            *        - Bit [28] = Mode category.
            *          0: Basic GPIO mode (Input, Output, Analog, Alternate Function)
            *          1: Special mode (EXTI/Event)
            *
            *        The driver uses GPIOx_MODE_CATEGORY_MASK to distinguish
            *        between basic GPIO modes and EXTI/Event modes.
            */
           if ((GPIO_Init->Mode & GPIOx_MODE_CATEGORY_MASK) == 0x00000000U)
           {
               switch (GPIO_Init->Mode)
               {
                   /* CNF = 00, MODE = Speed */
                   case GPIOx_MODE_OUTPUT_PP:
                       configbits = GPIOx_BIT_MODE_OUTPUT_PP | GPIO_Init->Speed;
                       break;
                   /* CNF = 01, MODE = Speed */
                   case GPIOx_MODE_OUTPUT_OD:
                       configbits = GPIOx_BIT_MODE_OUTPUT_OD | GPIO_Init->Speed;
                       break;
                   /* CNF = 10, MODE = Speed */
                   case GPIOx_MODE_AF_PP:
                       configbits = GPIOx_BIT_MODE_AF_PP | GPIO_Init->Speed;
                       break;
                   /* CNF = 11, MODE = Speed */
                   case GPIOx_MODE_AF_OD:
                       configbits = GPIOx_BIT_MODE_AF_OD | GPIO_Init->Speed;
                       break;
                   /* CNF = 00, MODE = 00 */
                   case GPIOx_MODE_ANALOG:
                       configbits = GPIOx_BIT_MODE_ANALOG | 0x00U;
                       break;
                   case GPIOx_MODE_INPUT:
                   default:
                       /* Floating Input: CNF = 01, MODE = 00 */
                       if (GPIO_Init->Pull == GPIOx_NOPULL)
                       {
                           configbits = GPIOx_BIT_MODE_FLOATING_IN | 0x00U;
                       }
                       else
                       {
                          /* Pull-up/down Input: CNF = 10 , MODE = 00*/
                           configbits = GPIOx_BIT_MODE_PP_IN | 0x00U;

                           if (GPIO_Init->Pull == GPIOx_PULLUP)
                           {
                               /* PULLUP: Set bit ODR */
                               GPIOx->ODR |= ioposition;
                           }
                           else
                           {
                               /* PULLDOWN: Clear bit ODR */
                               GPIOx->ODR &= ~ioposition;
                           }
                       }
                       break;
               }
               *configregister |= (configbits << bitshift);
           }
           /* Configure advanced mode (External Interrupt EXTI / External Event EVT) */
           else
           {
               /* Set the hardware pin to Input Pull-up or Pull-down/Floating for customization */
               if (GPIO_Init->Pull == GPIOx_NOPULL)
               {
                   /* Floating Input */
                   configbits = GPIOx_BIT_MODE_FLOATING_IN | 0x00U;
               }
               else
               {
                   /* Pull Input */
                   configbits = GPIOx_BIT_MODE_PP_IN | 0x00U;
                   if (GPIO_Init->Pull == GPIOx_PULLUP)
                   {
                       GPIOx->ODR |= ioposition;
                   }
                   else
                   {
                       GPIOx->ODR &= ~ioposition;
                   }
               }

               *configregister |= (configbits << bitshift);

               /* Enable Clock for AFIO peripherals to configure interrupt routing */
               HAL_CLOCK_APB2_Peripheral_Enable(RCCx_APB2ENR_Index_AFIO);

               /* Configure GPIO port mapping to EXTI line via AFIO_EXTICR registers */
               uint32_t crindex = position >> 2U;               /* Calucalate EXTICR[x] need to write */
               uint32_t crshift = (position & 0x03U) * 4U;      /* Determine the EXTICR position */
               uint32_t portcode = 0U;                          /* Determine the port */

               if (GPIOx == GPIOAx)      portcode = 0U;
               else if (GPIOx == GPIOBx) portcode = 1U;
               else if (GPIOx == GPIOCx) portcode = 2U;
               else if (GPIOx == GPIODx) portcode = 3U;
               else if (GPIOx == GPIOEx) portcode = 4U;
            
               /* Route pin into EXTI */
               volatile uint32_t *exticr = &AFIOx->EXTICR1;
               exticr[crindex] &= ~(0x0FU << crshift);
               exticr[crindex] |= (portcode << crshift);

                /* Configure EXTI trigger edge selection.
                 * Mode encoding:
                 *   Bit[20] = 1: Enable Rising-edge trigger.
                 *   Bit[21] = 1: Enable Falling-edge trigger.
                 */
                        
                /* Configure Rising-edge trigger */
                if ((GPIO_Init->Mode & GPIOx_MODE_RISING_EDGE) != 0U)
                {
                    EXTI_RTSR |= ioposition;
                }
                else
                {
                    EXTI_RTSR &= ~ioposition;
                }
                
                /* Configure Falling-edge trigger */
                if ((GPIO_Init->Mode & GPIOx_MODE_FALLING_EDGE) != 0U)
                {
                    EXTI_FTSR |= ioposition;
                }
                else
                {
                    EXTI_FTSR &= ~ioposition;
                }

                /* Configure EXTI request type.
                 * Mode encoding:
                 *   Bit[17] = 1: Event request.
                 *   Bit[17] = 0: Interrupt request.
                 */
                if ((GPIO_Init->Mode & GPIOx_MODE_EVENT) != 0U)
                {
                    /* Enable Event request and disable Interrupt request */
                    EXTI_EMR |= ioposition;
                    EXTI_IMR &= ~ioposition;
                }
                else
                {
                    /* Enable Interrupt request and disable Event request */
                    EXTI_IMR |= ioposition;
                    EXTI_EMR &= ~ioposition;
                }
           }
       }
   }
}

/**
 * @brief  Resets the configuration of the selected GPIO pin to its default reset state.
 * @param  GPIOx: Pointer to GPIO peripheral.
 * @param  GPIO_Pin: Specifies the GPIO pin to be de-initialized.
 *         This parameter can be GPIO_PIN_x.
 * @retval None
 */
void GPIO_DeInit_t(GPIOx_typedef_t *GPIOx, uint16_t GPIO_Pin)
{
   uint32_t position;
   uint32_t ioposition;
   uint32_t iocurrent;
   volatile uint32_t *configregister;

   for (position = 0U; position < 16U; position++)
   {
       ioposition = (1U << position);
       iocurrent = (uint32_t) GPIO_Pin & ioposition;

       if (iocurrent == ioposition)
       {
           configregister = (position < 8U) ? &GPIOx->CRL : &GPIOx->CRH;
           uint32_t bitshift = (position < 8U) ? (position * 4U) : ((position - 8U) * 4U);

           /* Restore STM32F1xxx default reset state:
            Floating Input (CNF = 01, MODE = 00 -> 0x04) */
           *configregister &= ~(0x0FU << bitshift);
           *configregister |= (0x04U << bitshift);

           /* Clear EXTI configuration associated with this line */
           EXTI_IMR  &= ~ioposition;
           EXTI_EMR  &= ~ioposition;
           EXTI_RTSR &= ~ioposition;
           EXTI_FTSR &= ~ioposition;
       }
   }
}

/**
 * @brief  Sets or clears the selected GPIO pin.
 * @param  GPIOx: Pointer to GPIO peripheral.
 * @param  GPIO_Pin: Specifies the GPIO pin to be written.
 *         This parameter can be GPIO_PIN_x.
 * @param  State: Specifies the value to be written to the selected pin.
 *         This parameter can be HIGH or LOW.
 * @retval None
 */
void GPIO_WritePin_t(GPIOx_typedef_t *GPIOx, uint16_t GPIO_Pin, GPIOx_PinState_t State)
{
   if (State == GPIOx_PinState_HIGH)
   {
       GPIOx->BSRR = (uint32_t)GPIO_Pin;
   }
   else
   {
       GPIOx->BRR = (uint32_t)GPIO_Pin;
   }
}

/**
 * @brief  Toggles the specified GPIO pin.
 * @param  GPIOx: Pointer to GPIO peripheral.
 * @param  GPIO_Pin: Specifies the GPIO pin to be toggled.
 *         This parameter can be GPIO_PIN_x.
 * @retval None
 */
void GPIO_Toggle_t(GPIOx_typedef_t *GPIOx, uint16_t GPIO_Pin)
{
   uint32_t odr;

   odr = GPIOx->ODR;

   GPIOx->BSRR = ((odr & GPIO_Pin) << 16U) |
                 (~odr & GPIO_Pin);
}

/**
 * @brief  Reads the specified input GPIO pin.
 * @param  GPIOx: Pointer to GPIO peripheral.
 * @param  GPIO_Pin: Specifies the GPIO pin to be read.
 *         This parameter can be GPIO_PIN_x.
 * @retval GPIO_PinState_t: The input pin state.
 */
GPIOx_PinState_t GPIO_ReadPin_t(GPIOx_typedef_t *GPIOx, uint16_t GPIO_Pin)
{
   if ((GPIOx->IDR & GPIO_Pin) != 0x00U)
   {
       return GPIOx_PinState_HIGH;
   }
   return GPIOx_PinState_LOW;
}

/**
 * @brief  Locks GPIO pin configuration registers.
 * @param  GPIOx: Pointer to GPIO peripheral.
 * @param  GPIO_Pin: Specifies the GPIO pin to be locked.
 *         This parameter can be GPIO_PIN_x.
 * @retval GPIO_StatusTypeDef_t:
 *         - STD_OK: GPIO pin configuration locked successfully
 *         - STD_ERROR: Lock sequence failed
 */
GPIOx_StatusTypeDef_t GPIO_LockPin_t(GPIOx_typedef_t *GPIOx, uint16_t GPIO_Pin)
{
   volatile uint32_t tmp = GPIOx_LCKR_LCKK;
   tmp |= GPIO_Pin;

   /* Perform the mandatory GPIO lock sequence for STM32F1xxx */
   GPIOx->LCKR = tmp;                /* Write LCKK = 1 and selected pin */
   GPIOx->LCKR = (uint32_t) GPIO_Pin;/* Write LCKK = 0 and selected pin */
   GPIOx->LCKR = tmp;                /* Write LCKK = 1 and selected pin */

   tmp = GPIOx->LCKR;                /* First read to validate the lock sequence */
   tmp = GPIOx->LCKR;                /* Second read required to activate the lock */

   if ((GPIOx->LCKR & GPIOx_LCKR_LCKK) != 0x00U)
   {
       return STD_OK;
   }

   return STD_ERROR;
}

/**
 * @brief  Configures the peripheral pin remapping (AFIO).
 * @param  Remap: Remap option from the AFIO_Remap_t enumeration.
 * @retval None
 * @note   This function preserves the SWD/JTAG debug configuration to prevent device lockout.
 */
void AFIO_RemapConfig_t(AFIOx_Remap_t Remap)
{
   /* 1. Active clock for AFIO */
   HAL_CLOCK_APB2_Peripheral_Enable(RCCx_APB2ENR_Index_AFIO);

   /* 2. Read currenly of MARR register */
   uint32_t tmpreg = AFIOx->MAPR;

   /* 3. Read bit SWJ of register (bit [26:24]) */
   uint32_t swj_config = tmpreg & (0x07U << POSITION_GPIOx_AFIO_MAPR_SWJ);

    /**
    * @note  Encoding format:
    *        - Bits [7:4]: Peripheral group identifier.
    *          0x00 = SWJ (JTAG/SWD)
    *          0x10 = SPI1
    *          0x20 = I2C1
    *          0x30 = USART1
    *          0x40 = USART2
    *          0x50 = USART3
    *          0x60 = TIM1
    *          0x70 = TIM2
    *          0x80 = TIM3
    *          0x90 = TIM4
    *          0xA0 = CAN
    *
    *        - Bits [3:0]: Remap option within the selected group.
    *          Example:
    *          0x11 = SPI1 Full Remap
    *          0x21 = I2C1 Full Remap
    *          0x73 = TIM2 Full Remap
    *
    *        The driver uses (Remap & 0xF0U) to identify the peripheral group
    *        and (Remap & 0x0FU) to determine the remap option.
    */
   /* 4. Check whether the user wants to change the programmer configuration or the peripheral configuration */
   if (((uint32_t)Remap & GPIOx_AFIO_REMAP_GROUP_MASK) == 0x00U)
   {

       /* case 1: the user wants to change to dirrectly mode Debug SWJ */
       tmpreg &= ~(0x07U << POSITION_GPIOx_AFIO_MAPR_SWJ);          /* Clear old SWJ bits */
       tmpreg |= (uint32_t)Remap << POSITION_GPIOx_AFIO_MAPR_SWJ;   /* Write new config SWJ into bit [26:24] */
   }
   else
   {

       /* Case 2: Config function Remap for perhipheral */
       /* Clear old SWJ bits */
       tmpreg &= ~(0x07U << POSITION_GPIOx_AFIO_MAPR_SWJ);

       switch (Remap)
       {
           /* Config Remap for SPI1 (Bit 0)*/
           case AFIOx_Remap_SPI1_NONE:
               tmpreg &= ~GPIOx_BIT_AFIO_MAPR_SP1_REMAP;
               break;
           case AFIOx_Remap_SPI1_FULL:
               tmpreg |= GPIOx_BIT_AFIO_MAPR_SP1_REMAP;
               break;

           /* Config Remap for I2C1 (Bit 1) */
           case AFIOx_Remap_I2C1_NONE:
               tmpreg &= ~GPIOx_BIT_AFIO_MAPR_I2C1_REMAP;
               break;
           case AFIOx_Remap_I2C1_FULL:
               tmpreg |= GPIOx_BIT_AFIO_MAPR_I2C1_REMAP;
               break;

           /* Config Remap for USART1 (Bit 2) */
           case AFIOx_Remap_USART1_NONE:
               tmpreg &= ~GPIOx_BIT_AFIO_MAPR_USART1_REMAP;
               break;
           case AFIOx_Remap_USART1_FULL:
               tmpreg |= GPIOx_BIT_AFIO_MAPR_USART1_REMAP;
               break;

           /* Config Remap for USART2 (Bit 3) */
           case AFIOx_Remap_USART2_NONE:
               tmpreg &= ~GPIOx_BIT_AFIO_MAPR_USART2_REMAP;
               break;
           case AFIOx_Remap_USART2_FULL:
               tmpreg |= GPIOx_BIT_AFIO_MAPR_USART2_REMAP;
               break;

           /* Config Remap for USART3 (Bit[5:4]) */
           case AFIOx_Remap_USART3_NONE:
               tmpreg &= ~GPIOx_BIT_AFIO_MAPR_USART3_REMAP;
               break;
           case AFIOx_Remap_USART3_PARTIAL:
               tmpreg &= ~GPIOx_BIT_AFIO_MAPR_USART3_REMAP;
               tmpreg |= GPIOx_BIT_AFIO_MAPR_USART3_PARTIAL_REMAP;
               break;
           case AFIOx_Remap_USART3_FULL:
               tmpreg &= ~GPIOx_BIT_AFIO_MAPR_USART3_REMAP;
               tmpreg |= GPIOx_BIT_AFIO_MAPR_USART3_REMAP;
               break;

           /* Config Remap for TIM1 (Bit [7:6]) */
           case AFIOx_Remap_TIM1_NONE:
               tmpreg &= ~ GPIOx_BIT_AFIO_MAPR_TIM1_REMAP;
               break;
           case AFIOx_Remap_TIM1_PARTIAL:
               tmpreg &= ~GPIOx_BIT_AFIO_MAPR_TIM1_REMAP;
               tmpreg |= GPIOx_BIT_AFIO_MAPR_TIM1_PARTIAL_REMAP;
               break;
           case AFIOx_Remap_TIM1_FULL:
               tmpreg &= ~GPIOx_BIT_AFIO_MAPR_TIM1_REMAP;
               tmpreg |= GPIOx_BIT_AFIO_MAPR_TIM1_REMAP;
               break;

           /* Config Remap for TIM2 (Bit [9:8]) */
           case AFIOx_Remap_TIM2_NONE:
               tmpreg &= ~GPIOx_BIT_AFIO_MAPR_TIM2_REMAP;
               break;
           case AFIOx_Remap_TIM2_PARTIAL_1:
               tmpreg &= ~GPIOx_BIT_AFIO_MAPR_TIM2_REMAP;
               tmpreg |= GPIOx_BIT_AFIO_MAPR_TIM2_PARTIAL1_REMAP;
               break;
           case AFIOx_Remap_TIM2_PARTIAL_2:
               tmpreg &= ~GPIOx_BIT_AFIO_MAPR_TIM2_REMAP;
               tmpreg |= GPIOx_BIT_AFIO_MAPR_TIM2_PARTIAL2_REMAP;
               break;
           case AFIOx_Remap_TIM2_FULL:
               tmpreg &= ~GPIOx_BIT_AFIO_MAPR_TIM2_REMAP;
               tmpreg |= GPIOx_BIT_AFIO_MAPR_TIM2_REMAP;
               break;

           /* Config remap for TIM3 (BIT [11:10]) */
           /* Note: Label Partial of TIM3 ghi write value logic 10b */
           case AFIOx_Remap_TIM3_NONE:
               tmpreg &= ~GPIOx_BIT_AFIO_MAPR_TIM3_REMAP;
               break;
           case AFIOx_Remap_TIM3_PARTIAL:
               tmpreg &= ~GPIOx_BIT_AFIO_MAPR_TIM3_REMAP;
               tmpreg |= GPIOx_BIT_AFIO_MAPR_TIM3_PARITIAL_REMAP;
               break;
           case AFIOx_Remap_TIM3_FULL:
               tmpreg &= ~GPIOx_BIT_AFIO_MAPR_TIM3_REMAP;
               tmpreg |= GPIOx_BIT_AFIO_MAPR_TIM3_REMAP;
               break;

           /* Config remap for TIM4 (bit12) */
           case AFIOx_Remap_TIM4_NONE:
               tmpreg &= ~GPIOx_BIT_AFIO_MAPR_TIM4_REMAP;
               break;
           case AFIOx_Remap_TIM4_FULL:
               tmpreg |= GPIOx_BIT_AFIO_MAPR_TIM4_REMAP;
               break;

           /* Config remap for CAN1 (bit [14:13]) */
           /* Position PB8/PB9 corresponds 10b */
           case AFIOx_Remap_CAN_NONE:
               tmpreg &= ~GPIOx_BIT_AFIO_MAPR_CAN_REMAP;
               break;
           case AFIOx_Remap_CAN_PARTIAL:
               tmpreg &= ~GPIOx_BIT_AFIO_MAPR_CAN_REMAP;
               tmpreg |= GPIOx_BIT_AFIO_MAPR_PARTIAL_CAN_REMAP;
               break;
           case AFIOx_Remap_CAN_FULL:
               tmpreg &= ~GPIOx_BIT_AFIO_MAPR_CAN_REMAP;
               tmpreg |= GPIOx_BIT_AFIO_MAPR_CAN_REMAP;
               break;

           default:
               /* Nothing to do something */
               return;
       }

       /* Restore the previous SWJ configuration. */
       tmpreg |= swj_config;
   }

   /* 5. Write the safely calculated value back to the hardware register. */
   AFIOx->MAPR = tmpreg;

}

/**
 * @brief  Handles GPIO EXTI interrupt requests.
 *         This function should be called from the corresponding EXTIx_IRQHandler().
 * @param  GPIO_Pin: GPIO pin connected to the EXTI line.
 *         This parameter can be GPIO_PIN_x.
 * @retval None
 * @note   Call this function when using a custom interrupt vector.
 */
void GPIO_EXTI_IRQHandler_t(uint16_t GPIO_Pin)
{
    /* Check if the interrupt pending flag is set */
    if ((EXTI_PR & GPIO_Pin) != 0x00U)
    {
        /* Clear the interrupt pending flag by writing 1 */
        EXTI_PR = GPIO_Pin;

        /* Call the user callback */
        GPIO_EXTI_Callback_t(GPIO_Pin);
    }
}

/**
 * @brief  Weak interrupt callback function.
 * @param  GPIO_Pin: GPIO pin that triggered the interrupt.
 * @retval None
 * @note   Call this function when using a custom interrupt vector.
 */
void GPIO_EXTI_Callback_t(uint16_t GPIO_Pin)
{
   /* Avoid compiler warning for unused parameter */
   (void)GPIO_Pin;
}
