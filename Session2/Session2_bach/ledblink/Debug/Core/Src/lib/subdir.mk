################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/lib/cnt.c \
../Core/Src/lib/rgt.c 

OBJS += \
./Core/Src/lib/cnt.o \
./Core/Src/lib/rgt.o 

C_DEPS += \
./Core/Src/lib/cnt.d \
./Core/Src/lib/rgt.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/lib/%.o Core/Src/lib/%.su Core/Src/lib/%.cyclo: ../Core/Src/lib/%.c Core/Src/lib/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/PC/STM32CubeIDE/workspace_1.19.0/ledblink/Core/Src/lib" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-lib

clean-Core-2f-Src-2f-lib:
	-$(RM) ./Core/Src/lib/cnt.cyclo ./Core/Src/lib/cnt.d ./Core/Src/lib/cnt.o ./Core/Src/lib/cnt.su ./Core/Src/lib/rgt.cyclo ./Core/Src/lib/rgt.d ./Core/Src/lib/rgt.o ./Core/Src/lib/rgt.su

.PHONY: clean-Core-2f-Src-2f-lib

