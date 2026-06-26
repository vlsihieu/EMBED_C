################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Library/timer.c 

OBJS += \
./Core/Src/Library/timer.o 

C_DEPS += \
./Core/Src/Library/timer.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Library/%.o Core/Src/Library/%.su Core/Src/Library/%.cyclo: ../Core/Src/Library/%.c Core/Src/Library/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I"E:/Embedded-IoT/Workspace/Timer_exercise/Core/Src/Library" -I"E:/Embedded-IoT/Workspace/Timer_exercise/Core/Src/Library/PWM" -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Library

clean-Core-2f-Src-2f-Library:
	-$(RM) ./Core/Src/Library/timer.cyclo ./Core/Src/Library/timer.d ./Core/Src/Library/timer.o ./Core/Src/Library/timer.su

.PHONY: clean-Core-2f-Src-2f-Library

