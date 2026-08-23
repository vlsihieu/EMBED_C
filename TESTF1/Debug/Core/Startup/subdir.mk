################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Core/Startup/startup_stm32f103c8tx.s 

OBJS += \
./Core/Startup/startup_stm32f103c8tx.o 

S_DEPS += \
./Core/Startup/startup_stm32f103c8tx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Startup/%.o: ../Core/Startup/%.s Core/Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m3 -g -DDEBUG -c -I"C:/Users/quocv/STM32CubeIDE/workspace_1.19.0/TESTF1/Lib/Driver" -I"C:/Users/quocv/STM32CubeIDE/workspace_1.19.0/TESTF1/Lib/Utils" -I"C:/Users/quocv/STM32CubeIDE/workspace_1.19.0/TESTF1/Lib/Driver/gpio" -I"C:/Users/quocv/STM32CubeIDE/workspace_1.19.0/TESTF1/Lib/Utils/common" -I"C:/Users/quocv/STM32CubeIDE/workspace_1.19.0/TESTF1/Lib/Driver/uart" -I"C:/Users/quocv/STM32CubeIDE/workspace_1.19.0/TESTF1/Lib/Driver/rcc" -I"C:/Users/quocv/STM32CubeIDE/workspace_1.19.0/TESTF1/Lib/Driver/dma" -I"C:/Users/quocv/STM32CubeIDE/workspace_1.19.0/TESTF1/Lib/Driver/spi" -I"C:/Users/quocv/STM32CubeIDE/workspace_1.19.0/TESTF1/Lib/Driver/timer" -I"C:/Users/quocv/STM32CubeIDE/workspace_1.19.0/TESTF1/Lib" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@" "$<"

clean: clean-Core-2f-Startup

clean-Core-2f-Startup:
	-$(RM) ./Core/Startup/startup_stm32f103c8tx.d ./Core/Startup/startup_stm32f103c8tx.o

.PHONY: clean-Core-2f-Startup

