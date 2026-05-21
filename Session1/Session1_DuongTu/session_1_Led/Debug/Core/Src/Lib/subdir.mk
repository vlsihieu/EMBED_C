################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Lib/GPIOx.c 

OBJS += \
./Core/Src/Lib/GPIOx.o 

C_DEPS += \
./Core/Src/Lib/GPIOx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Lib/%.o Core/Src/Lib/%.su Core/Src/Lib/%.cyclo: ../Core/Src/Lib/%.c Core/Src/Lib/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I"E:/Embedded-IoT/Workspace/session_1_Led/Core/Inc" -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"E:/Embedded-IoT/Workspace/session_1_Led/Core/Src/Lib" -I../Core/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Lib

clean-Core-2f-Src-2f-Lib:
	-$(RM) ./Core/Src/Lib/GPIOx.cyclo ./Core/Src/Lib/GPIOx.d ./Core/Src/Lib/GPIOx.o ./Core/Src/Lib/GPIOx.su

.PHONY: clean-Core-2f-Src-2f-Lib

