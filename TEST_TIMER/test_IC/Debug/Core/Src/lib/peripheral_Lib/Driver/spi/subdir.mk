################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/lib/peripheral_Lib/Driver/spi/hal_spi.c 

OBJS += \
./Core/Src/lib/peripheral_Lib/Driver/spi/hal_spi.o 

C_DEPS += \
./Core/Src/lib/peripheral_Lib/Driver/spi/hal_spi.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/lib/peripheral_Lib/Driver/spi/%.o Core/Src/lib/peripheral_Lib/Driver/spi/%.su Core/Src/lib/peripheral_Lib/Driver/spi/%.cyclo: ../Core/Src/lib/peripheral_Lib/Driver/spi/%.c Core/Src/lib/peripheral_Lib/Driver/spi/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/PC/STM32CubeIDE/workspace_1.19.0/test_IC/Core/Src/lib/personal_lib" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-lib-2f-peripheral_Lib-2f-Driver-2f-spi

clean-Core-2f-Src-2f-lib-2f-peripheral_Lib-2f-Driver-2f-spi:
	-$(RM) ./Core/Src/lib/peripheral_Lib/Driver/spi/hal_spi.cyclo ./Core/Src/lib/peripheral_Lib/Driver/spi/hal_spi.d ./Core/Src/lib/peripheral_Lib/Driver/spi/hal_spi.o ./Core/Src/lib/peripheral_Lib/Driver/spi/hal_spi.su

.PHONY: clean-Core-2f-Src-2f-lib-2f-peripheral_Lib-2f-Driver-2f-spi

