################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/LIB/PWM/PWM.c 

OBJS += \
./Core/Src/LIB/PWM/PWM.o 

C_DEPS += \
./Core/Src/LIB/PWM/PWM.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/LIB/PWM/%.o Core/Src/LIB/PWM/%.su Core/Src/LIB/PWM/%.cyclo: ../Core/Src/LIB/PWM/%.c Core/Src/LIB/PWM/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/quocv/STM32CubeIDE/workspace_1.19.0/TIMERx/Core/Src/LIB" -I"C:/Users/quocv/STM32CubeIDE/workspace_1.19.0/TIMERx/Core/Src/LIB/PWM" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-LIB-2f-PWM

clean-Core-2f-Src-2f-LIB-2f-PWM:
	-$(RM) ./Core/Src/LIB/PWM/PWM.cyclo ./Core/Src/LIB/PWM/PWM.d ./Core/Src/LIB/PWM/PWM.o ./Core/Src/LIB/PWM/PWM.su

.PHONY: clean-Core-2f-Src-2f-LIB-2f-PWM

