################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Lib/Driver/spi/hal_spi.c 

OBJS += \
./Lib/Driver/spi/hal_spi.o 

C_DEPS += \
./Lib/Driver/spi/hal_spi.d 


# Each subdirectory must supply rules for building sources it contributes
Lib/Driver/spi/%.o Lib/Driver/spi/%.su Lib/Driver/spi/%.cyclo: ../Lib/Driver/spi/%.c Lib/Driver/spi/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/quocv/STM32CubeIDE/workspace_1.19.0/TESTF1/Lib/Driver" -I"C:/Users/quocv/STM32CubeIDE/workspace_1.19.0/TESTF1/Lib/Utils" -I"C:/Users/quocv/STM32CubeIDE/workspace_1.19.0/TESTF1/Lib/Driver/gpio" -I"C:/Users/quocv/STM32CubeIDE/workspace_1.19.0/TESTF1/Lib/Driver/dma" -I"C:/Users/quocv/STM32CubeIDE/workspace_1.19.0/TESTF1/Lib/Driver/rcc" -I"C:/Users/quocv/STM32CubeIDE/workspace_1.19.0/TESTF1/Lib/Utils/common" -I"C:/Users/quocv/STM32CubeIDE/workspace_1.19.0/TESTF1/Lib/Driver/uart" -I"C:/Users/quocv/STM32CubeIDE/workspace_1.19.0/TESTF1/Lib" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Lib-2f-Driver-2f-spi

clean-Lib-2f-Driver-2f-spi:
	-$(RM) ./Lib/Driver/spi/hal_spi.cyclo ./Lib/Driver/spi/hal_spi.d ./Lib/Driver/spi/hal_spi.o ./Lib/Driver/spi/hal_spi.su

.PHONY: clean-Lib-2f-Driver-2f-spi

