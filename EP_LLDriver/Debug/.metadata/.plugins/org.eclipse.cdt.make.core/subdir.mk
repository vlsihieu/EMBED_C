################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../.metadata/.plugins/org.eclipse.cdt.make.core/specs.c 

OBJS += \
./.metadata/.plugins/org.eclipse.cdt.make.core/specs.o 

C_DEPS += \
./.metadata/.plugins/org.eclipse.cdt.make.core/specs.d 


# Each subdirectory must supply rules for building sources it contributes
.metadata/.plugins/org.eclipse.cdt.make.core/specs.o: ../.metadata/.plugins/org.eclipse.cdt.make.core/specs.c .metadata/.plugins/org.eclipse.cdt.make.core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g -c -I"D:/07_Source/embedded_01/EMBED_C/EP_LLDriver/Core/Lib/Utils" -I"D:/07_Source/embedded_01/EMBED_C/EP_LLDriver/Core/Lib/Driver" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF".metadata/.plugins/org.eclipse.cdt.make.core/specs.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

