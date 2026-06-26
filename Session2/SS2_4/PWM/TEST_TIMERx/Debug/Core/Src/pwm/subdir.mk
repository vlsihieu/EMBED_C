################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/pwm/pwm.c 

OBJS += \
./Core/Src/pwm/pwm.o 

C_DEPS += \
./Core/Src/pwm/pwm.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/pwm/%.o Core/Src/pwm/%.su Core/Src/pwm/%.cyclo: ../Core/Src/pwm/%.c Core/Src/pwm/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"D:/STM32CubeIDE/workspace_2.1.0/TEST_TIMERx/Core/Src/lib" -I"D:/STM32CubeIDE/workspace_2.1.0/TEST_TIMERx/Core/Src/pwm" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-pwm

clean-Core-2f-Src-2f-pwm:
	-$(RM) ./Core/Src/pwm/pwm.cyclo ./Core/Src/pwm/pwm.d ./Core/Src/pwm/pwm.o ./Core/Src/pwm/pwm.su

.PHONY: clean-Core-2f-Src-2f-pwm

