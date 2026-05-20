################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/LIB/GPIO_x.c 

OBJS += \
./Core/Src/LIB/GPIO_x.o 

C_DEPS += \
./Core/Src/LIB/GPIO_x.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/LIB/%.o Core/Src/LIB/%.su Core/Src/LIB/%.cyclo: ../Core/Src/LIB/%.c Core/Src/LIB/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/quocv/STM32CubeIDE/workspace_1.19.0/LED_Light/Core/Src/LIB" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-LIB

clean-Core-2f-Src-2f-LIB:
	-$(RM) ./Core/Src/LIB/GPIO_x.cyclo ./Core/Src/LIB/GPIO_x.d ./Core/Src/LIB/GPIO_x.o ./Core/Src/LIB/GPIO_x.su

.PHONY: clean-Core-2f-Src-2f-LIB

