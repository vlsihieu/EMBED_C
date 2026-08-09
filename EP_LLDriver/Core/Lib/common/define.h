/*
 * Define.h
 *
 *  Created on: May 6, 2026
 *      Author: hieuk
 */

#ifndef LIB_COMMON_INC_DEFINE_H_
#define LIB_COMMON_INC_DEFINE_H_


/*
 * make library by ChucDao
 * congchuc289@gmail.com
 */
#ifndef _LIBRARY_DEFINE_H_
#define _LIBRARY_DEFINE_H_

//#define FSMxC_Address     									0xA0000000
//#define USBx_OTG_FS_Address      						    0x50000000
//#define Ethernetx_Address       								0x40028000
//#define CRCx_Address             							0x40023000
//#define Flashx_Address   				         			0x40022000
//#define RCCx_Address 										0x40021000
//#define DMA2x_Address                						0x40020400
//#define DMA1x_Address               							0x40020000
//#define SDIOx_Address               							0x40018000
//#define TIM11x_timer_Address      							0x40015400
//#define TIM10x_timer_Address        							0x40015000
//#define TIM9x_timer_Address            						0x40014C00
//#define ADC3x_Address               							0x40013C00
//#define USART1x_Address                						0x40013800
//#define TIM8x_timer_Address            						0x40013400
//#define SPI1x_Address                        				0x40013000

//#define ADC2x_Address                         				0x40012800
//#define ADC1x_Address                     					0x40012400
#define RCCx_Address 										    0x40021000
#define GPIOx_Portx_G_Address               0x40012000
#define GPIOx_Portx_F_Address               0x40011C00
#define GPIOx_Portx_E_Address               0x40011800
#define GPIOx_Portx_D_Address               0x40011400
#define GPIOx_Portx_C_Address               0x40011000
#define GPIOx_Portx_B_Address               0x40010C00
#define GPIOx_Portx_A_Address               0x40010800
#define AFIOx_Address                       0x40010000
#define Flashx_Address   				         	  0x40022000
#define TIM14x_timer_Address                0x40002000
#define TIM13x_timer_Address                0x40001C00
#define TIM12x_timer_Address                0x40001800
#define TIM7x_timer_Address                 0x40001400
#define TIM6x_timer_Address                 0x40001000
#define TIM5x_timer_Address                 0x40000C00
#define TIM4x_timer_Address                 0x40000800
#define TIM3x_timer_Address                 0x40000400
#define TIM2x_timer_Address                 0x40000000
#define TIM1x_timer_Address                       0x40012C00
//#define EXTIx_Address                             			0x40010400
//#define AFIOx_Address                                   	 	0x40010000
//#define DACx_Address                                  		0x40007400
//#define Powerx_control_PWR_Address                       	0x40007000
//#define BKP_Address                                  		0x40006C00
//#define bxCAN1_Address                               		0x40006400
//#define bxCAN2_Address                                 		0x40006800
//#define Shared_USB_CAN_SRAM_512_bytes_Address            	0x40006000
//#define USB_device_FS_registers_Address                     0x40005C00
//#define I2C2_Address                                        0x40005800
//#define I2C1_Address                                        0x40005400
//#define UART5_Address                                       0x40005000
//#define UART4_Address                                    	0x40004C00
//#define USART3_Address                             			0x40004800
//#define USART2_Address                             			0x40004400
//#define SPI3_I2S_Address                            		0x40003C00
//#define SPI2_I2S_Address                              		0x40003800
//#define IWDG_Address                                   		0x40003000
//#define WWDG_Address                                    	0x40002C00
//#define RTC_Address                                        	0x40002800


typedef enum
{
  STD_OK       = 0x00U,
  STD_ERROR    = 0x01U,
  STD_BUSY     = 0x02U,
  STD_TIMEOUT  = 0x03U
} HALx_StatusTypeDef;

