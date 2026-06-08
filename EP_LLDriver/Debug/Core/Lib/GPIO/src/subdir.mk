################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Lib/GPIO/src/GPIOx_Ip.c 

OBJS += \
./Core/Lib/GPIO/src/GPIOx_Ip.o 

C_DEPS += \
./Core/Lib/GPIO/src/GPIOx_Ip.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Lib/GPIO/src/GPIOx_Ip.o: ../Core/Lib/GPIO/src/GPIOx_Ip.c Core/Lib/GPIO/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I"C:/Users/hieuk/STM32CubeIDE/workspace_1.6.0/EP_LLDriver/Core/Lib/RCC/inc" -I"C:/Users/hieuk/STM32CubeIDE/workspace_1.6.0/EP_LLDriver/Core/Lib/Common/inc" -I"C:/Users/hieuk/STM32CubeIDE/workspace_1.6.0/EP_LLDriver/Core/Lib/GPIO/inc" -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/hieuk/STM32CubeIDE/workspace_1.6.0/EP_LLDriver/Core/Lib/GPIO" -I"C:/Users/hieuk/STM32CubeIDE/workspace_1.6.0/EP_LLDriver/Core/Lib/Common" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Lib/GPIO/src/GPIOx_Ip.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

