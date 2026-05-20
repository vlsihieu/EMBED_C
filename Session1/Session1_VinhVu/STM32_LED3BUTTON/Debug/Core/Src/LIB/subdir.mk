################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/LIB/GPIOx.c 

OBJS += \
./Core/Src/LIB/GPIOx.o 

C_DEPS += \
./Core/Src/LIB/GPIOx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/LIB/%.o Core/Src/LIB/%.su Core/Src/LIB/%.cyclo: ../Core/Src/LIB/%.c Core/Src/LIB/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Admin/STM32CubeIDE/workspace_2.1.0/STM32_LED3BUTTON/Core/Src/LIB" -I"C:/Users/Admin/STM32CubeIDE/workspace_2.1.0/STM32_LED3BUTTON" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-LIB

clean-Core-2f-Src-2f-LIB:
	-$(RM) ./Core/Src/LIB/GPIOx.cyclo ./Core/Src/LIB/GPIOx.d ./Core/Src/LIB/GPIOx.o ./Core/Src/LIB/GPIOx.su

.PHONY: clean-Core-2f-Src-2f-LIB

