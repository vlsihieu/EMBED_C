################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Lib/Capture/capture.c 

OBJS += \
./Core/Src/Lib/Capture/capture.o 

C_DEPS += \
./Core/Src/Lib/Capture/capture.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Lib/Capture/%.o Core/Src/Lib/Capture/%.su Core/Src/Lib/Capture/%.cyclo: ../Core/Src/Lib/Capture/%.c Core/Src/Lib/Capture/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I"E:/Embedded-IoT/Workspace/timer_exercise_24_7/Core/Src/Lib/Capture" -I"E:/Embedded-IoT/Workspace/timer_exercise_24_7/Core/Src/Lib/Delay" -I"E:/Embedded-IoT/Workspace/timer_exercise_24_7/Core/Src/Lib/PWM" -I"E:/Embedded-IoT/Workspace/timer_exercise_24_7/Core/Src/Lib" -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Lib-2f-Capture

clean-Core-2f-Src-2f-Lib-2f-Capture:
	-$(RM) ./Core/Src/Lib/Capture/capture.cyclo ./Core/Src/Lib/Capture/capture.d ./Core/Src/Lib/Capture/capture.o ./Core/Src/Lib/Capture/capture.su

.PHONY: clean-Core-2f-Src-2f-Lib-2f-Capture

