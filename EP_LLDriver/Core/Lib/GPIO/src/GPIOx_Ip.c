/*
 * GPIOx.c
 *
 *  Created on: May 6, 2026
 *      Author: hieuk
 */

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 *                                                     INCLUDES
 **********************************************************************************************************************/
#include <GPIOx_Ip.h>


/***********************************************************************************************************************
 *                                                   LOCAL MACROS
 **********************************************************************************************************************/

#define  GPIOx_CR_MODE_INPUT         0x00000000u    ///< 00: Input mode (reset state)
#define  GPIOx_CR_CNF_ANALOG         0x00000000u    ///< 00: Analog mode
#define  GPIOx_CR_CNF_INPUT_FLOATING 0x00000004u    ///< 01: Floating input (reset state)
#define  GPIOx_CR_CNF_INPUT_PU_PD    0x00000008u    ///< 10: Input with pull-up / pull-down 
#define  GPIOx_CR_CNF_GP_OUTPUT_PP   0x00000000u    ///< 00: General purpose output push-pull
#define  GPIOx_CR_CNF_GP_OUTPUT_OD   0x00000004u    ///< 01: General purpose output Open-drain 
#define  GPIOx_CR_CNF_AF_OUTPUT_PP   0x00000008u    ///< 10: Alternate function output Push-pull 
#define  GPIOx_CR_CNF_AF_OUTPUT_OD   0x0000000Cu    ///< 11: Alternate function output Open-drain

/***********************************************************************************************************************
 *                                                      EXTERN
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                       LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                                  LOCAL CONSTANTS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                                  LOCAL VARIABLES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                                 GLOBAL VARIABLES
 **********************************************************************************************************************/
 
/***********************************************************************************************************************
 *                                             LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                                  LOCAL FUNCTION
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *                                                  GLOBAL FUNCTION
 **********************************************************************************************************************/

/**
 * @brief Initialize GPIO pin configuration.
 * @param GPIOx GPIO port.
 * @param GPIO_Init Pointer to GPIO configuration structure.
 */
