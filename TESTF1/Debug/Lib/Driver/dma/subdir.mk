################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Lib/Driver/dma/hal_dma.c 

OBJS += \
./Lib/Driver/dma/hal_dma.o 

C_DEPS += \
./Lib/Driver/dma/hal_dma.d 


# Each subdirectory must supply rules for building sources it contributes
Lib/Driver/dma/%.o Lib/Driver/dma/%.su Lib/Driver/dma/%.cyclo: ../Lib/Driver/dma/%.c Lib/Driver/dma/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/quocv/STM32CubeIDE/workspace_1.19.0/TESTF1/Lib/Driver" -I"C:/Users/quocv/STM32CubeIDE/workspace_1.19.0/TESTF1/Lib/Utils" -I"C:/Users/quocv/STM32CubeIDE/workspace_1.19.0/TESTF1/Lib/Driver/gpio" -I"C:/Users/quocv/STM32CubeIDE/workspace_1.19.0/TESTF1/Lib/Driver/dma" -I"C:/Users/quocv/STM32CubeIDE/workspace_1.19.0/TESTF1/Lib/Driver/rcc" -I"C:/Users/quocv/STM32CubeIDE/workspace_1.19.0/TESTF1/Lib/Utils/common" -I"C:/Users/quocv/STM32CubeIDE/workspace_1.19.0/TESTF1/Lib/Driver/uart" -I"C:/Users/quocv/STM32CubeIDE/workspace_1.19.0/TESTF1/Lib" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Lib-2f-Driver-2f-dma

clean-Lib-2f-Driver-2f-dma:
	-$(RM) ./Lib/Driver/dma/hal_dma.cyclo ./Lib/Driver/dma/hal_dma.d ./Lib/Driver/dma/hal_dma.o ./Lib/Driver/dma/hal_dma.su

.PHONY: clean-Lib-2f-Driver-2f-dma

