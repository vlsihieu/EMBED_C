///*
//* timer.c
//*
//*  Created on: Jun 9, 2026
//*      Author: hieuk
//*/
//
//#ifndef LIB_TIMER_TIMER_C_
//#define LIB_TIMER_TIMER_C_
//
//
//#include "hal_timer.h"
//
//
///***********************************************************************************************************************
//* LOCAL PERIPHERAL REGISTER MAPS
//**********************************************************************************************************************/
//
//#define RCC_BASE_ADDR           (0x40021000U)
//#define RCC_APB1ENR_REG         (*(volatile uint32_t *)(RCC_BASE_ADDR + 0x1CU))
//#define RCC_APB2ENR_REG         (*(volatile uint32_t *)(RCC_BASE_ADDR + 0x18U))
//
///***********************************************************************************************************************
//* LOCAL VARIABLES
//**********************************************************************************************************************/
//
///* Mảng quản lý Handle để phục vụ định tuyến từ Vector ngắt phần cứng sang đúng Object Handle của ứng dụng */
//static TIM_HandleTypeDef_t * gp_tim_handles[8] = { NULL };
//
///***********************************************************************************************************************
//* LOCAL FUNCTION PROTOTYPES
//**********************************************************************************************************************/
//
//static void prv_TIM_EnableClock(TIM_typedef_t * TIMx);
//static inline IRQn_Type prv_TIM_GetIrqNumber(TIM_typedef_t * TIMx);
//static inline uint8_t prv_TIM_GetHandleId(TIM_typedef_t * TIMx);
//
///***********************************************************************************************************************
//* GLOBAL FUNCTIONS (HAL LAYER)
//**********************************************************************************************************************/
//
///**
//* @brief  Khởi tạo cấu hình cơ bản cho Timer (Mức thanh ghi).
//*/
//HAL_TIM_StatusTypeDef_t HAL_TIM_Base_Init(TIM_HandleTypeDef_t *htim)
//{
//   if (htim == NULL)
//   {
//       return HAL_TIM_ERROR;
//   }
//
//   /* Kiểm tra tính hợp lệ của thanh ghi ngoại vi */
//   if (!ASSET_TIM_ALL_INSTANCE(htim->Instance))
//   {
//       return HAL_TIM_ERROR;
//   }
//
//   /* Kích hoạt Clock cho ngoại vi trước khi ghi dữ liệu vào thanh ghi */
//   prv_TIM_EnableClock(htim->Instance);
//
//   /* Cấu hình giá trị Prescaler và Auto-Reload (Period) */
//   htim->Instance->PSC = htim->Init.Prescaler;
//   htim->Instance->ARR = htim->Init.Period;
//
//   /* Cấu hình hướng đếm (Đếm lên hoặc đếm xuống) cho TIM1, 2, 3, 4, 5 */
//   if ((htim->Instance == TIM1x) || (htim->Instance == TIM2x) ||
//       (htim->Instance == TIM3x) || (htim->Instance == TIM4x) || (htim->Instance == TIM5x))
//   {
//       uint32_t tmpcr1 = htim->Instance->CR1;
//       tmpcr1 &= ~(0x00000010U); /* Xoá bit DIR cũ */
//       tmpcr1 |= htim->Init.CounterMode;
//       htim->Instance->CR1 = tmpcr1;
//   }
//
//   /* Tạo một sự kiện Update để cập nhật bóng (Shadow Register) lập tức cho PSC và ARR */
//   htim->Instance->EGR |= (1U << 0U);
//
//   /* Đăng ký con trỏ Handle vào hệ thống quản lý ngắt */
//   uint8_t id = prv_TIM_GetHandleId(htim->Instance);
//   if (id != 0U)
//   {
//       gp_tim_handles[id] = htim;
//   }
//
//   return HAL_TIM_OK;
//}
//
///**
//* @brief  Huỷ khởi tạo và tắt ngoại vi Timer.
//*/
//HAL_TIM_StatusTypeDef_t HAL_TIM_Base_DeInit(TIM_HandleTypeDef_t *htim)
//{
//   if ((htim == NULL) || (!ASSET_TIM_ALL_INSTANCE(htim->Instance)))
//   {
//       return HAL_TIM_ERROR;
//   }
//
//   /* Dừng đếm và xoá các cờ cấu hình ngắt */
//   htim->Instance->CR1  &= ~(1U << 0U);
//   htim->Instance->DIER &= ~(1U << 0U);
//
//   /* Tắt ngắt trên nhân NVIC */
//   IRQn_Type irq = prv_TIM_GetIrqNumber(htim->Instance);
//   NVIC_DisableIRQ(irq);
//
//   /* Giải phóng vị trí trong bảng Handle */
//   uint8_t id = prv_TIM_GetHandleId(htim->Instance);
//   if (id != 0U)
//   {
//       gp_tim_handles[id] = NULL;
//   }
//
//   return HAL_TIM_OK;
//}
//
///**
//* @brief  Khởi động Timer ở chế độ cơ bản (Không dùng ngắt).
//*/
//HAL_TIM_StatusTypeDef_t HAL_TIM_Base_Start(TIM_HandleTypeDef_t *htim)
//{
//   if ((htim == NULL) || (!ASSET_TIM_ALL_INSTANCE(htim->Instance))) return HAL_TIM_ERROR;
//
//   htim->Instance->CR1 |= (1U << 0U); /* Set bit CEN (Counter Enable) */
//   return HAL_TIM_OK;
//}
//
///**
//* @brief  Dừng hoạt động Timer.
//*/
//HAL_TIM_StatusTypeDef_t HAL_TIM_Base_Stop(TIM_HandleTypeDef_t *htim)
//{
//   if ((htim == NULL) || (!ASSET_TIM_ALL_INSTANCE(htim->Instance))) return HAL_TIM_ERROR;
//
//   htim->Instance->CR1 &= ~(1U << 0U); /* Clear bit CEN */
//   return HAL_TIM_OK;
//}
//
///**
//* @brief  Khởi động Timer kết hợp kích hoạt ngắt phần cứng qua NVIC.
//*/
//HAL_TIM_StatusTypeDef_t HAL_TIM_Base_Start_IT(TIM_HandleTypeDef_t *htim)
//{
//   if ((htim == NULL) || (!ASSET_TIM_ALL_INSTANCE(htim->Instance))) return HAL_TIM_ERROR;
//
//   /* Xoá cờ ngắt cũ nếu có */
//   htim->Instance->SR &= ~(1U << 0U);
//
//   /* Bật bit UIE (Update Interrupt Enable) trên thanh ghi DIER */
//   htim->Instance->DIER |= (1U << 0U);
//
//   /* Kích hoạt đường ngắt NVIC */
//   IRQn_Type irq = prv_TIM_GetIrqNumber(htim->Instance);
//   NVIC_ClearPendingIRQ(irq);
//   NVIC_EnableIRQ(irq);
//
//   /* Bật đếm bộ Counter */
//   htim->Instance->CR1 |= (1U << 0U);
//
//   return HAL_TIM_OK;
//}
//
///**
//* @brief  Dừng Timer và ngắt phần cứng.
//*/
//HAL_TIM_StatusTypeDef_t HAL_TIM_Base_Stop_IT(TIM_HandleTypeDef_t *htim)
//{
//   if ((htim == NULL) || (!ASSET_TIM_ALL_INSTANCE(htim->Instance))) return HAL_TIM_ERROR;
//
//   /* Tắt ngắt phần cứng nội tại */
//   htim->Instance->DIER &= ~(1U << 0U);
//
//   /* Tắt đếm */
//   htim->Instance->CR1 &= ~(1U << 0U);
//
//   /* Tắt đường truyền NVIC */
//   IRQn_Type irq = prv_TIM_GetIrqNumber(htim->Instance);
//   NVIC_DisableIRQ(irq);
//
//   return HAL_TIM_OK;
//}
//
///**
//* @brief  Hàm tạo trễ Microseconds bằng phương pháp Polling (đếm vòng).
//*/
//void HAL_TIM_DelayUs(TIM_HandleTypeDef_t *htim, uint32_t Us)
//{
//   if ((htim == NULL) || (!ASSET_TIM_ALL_INSTANCE(htim->Instance))) return;
//
//   htim->Instance->CNT = 0U;
//   while (htim->Instance->CNT < Us);
//}
//
///**
//* @brief  Hàm tạo trễ Milliseconds.
//*/
//void HAL_TIM_DelayMs(TIM_HandleTypeDef_t *htim, uint32_t Ms)
//{
//   for (uint32_t i = 0; i < Ms; i++)
//   {
//       HAL_TIM_DelayUs(htim, 1000U);
//   }
//}
//
///**
//* @brief  Bộ xử lý ngắt trung gian tổng quát (Sẽ gọi trong ISR).
//*/
//void HAL_TIM_IRQHandler(TIM_HandleTypeDef_t *htim)
//{
//   /* Kiểm tra cờ Update Interrupt Flag */
//   if ((htim->Instance->SR & (1U << 0U)) != 0x00U)
//   {
//       /* Kiểm tra xem ngắt này có thực sự được kích hoạt trước đó hay không */
//       if ((htim->Instance->DIER & (1U << 0U)) != 0x00U)
//       {
//           /* Xoá cờ ngắt Update */
//           htim->Instance->SR &= ~(1U << 0U);
//
//           /* Gọi hàm Callback xử lý nghiệp vụ tầng ứng dụng */
//           HAL_TIM_PeriodElapsedCallback(htim);
//       }
//   }
//}
//
///**
//* @brief  Hàm Callback mặc định (Có từ khoá gán yếu __weak để tầng ứng dụng Override lại).
//*/
//__weak void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef_t *htim)
//{
//   /* Tránh cảnh báo unused parameter từ trình biên dịch */
//   (void)htim;
//}
//
///***********************************************************************************************************************
//* LOCAL FUNCTION IMPLEMENTATIONS
//**********************************************************************************************************************/
//
//static void prv_TIM_EnableClock(TIM_typedef_t * TIMx)
//{
//   if (TIMx == TIM1x)       RCC_APB2ENR_REG |= (1U << 11U);
//   else if (TIMx == TIM2x)  RCC_APB1ENR_REG |= (1U << 0U);
//   else if (TIMx == TIM3x)  RCC_APB1ENR_REG |= (1U << 1U);
//   else if (TIMx == TIM4x)  RCC_APB1ENR_REG |= (1U << 2U);
//   else if (TIMx == TIM5x)  RCC_APB1ENR_REG |= (1U << 3U);
//   else if (TIMx == TIM6x)  RCC_APB1ENR_REG |= (1U << 4U);
//   else if (TIMx == TIM7x)  RCC_APB1ENR_REG |= (1U << 5U);
//}
//
//static inline IRQn_Type prv_TIM_GetIrqNumber(TIM_typedef_t * TIMx)
//{
//   if (TIMx == TIM1x)      return (IRQn_Type)25; /* TIM1_UP_IRQn */
//   else if (TIMx == TIM2x) return (IRQn_Type)28; /* TIM2_IRQn */
//   else if (TIMx == TIM3x) return (IRQn_Type)29; /* TIM3_IRQn */
//   else if (TIMx == TIM4x) return (IRQn_Type)30; /* TIM4_IRQn */
//   else if (TIMx == TIM5x) return (IRQn_Type)50; /* TIM5_IRQn */
//   else if (TIMx == TIM6x) return (IRQn_Type)54; /* TIM6_IRQn */
//   else                    return (IRQn_Type)55; /* TIM7_IRQn */
//}
//
//static inline uint8_t prv_TIM_GetHandleId(TIM_typedef_t * TIMx)
//{
//   if (TIMx == TIM1x)      return 1U;
//   else if (TIMx == TIM2x) return 2U;
//   else if (TIMx == TIM3x) return 3U;
//   else if (TIMx == TIM4x) return 4U;
//   else if (TIMx == TIM5x) return 5U;
//   else if (TIMx == TIM6x) return 6U;
//   else if (TIMx == TIM7x) return 7U;
//   return 0U;
//}
//
///***********************************************************************************************************************
//* INTERRUPT SERVICE ROUTINES (ISRs)
//**********************************************************************************************************************/
//
//void TIM1_UP_IRQHandler(void)  { if(gp_tim_handles[1] != NULL) HAL_TIM_IRQHandler(gp_tim_handles[1]); }
//void TIM2_IRQHandler(void)     { if(gp_tim_handles[2] != NULL) HAL_TIM_IRQHandler(gp_tim_handles[2]); }
//void TIM3_IRQHandler(void)     { if(gp_tim_handles[3] != NULL) HAL_TIM_IRQHandler(gp_tim_handles[3]); }
//void TIM4_IRQHandler(void)     { if(gp_tim_handles[4] != NULL) HAL_TIM_IRQHandler(gp_tim_handles[4]); }
//void TIM5_IRQHandler(void)     { if(gp_tim_handles[5] != NULL) HAL_TIM_IRQHandler(gp_tim_handles[5]); }
//void TIM6_IRQHandler(void)     { if(gp_tim_handles[6] != NULL) HAL_TIM_IRQHandler(gp_tim_handles[6]); }
//void TIM7_IRQHandler(void)     { if(gp_tim_handles[7] != NULL) HAL_TIM_IRQHandler(gp_tim_handles[7]); }
//
//#endif /* LIB_TIMER_TIMER_C_ */