void GPIO_Init_t(GPIO_typedef_t *GPIOx, GPIO_InitTypeDef_t * GPIO_Init)
{
    /* Enable clock for Port x */
    if (GPIOx == GPIOAx)
    {
        /* Enable clock for Port A */
        RCC_APB2_Peripheral(IOPA_EN);
    }
    else if (GPIOx == GPIOBx)
    {
        /* ENable clock for Port B */
        RCC_APB2_Peripheral(IOPB_EN);
    }
    else if (GPIOx == GPIOCx)
    {
        /* ENable clock for Port C */
        RCC_APB2_Peripheral(IOPC_EN);
    }
    else if (GPIOx == GPIODx)
    {
        /* ENable clock for Port D */
        RCC_APB2_Peripheral(IOPD_EN);
    }
    else if (GPIOx == GPIOEx)
    {
        /* ENable clock for Port E */
        RCC_APB2_Peripheral(IOPE_EN);
    }
    else if (GPIOx == GPIOFx)
    {
        /* ENable clock for Port F */
        RCC_APB2_Peripheral(IOPF_EN);
    }
    else
    {
        /* Do somthing */
    }

    /* Enable clock for AFIO */
    if (GPIO_Init->Mode == AF_OUTPUT_PUSH_PULL || GPIO_Init->Mode == AF_OUTPUT_OPEN_DRAIN)
    {
        RCC_APB2_Peripheral(AFIO_EN);
    }

    /* Disable JTAG to use PB3/PB4 as GPIO, keep SWD enabled */
	if (GPIOx == GPIOBx)
    {
		if (GPIO_Init->Pin == GPIO_PIN_3 || GPIO_Init->Pin == GPIO_PIN_4) 
        {
			RCC_APB2_Peripheral(AFIO_EN);
			AFIOx->MAPR |= (0b010 << 24);
		}
	}

    /* Disable JTAG to use PA15 as GPIO, keep SWD enabled */
	if(GPIOx == GPIOAx)
    {
		if (GPIO_Init->Pin == GPIO_PIN_15)
        {
			RCC_APB2_Peripheral(AFIO_EN);
			AFIOx->MAPR |= (0b010 << 24);
		}
	}

    /*
    * CRL is used to configure GPIO pins 0 → 7
    * CRH is used to configure GPIO pins 8 → 15
    * These registers are used to configure:
    * Input / Output
    * Push-pull / Open-drain
    * Analog / Floating / Pull-up Pull-down
    * Output speed (10MHz, 2MHz, 50MHz)
    * Each pin uses 4 configuration bits in CRL/CRH.
    */
    if (GPIO_Init->Pin < GPIO_PIN_7)
    {

        switch (GPIO_Init->Mode)
        {
        case OUTPUT_PUSH_PULL :
            /*
            * Output mode, max speed 50Mhz
            * General purpose output push-pull  
            */
            GPIOx->CRL &= ~(0b1111 << (GPIO_Init->Pin * 4));
            GPIOx->CRL |= ((GPIO_Init->Speed | GPIOx_CR_CNF_GP_OUTPUT_PP) 
                << (GPIO_Init->Pin * 4));
            break;
        case AF_OUTPUT_PUSH_PULL:
            /*
            * Output mode, max speed 50MHz
            * Alternate function output Push-pull
            */
            GPIOx->CRL &= ~(0b1111 << (GPIO_Init->Pin * 4));
            GPIOx->CRL |= ((GPIO_Init->Speed | GPIOx_CR_CNF_AF_OUTPUT_PP)
                << (GPIO_Init->Pin * 4));
            break;

        case OUTPUT_OPEN_DRAIN:
            /*
            * Output mode, max speed 50MHz
            * General purpose output Open-drain
            */
            GPIOx->CRL &= ~(0b1111 << (GPIO_Init->Pin * 4));
            GPIOx->CRL |= ((GPIO_Init->Speed | GPIOx_CR_CNF_GP_OUTPUT_OD)
                << (GPIO_Init->Pin * 4));
            break;

        case AF_OUTPUT_OPEN_DRAIN:
            /*
            * Output mode, max speed 50MHz
            * Alternate function output Open-drain
            */
            GPIOx->CRL &= ~(0b1111 << (GPIO_Init->Pin * 4));
            GPIOx->CRL |= ((GPIO_Init->Speed | GPIOx_CR_CNF_AF_OUTPUT_OD)
                << (GPIO_Init->Pin * 4));
            break;
        case INPUT_PULLUP:
            /*
            * Input mode
            * Input with pull-up / pull-down
            */
            GPIOx->CRL &= ~(0b1111 << (GPIO_Init->Pin * 4));
            GPIOx->CRL |= (GPIOx_CR_MODE_INPUT | GPIOx_CR_CNF_INPUT_PU_PD)
                << (GPIO_Init->Pin * 4);
            break;
        case INPUT_FLOATING:
            /*
            * Input mode
            * Floating input
            */
            GPIOx->CRL &= ~(0b1111 << (GPIO_Init->Pin * 4));
            GPIOx->CRL |= (GPIOx_CR_MODE_INPUT | GPIOx_CR_CNF_INPUT_FLOATING)
                            << (GPIO_Init->Pin * 4);
            break;
        case INPUT_ANALOG:
            /*
            * Input mode
            * Analog input
            */
            GPIOx->CRL &= ~(0b1111 << (GPIO_Init->Pin * 4));
            GPIOx->CRL |= (GPIOx_CR_MODE_INPUT | GPIOx_CR_CNF_ANALOG)
                << (GPIO_Init->Pin * 4);
            break;
        default:
            break;
        }
    }
    else
    {
        switch (GPIO_Init->Mode)
        {
        case OUTPUT_PUSH_PULL:
            /*
            * Output mode, max speed 50Mhz
            * General purpose output push-pull  
            */
            GPIOx->CRL &= ~(0b1111 << ((GPIO_Init->Pin -8 )* 4));
            GPIOx->CRL |= ((GPIO_Init->Speed | GPIOx_CR_CNF_GP_OUTPUT_PP) 
                << ((GPIO_Init->Pin - 8)* 4));
            break;
        case AF_OUTPUT_PUSH_PULL:
            /*
            * Output mode, max speed 50MHz
            * Alternate function output Push-pull
            */
            GPIOx->CRL &= ~(0b1111 << ((GPIO_Init->Pin - 8)* 4));
            GPIOx->CRL |= ((GPIO_Init->Speed | GPIOx_CR_CNF_AF_OUTPUT_PP)
                << ((GPIO_Init->Pin - 8)* 4));
            break;
        case OUTPUT_OPEN_DRAIN:
            /*
            * Output mode, max speed 50MHz
            * General purpose output Open-drain
            */
            GPIOx->CRL &= ~(0b1111 << ((GPIO_Init->Pin - 8) * 4));
            GPIOx->CRL |= ((GPIO_Init->Speed | GPIOx_CR_CNF_GP_OUTPUT_OD)
                << ((GPIO_Init->Pin - 8) * 4));
            break;
        case AF_OUTPUT_OPEN_DRAIN:
            /*
            * Output mode, max speed 50MHz
            * Alternate function output Open-drain
            */
            GPIOx->CRL &= ~(0b1111 << ((GPIO_Init->Pin-8) * 4));
            GPIOx->CRL |= ((GPIO_Init->Speed | GPIOx_CR_CNF_AF_OUTPUT_OD)
                << ((GPIO_Init->Pin - 8)* 4));
            break;
        case INPUT_PULLUP:
            /*
            * Input mode
            * Input with pull-up / pull-down
            */
            GPIOx->CRL &= ~(0b1111 << ((GPIO_Init->Pin-8) * 4));
            GPIOx->CRL |= (GPIOx_CR_MODE_INPUT | GPIOx_CR_CNF_INPUT_PU_PD)
                << ((GPIO_Init->Pin-8) * 4);
            break;
        case INPUT_FLOATING:
            /*
            * Input mode
            * Floating input
            */
            GPIOx->CRL &= ~(0b1111 << ((GPIO_Init->Pin-8) * 4));
            GPIOx->CRL |= (GPIOx_CR_MODE_INPUT | GPIOx_CR_CNF_INPUT_FLOATING)
                            << ((GPIO_Init->Pin-8) * 4);
            break;
        case INPUT_ANALOG:
            /*
            * Input mode
            * Analog input
            */
            GPIOx->CRL &= ~(0b1111 << ((GPIO_Init->Pin-8) * 4));
            GPIOx->CRL |= (GPIOx_CR_MODE_INPUT | GPIOx_CR_CNF_ANALOG)
                << ((GPIO_Init->Pin-8) * 4);
            break;
        default:
            break;
        }
    }

    /* Pull-Up/PUll-Down */
    if (GPIO_Init->Pull == INPUT_PULLUP)
    {
         GPIOx->ODR|= 1<< (GPIO_Init->Pin);
    }
    else if (GPIO_Init->Pull == INPUT_PULLDOWN)
    {
        GPIOx->ODR|= 0<< (GPIO_Init->Pin);
    }

}

