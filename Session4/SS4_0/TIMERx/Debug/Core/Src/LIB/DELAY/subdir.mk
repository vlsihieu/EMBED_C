################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/LIB/DELAY/TIMER.c 

OBJS += \
./Core/Src/LIB/DELAY/TIMER.o 

C_DEPS += \
./Core/Src/LIB/DELAY/TIMER.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/LIB/DELAY/%.o Core/Src/LIB/DELAY/%.su Core/Src/LIB/DELAY/%.cyclo: ../Core/Src/LIB/DELAY/%.c Core/Src/LIB/DELAY/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/quocv/STM32CubeIDE/workspace_1.19.0/TIMERx/Core/Src/LIB" -I"C:/Users/quocv/STM32CubeIDE/workspace_1.19.0/TIMERx/Core/Src/LIB/PWM" -I"C:/Users/quocv/STM32CubeIDE/workspace_1.19.0/TIMERx/Core/Src/LIB/Capture" -I"C:/Users/quocv/STM32CubeIDE/workspace_1.19.0/TIMERx/Core/Src/LIB/DELAY" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-LIB-2f-DELAY

clean-Core-2f-Src-2f-LIB-2f-DELAY:
	-$(RM) ./Core/Src/LIB/DELAY/TIMER.cyclo ./Core/Src/LIB/DELAY/TIMER.d ./Core/Src/LIB/DELAY/TIMER.o ./Core/Src/LIB/DELAY/TIMER.su

.PHONY: clean-Core-2f-Src-2f-LIB-2f-DELAY

