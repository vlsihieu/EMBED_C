################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/lib/personal_lib/private.c 

OBJS += \
./Core/Src/lib/personal_lib/private.o 

C_DEPS += \
./Core/Src/lib/personal_lib/private.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/lib/personal_lib/%.o Core/Src/lib/personal_lib/%.su Core/Src/lib/personal_lib/%.cyclo: ../Core/Src/lib/personal_lib/%.c Core/Src/lib/personal_lib/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/PC/STM32CubeIDE/workspace_1.19.0/test_IC/Core/Src/lib/personal_lib" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-lib-2f-personal_lib

clean-Core-2f-Src-2f-lib-2f-personal_lib:
	-$(RM) ./Core/Src/lib/personal_lib/private.cyclo ./Core/Src/lib/personal_lib/private.d ./Core/Src/lib/personal_lib/private.o ./Core/Src/lib/personal_lib/private.su

.PHONY: clean-Core-2f-Src-2f-lib-2f-personal_lib

