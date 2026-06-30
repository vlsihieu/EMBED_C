################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/LIB/Capture/capture.c 

OBJS += \
./Core/Src/LIB/Capture/capture.o 

C_DEPS += \
./Core/Src/LIB/Capture/capture.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/LIB/Capture/%.o Core/Src/LIB/Capture/%.su Core/Src/LIB/Capture/%.cyclo: ../Core/Src/LIB/Capture/%.c Core/Src/LIB/Capture/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/quocv/STM32CubeIDE/workspace_1.19.0/TIMERx/Core/Src/LIB" -I"C:/Users/quocv/STM32CubeIDE/workspace_1.19.0/TIMERx/Core/Src/LIB/PWM" -I"C:/Users/quocv/STM32CubeIDE/workspace_1.19.0/TIMERx/Core/Src/LIB/Capture" -I"C:/Users/quocv/STM32CubeIDE/workspace_1.19.0/TIMERx/Core/Src/LIB/DELAY" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-LIB-2f-Capture

clean-Core-2f-Src-2f-LIB-2f-Capture:
	-$(RM) ./Core/Src/LIB/Capture/capture.cyclo ./Core/Src/LIB/Capture/capture.d ./Core/Src/LIB/Capture/capture.o ./Core/Src/LIB/Capture/capture.su

.PHONY: clean-Core-2f-Src-2f-LIB-2f-Capture

