/*
 * GPIO_drv.c
 *
 *  Created on: Oct 29, 2024
 *      Author: Admin
 */

#include "GPIOx.h"
//#include <stdint.h>
//#include "stm32f103xb.h"

void GPIOx_Init(GPIO_TypeDef * GPIOx, uint8_t pin, uint8_t mode, uint8_t pull, uint8_t speed)
{
  if (GPIOx == GPIOA)
  {
	/* port A clock enable */
    RCC->APB2ENR|= 1<<2;
  }
  else if (GPIOx == GPIOB)
  {
	  /* port B clock enable */
    RCC->APB2ENR|= 1<<3;
  }
  else if (GPIOx == GPIOC)
  {
	/* port C clock enable */
    RCC->APB2ENR|= 1<<4;
  }
  else if (GPIOx == GPIOD)
  {
	/* port D clock enable */
	RCC->APB2ENR|= 1<<5;
  }
  else
  {
	//do some thing
  }

  if ( pin < 8)
  {
	  /* clear bit */
	  GPIOx->CRL &= ~(0xf << (pin *4));
	  GPIOx->CRL |= ( mode<<2 | speed) << (pin *4);

  }
  else
  {
	  GPIOx->CRH &= ~(0xf << ((pin-8) *4));
	  GPIOx->CRH |= ( mode<<2 | speed) << ((pin-8) *4);
  }

  if (pull == PULL_UP)
  {
	  /* Pull up */
	  GPIOx->ODR|= 1<< (pin);
  }
  else if ( pull == PULL_DOWN)
  {
	  /* pull down */
	  GPIOx->ODR|= 0<< (pin);
  }
}


uint8_t GPIOx_Read(GPIO_TypeDef *GPIOx, uint16_t pin_number)
{
    return ((GPIOx->IDR & (1 << pin_number)) !=0) ? 1 : 0;
}


void GPIOx_Write(GPIO_TypeDef *GPIOx, uint16_t pin_number, uint8_t state)
{
    if (state == 1)
    {
    	/* set pin pin_number high */
        GPIOx->BSRR = (1 << pin_number);
    }
    else
    {
    	/* set pin pin_number low */
        GPIOx->BSRR = (1 << (pin_number+16));
    }
}


void GPIOx_Toggle(GPIO_TypeDef *GPIOx, uint16_t pin_number)
{
	/* Use XOR allowed to toggle pin_number bit in ODR register */
    GPIOx->ODR ^= (1 << pin_number);
}