/**
 * @brief Write logic level to a GPIO pin.
 * @param GPIOx GPIO port.
 * @param GPIO_Pin Pin number.
 * @param State Pin state (LOW/HIGH).
 */
void GPIO_WritePin_t(GPIO_typedef_t *GPIOx, uint16_t GPIO_Pin, GPIO_PinState_t State)
{
    /* check the parameters */
    assert_param(ASSET_GPIO_PIN(GPIO_Pin));
    assert_param(ASSET_GPIO_PIN_ACTION(State));

    if (State != LOW)
    {
    	  GPIOx->BSRR |= (1 << GPIO_Pin);
    }
    else
    {
    	  GPIOx->BSRR &= ~(1 << GPIO_Pin);
    }
}

/**
 * @brief Toggle GPIO pin state.
 * @param GPIOx GPIO port.
 * @param GPIO_Pin Pin number.
 */
void GPIO_Toggle_t(GPIO_typedef_t *GPIOx, uint16_t GPIO_Pin)
{

    /* check the parameters */
    assert_param(ASSET_GPIO_PIN(GPIO_Pin));


    GPIOx->ODR ^= (1 << GPIO_Pin);
}

/**
 * @brief Read GPIO pin state.
 * @param GPIOx GPIO port.
 * @param GPIO_Pin Pin number.
 * @retval 0: LOW, 1: HIGH
 */
GPIO_PinState_t GPIO_ReadPin_t(GPIO_typedef_t *GPIOx, uint16_t GPIO_Pin)
{
    GPIO_PinState_t bitstatus;

    /* check the parameters */
    assert_param(ASSET_GPIO_PIN(GPIO_Pin));

    if ((GPIOx->IDR & GPIO_Pin) != (uint32_t)LOW)
    {
      bitstatus = HIGH;
    }
    else
    {
      bitstatus = LOW;
    }
    return bitstatus;
}

/**
 * @brief  Locks GPIO Pins configuration registers.
 * @note   The locking mechanism allows the IO configuration to be frozen. When the LOCK sequence
 *         has been applied on a port bit, it is no longer possible to modify the value of the port bit until
 *         the next reset.
 * @param  GPIOx: where x can be (A..G depending on device used) to select the GPIO peripheral
 * @param  GPIO_Pin: specifies the port bit to be locked.
 *         This parameter can be any combination of GPIO_PIN_x where x can be (0..15).
 * @retval GPIO_StatusTypeDef_t
 */
GPIO_StatusTypeDef_t GPIO_LockPin(GPIO_typedef_t *GPIOx, uint16_t GPIO_Pin)
{
    volatile uint32_t tmp;

    /* check the parameters */
    assert_param(ASSET_GPIO_AF_INSTANCE(GPIOx));
    assert_param(ASSET_GPIO_PIN(GPIO_Pin));

    /* Apply lock key write sequence */
    tmp = (1UL << GPIO_Pin);

    /* Set LCKx bit(s): LCKK='1' + LCK[15:0] */
    tmp = (1UL << 16U) | tmp;
    GPIOx->LCKR = tmp;       

    /* Reset LCKx bit(s): LCKK='0' + LCK[15:0] */
    GPIOx->LCKR = tmp & ~(1UL << 16U);

    /* Set LCKx bit(s): LCKK='1' + LCK[15:0] */
    GPIOx->LCKR = tmp;

    /* Read LCKK register. This read is mandatory to complete key lock sequence */
    tmp = GPIOx->LCKR;

    /* Read again in order to confirm lock is active */
    if ((uint32_t)(GPIOx->LCKR & GPIOx_LCKR_LCKK))
    {
        return STD_OK;
    }
    else
    {
        return STD_ERROR;
    }
}




#ifdef __cplusplus
}
#endif

/*--------------------------------------------------- EOF ------------------------------------------------------------*/
