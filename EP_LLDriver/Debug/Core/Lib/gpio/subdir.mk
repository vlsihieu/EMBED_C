################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Lib/gpio/hal_gpio.c 

OBJS += \
./Core/Lib/gpio/hal_gpio.o 

C_DEPS += \
./Core/Lib/gpio/hal_gpio.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Lib/gpio/hal_gpio.o: ../Core/Lib/gpio/hal_gpio.c Core/Lib/gpio/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/hieuk/STM32CubeIDE/workspace_1.6.0/EP_LLDriver/Core/Lib/rcc" -I"C:/Users/hieuk/STM32CubeIDE/workspace_1.6.0/EP_LLDriver/Core/Lib/common" -I"C:/Users/hieuk/STM32CubeIDE/workspace_1.6.0/EP_LLDriver/Core/Lib/gpio" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Lib/gpio/hal_gpio.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

