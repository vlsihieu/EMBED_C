################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Lib/Device/w25xx/hal_w25qxx.c 

OBJS += \
./Core/Lib/Device/w25xx/hal_w25qxx.o 

C_DEPS += \
./Core/Lib/Device/w25xx/hal_w25qxx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Lib/Device/w25xx/hal_w25qxx.o: ../Core/Lib/Device/w25xx/hal_w25qxx.c Core/Lib/Device/w25xx/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I"D:/07_Source/embedded_01/EMBED_C/EP_LLDriver/Core/Lib/Utils" -I"D:/07_Source/embedded_01/EMBED_C/EP_LLDriver/Core/Lib/Utils/common" -I"D:/07_Source/embedded_01/EMBED_C/EP_LLDriver/Core/Lib/Driver/rcc" -I"D:/07_Source/embedded_01/EMBED_C/EP_LLDriver/Core/Lib/Driver" -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I"D:/07_Source/embedded_01/EMBED_C/EP_LLDriver/Core/Lib/Driver/gpio" -I"D:/07_Source/embedded_01/EMBED_C/EP_LLDriver/Core/Lib/Driver/timer" -I"D:/07_Source/embedded_01/EMBED_C/EP_LLDriver/Core/Lib/Driver/dma" -I"D:/07_Source/embedded_01/EMBED_C/EP_LLDriver/Core/Lib/Driver/uart" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Lib/Device/w25xx/hal_w25qxx.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

