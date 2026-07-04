///*
// * hal_timer.h
// *
// *  Created on: Jun 9, 2026
// *      Author: hieuk
// */
//
//#ifndef LIB_TIMER_HAL_TIMER_H_
//#define LIB_TIMER_HAL_TIMER_H_
//
///*
// * Timer_Basic.h
// *
// * Created on: May 6, 2026
// * Author: hieuk
// */
//
//#ifdef __cplusplus
//extern "C" {
//#endif
//
///***********************************************************************************************************************
// * INCLUDES AND DEFINITIONS
// **********************************************************************************************************************/
//
//#include <stdint.h>
//#include "Define.h"
//
///***********************************************************************************************************************
// * LOCAL MACROS
// **********************************************************************************************************************/
//
///**
// * @brief Check if TIM instance is valid.
// */
//#define ASSET_TIM_ALL_INSTANCE(INSTANCE) (((INSTANCE) == TIM1x) || \
//                                          ((INSTANCE) == TIM2x) || \
//                                          ((INSTANCE) == TIM3x) || \
//                                          ((INSTANCE) == TIM4x) || \
//                                          ((INSTANCE) == TIM5x) || \
//                                          ((INSTANCE) == TIM6x) || \
//                                          ((INSTANCE) == TIM7x))
//
///**
// * @brief Base hardware addresses mappings for STM32F103 Timers
// */
//#define TIM1_Base_Address       (0x40012C00U)
//#define TIM2_Base_Address       (0x40000000U)
//#define TIM3_Base_Address       (0x40000400U)
//#define TIM4_Base_Address       (0x40000800U)
//#define TIM5_Base_Address       (0x40000C00U)
//#define TIM6_Base_Address       (0x40001000U)
//#define TIM7_Base_Address       (0x40001400U)
//
//#define TIM1x                   ((TIM_typedef_t *)TIM1_Base_Address)
//#define TIM2x                   ((TIM_typedef_t *)TIM2_Base_Address)
//#define TIM3x                   ((TIM_typedef_t *)TIM3_Base_Address)
//#define TIM4x                   ((TIM_typedef_t *)TIM4_Base_Address)
//#define TIM5x                   ((TIM_typedef_t *)TIM5_Base_Address)
//#define TIM6x                   ((TIM_typedef_t *)TIM6_Base_Address)
//#define TIM7x                   ((TIM_typedef_t *)TIM7_Base_Address)
//
///***********************************************************************************************************************
// * LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
// **********************************************************************************************************************/
//
///**
// * @brief HAL Timer status return type.
// */
//typedef enum
//{
//  HAL_TIM_OK       = 0x00U,
//  HAL_TIM_ERROR    = 0x01U,
//  HAL_TIM_BUSY     = 0x02U,
//  HAL_TIM_TIMEOUT  = 0x03U
//} HAL_TIM_StatusTypeDef_t;
//
///**
// * @brief Timer Configuration Structure.
// */
//typedef struct
//{
//  uint32_t Prescaler;     /**< Specifies the prescaler value used to divide the TIM clock. (0 to 65535) */
//  uint32_t CounterMode;   /**< Specifies the counter mode. This parameter can be a value of @ref TIM_Counter_Mode */
//  uint32_t Period;        /**< Specifies the period value to be loaded into the ARR. (0 to 65535) */
//} TIM_Base_InitTypeDef_t;
//
///**
// * @brief TIM register structure definition.
// */
//typedef struct
//{
//    volatile unsigned int CR1;
//    volatile unsigned int CR2;
//    volatile unsigned int SMCR;
//    volatile unsigned int DIER;
//    volatile unsigned int SR;
//    volatile unsigned int EGR;
//    volatile unsigned int CCMR1;
//    volatile unsigned int CCMR2;
//    volatile unsigned int CCER;
//    volatile unsigned int CNT;
//    volatile unsigned int PSC;
//    volatile unsigned int ARR;
//    volatile unsigned int RCR;
//    volatile unsigned int CCR1;
//    volatile unsigned int CCR2;
//    volatile unsigned int CCR3;
//    volatile unsigned int CCR4;
//    volatile unsigned int BDTR;
//    volatile unsigned int DCR;
//    volatile unsigned int DMAR;
//} TIM_typedef_t;
//
///**
// * @brief TIM Handle Structure definition (HAL Style).
// */
//typedef struct
//{
//  TIM_typedef_t            *Instance;   /**< Register base address */
//  TIM_Base_InitTypeDef_t   Init;       /**< TIM Time Base required parameters */
//} TIM_HandleTypeDef_t;
//
///**
// * @ref TIM_Counter_Mode
// */
//#define TIM_COUNTERMODE_UP              (0x00000000U)
//#define TIM_COUNTERMODE_DOWN            (0x00000010U)
//
///***********************************************************************************************************************
// * GLOBAL FUNCTION PROTOTYPES
// **********************************************************************************************************************/
//
///* Khởi tạo và giải phóng Timer */
//HAL_TIM_StatusTypeDef_t HAL_TIM_Base_Init(TIM_HandleTypeDef_t *htim);
//HAL_TIM_StatusTypeDef_t HAL_TIM_Base_DeInit(TIM_HandleTypeDef_t *htim);
//
///* Điều khiển chạy/dừng ở chế độ thường (Polling) */
//HAL_TIM_StatusTypeDef_t HAL_TIM_Base_Start(TIM_HandleTypeDef_t *htim);
//HAL_TIM_StatusTypeDef_t HAL_TIM_Base_Stop(TIM_HandleTypeDef_t *htim);
//
///* Điều khiển chạy/dừng ở chế độ ngắt (Interrupt) */
//HAL_TIM_StatusTypeDef_t HAL_TIM_Base_Start_IT(TIM_HandleTypeDef_t *htim);
//HAL_TIM_StatusTypeDef_t HAL_TIM_Base_Stop_IT(TIM_HandleTypeDef_t *htim);
//
///* Các hàm tạo hàm trễ (Delay) */
//void HAL_TIM_DelayUs(TIM_HandleTypeDef_t *htim, uint32_t Us);
//void HAL_TIM_DelayMs(TIM_HandleTypeDef_t *htim, uint32_t Ms);
//
///* Hàm xử lý ngắt tổng quát và hàm Callback xử lý sự kiện ngắt */
//void HAL_TIM_IRQHandler(TIM_HandleTypeDef_t *htim);
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef_t *htim);
//
//#ifdef __cplusplus
//}
//#endif
//
//
//#endif /* LIB_TIMER_HAL_TIMER_H_ */
