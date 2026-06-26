################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Library/PWM/PWM.c 

OBJS += \
./Core/Src/Library/PWM/PWM.o 

C_DEPS += \
./Core/Src/Library/PWM/PWM.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Library/PWM/%.o Core/Src/Library/PWM/%.su Core/Src/Library/PWM/%.cyclo: ../Core/Src/Library/PWM/%.c Core/Src/Library/PWM/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I"E:/Embedded-IoT/Workspace/Timer_exercise/Core/Src/Library" -I"E:/Embedded-IoT/Workspace/Timer_exercise/Core/Src/Library/PWM" -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Library-2f-PWM

clean-Core-2f-Src-2f-Library-2f-PWM:
	-$(RM) ./Core/Src/Library/PWM/PWM.cyclo ./Core/Src/Library/PWM/PWM.d ./Core/Src/Library/PWM/PWM.o ./Core/Src/Library/PWM/PWM.su

.PHONY: clean-Core-2f-Src-2f-Library-2f-PWM

