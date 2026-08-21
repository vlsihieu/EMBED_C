################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Core/Startup/startup_stm32f103c8tx.s 

OBJS += \
./Core/Startup/startup_stm32f103c8tx.o 

S_DEPS += \
./Core/Startup/startup_stm32f103c8tx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Startup/startup_stm32f103c8tx.o: ../Core/Startup/startup_stm32f103c8tx.s Core/Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m3 -g -c -I"D:/07_Source/embedded_01/EMBED_C/EP_LLDriver/Core/Lib/Driver/gpio" -I"D:/07_Source/embedded_01/EMBED_C/EP_LLDriver/Core/Lib/Utils" -I"D:/07_Source/embedded_01/EMBED_C/EP_LLDriver/Core/Lib/Driver/gpio" -I"D:/07_Source/embedded_01/EMBED_C/EP_LLDriver/Core/Lib/Driver/timer" -I"D:/07_Source/embedded_01/EMBED_C/EP_LLDriver/Core/Lib/Driver/dma" -I"D:/07_Source/embedded_01/EMBED_C/EP_LLDriver/Core/Lib/Driver/uart" -x assembler-with-cpp -MMD -MP -MF"Core/Startup/startup_stm32f103c8tx.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@" "$<"

