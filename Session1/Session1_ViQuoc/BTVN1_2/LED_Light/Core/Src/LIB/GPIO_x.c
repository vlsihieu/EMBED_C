/*
 * GPIO_x.c
 *
 * Created on: May 20, 2026
 * Author: quocvi
 */

#include "GPIO_x.h"

void GPIOx_Init(GPIO_TypeDef * GPIOx, uint8_t pin, uint8_t mode, uint8_t pull, uint8_t speed)
{
    // Bật xung nhịp (Clock enable) cho các Port tương ứng
    if (GPIOx == GPIOA)
    {
        RCC->APB2ENR |= (1 << 2);
    }
    else if (GPIOx == GPIOB)
    {
        RCC->APB2ENR |= (1 << 3);
    }
    else if (GPIOx == GPIOC)
    {
        RCC->APB2ENR |= (1 << 4);
    }
    else if (GPIOx == GPIOD)
    {
        RCC->APB2ENR |= (1 << 5);
    }

    // Cấu hình Mode và Speed vào thanh ghi CRL (Pin 0-7) hoặc CRH (Pin 8-15)
    if (pin < 8)
    {
        /* Xóa 4 bit cấu hình cũ của chân pin */
        GPIOx->CRL &= ~(0xF << (pin * 4));
        /* Ghi cấu hình mới vào */
        GPIOx->CRL |= ((mode << 2) | speed) << (pin * 4);
    }
    else
    {
        /* Xóa 4 bit cấu hình cũ của chân pin */
        GPIOx->CRH &= ~(0xF << ((pin - 8) * 4));
        /* Ghi cấu hình mới vào */
        GPIOx->CRH |= ((mode << 2) | speed) << ((pin - 8) * 4);
    }

    // Cấu hình điện trở kéo (Pull-up / Pull-down) qua thanh ghi ODR
    if (pull == PULL_UP)
    {
        /* Set bit lên 1 để Pull-up */
        GPIOx->ODR |= (1 << pin);
    }
    else if (pull == PULL_DOWN)
    {
        GPIOx->ODR &= ~(1 << pin);
    }
}

uint8_t GPIOx_Read(GPIO_TypeDef *GPIOx, uint16_t pin_number)
{
    /* Đọc thanh ghi IDR, dịch mặt nạ để lấy đúng bit của chân cần đọc */
    return ((GPIOx->IDR & (1 << pin_number)) != 0) ? 1 : 0;
}

void GPIOx_Write(GPIO_TypeDef *GPIOx, uint16_t pin_number, uint8_t state)
{
    if (state == 1)
    {
        /* Set pin high: Ghi số 1 vào nửa dưới của thanh ghi BSRR */
        GPIOx->BSRR = (1 << pin_number);
    }
    else
    {
        /* Set pin low: Ghi số 1 vào nửa trên (bit 16-31) của thanh ghi BSRR */
        GPIOx->BSRR = (1 << (pin_number + 16));
    }
}

void GPIOx_Toggle(GPIO_TypeDef *GPIOx, uint16_t pin_number)
{
    /* Dùng phép XOR (^) để lật trạng thái bit trong thanh ghi ODR */
    GPIOx->ODR ^= (1 << pin_number);
}
