################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/lib/TIMERx.c 

OBJS += \
./Core/Src/lib/TIMERx.o 

C_DEPS += \
./Core/Src/lib/TIMERx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/lib/%.o Core/Src/lib/%.su Core/Src/lib/%.cyclo: ../Core/Src/lib/%.c Core/Src/lib/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"D:/STM32CubeIDE/workspace_2.1.0/TEST_TIMERx/Core/Src/lib" -I"D:/STM32CubeIDE/workspace_2.1.0/TEST_TIMERx/Core/Src/pwm" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-lib

clean-Core-2f-Src-2f-lib:
	-$(RM) ./Core/Src/lib/TIMERx.cyclo ./Core/Src/lib/TIMERx.d ./Core/Src/lib/TIMERx.o ./Core/Src/lib/TIMERx.su

.PHONY: clean-Core-2f-Src-2f-lib