/**
* @brief GPIO register structure definition.
*/
typedef struct
{
   volatile uint32_t CRL;      /* GPIO config (Pin 0~7)  */
   volatile uint32_t CRH;      /* GPIO config (Pin 8~15) */
   volatile uint32_t IDR;      /* Read input state       */
   volatile uint32_t ODR;      /* Read/write output      */
   volatile uint32_t BSRR;     /* Atomic set/reset       */
   volatile uint32_t BRR;      /* Reset output           */
   volatile uint32_t LCKR;     /* Lock configuration     */
} GPIOx_typedef_t;

/**
* @brief AFIO register structure definition.
*/
typedef struct
{
   volatile uint32_t EVCR;      /* Event control         */
   volatile uint32_t MAPR;      /* Remap configuration   */
   volatile uint32_t EXTICR1;   /* EXTI config (0~3)     */
   volatile uint32_t EXTICR2;   /* EXTI config (4~7)     */
   volatile uint32_t EXTICR3;   /* EXTI config (8~11)    */
   volatile uint32_t EXTICR4;   /* EXTI config (12~15)   */
   volatile uint32_t MAPR2;     /* Additional remap      */
} AFIOx_typedef_t;

/**
* @brief TIMER register structure definition.
*/
typedef struct
{
    __IO uint32_t CR1;
    __IO uint32_t CR2;
    __IO uint32_t SMCR;
    __IO uint32_t DIER;
    __IO uint32_t SR;
    __IO uint32_t EGR;
    __IO uint32_t CCMR1;
    __IO uint32_t CCMR2;
    __IO uint32_t CCER;
    __IO uint32_t CNT;
    __IO uint32_t PSC;
    __IO uint32_t ARR;
    __IO uint32_t RCR;
    __IO uint32_t CCR1;
    __IO uint32_t CCR2;
    __IO uint32_t CCR3;
    __IO uint32_t CCR4;
    __IO uint32_t BDTR;
    __IO uint32_t DCR;
    __IO uint32_t DMAR;
} TIMx_TypeDef_t;


#define TIM1x   ((TIMx_TypeDef_t *) TIM1x_timer_Address)
#define TIM2x   ((TIMx_TypeDef_t *) TIM2x_timer_Address)
#define TIM3x   ((TIMx_TypeDef_t *) TIM3x_timer_Address)
#define TIM4x   ((TIMx_TypeDef_t *) TIM4x_timer_Address)
#define TIM5x   ((TIMx_TypeDef_t *) TIM5x_timer_Address)
#define TIM6x   ((TIMx_TypeDef_t *) TIM6x_timer_Address)
#define TIM7x   ((TIMx_TypeDef_t *) TIM7x_timer_Address)
#define TIM12x  ((TIMx_TypeDef_t *) TIM12x_timer_Address)
#define TIM13x  ((TIMx_TypeDef_t *) TIM13x_timer_Address)
#define TIM14x  ((TIMx_TypeDef_t *) TIM14x_timer_Address)


#define GPIOAx  ((GPIOx_typedef_t *)GPIOx_Portx_A_Address)
#define GPIOBx  ((GPIOx_typedef_t *)GPIOx_Portx_B_Address)
#define GPIOCx  ((GPIOx_typedef_t *)GPIOx_Portx_C_Address)
#define GPIODx  ((GPIOx_typedef_t *)GPIOx_Portx_D_Address)
#define GPIOEx  ((GPIOx_typedef_t *)GPIOx_Portx_E_Address)
#define AFIOx   ((AFIOx_typedef_t *)AFIOx_Address)

/* Exported macro ------------------------------------------------------------*/
#ifdef  USE_FULL_ASSERT
/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr If expr is false, it calls assert_failed function
  *         which reports the name of the source file and the source
  *         line number of the call that failed.
  *         If expr is true, it returns no value.
  * @retval None
  */
#define assert_param(expr) ((expr) ? (void)0U : assert_failed((uint8_t *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- */
void assert_failed(uint8_t* file, uint32_t line);
#else
#define assert_param(expr) ((void)0U)
#endif /* USE_FULL_ASSERT */


#endif /* MY_LIBRARY_DEFINE_H_ */


#endif /* LIB_COMMON_INC_DEFINE_H_ */
