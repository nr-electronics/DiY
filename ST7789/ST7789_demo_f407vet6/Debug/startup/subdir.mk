################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../startup/startup_stm32f407xx.s 

OBJS += \
./startup/startup_stm32f407xx.o 

S_DEPS += \
./startup/startup_stm32f407xx.d 


# Each subdirectory must supply rules for building sources it contributes
startup/startup_stm32f407xx.o: ../startup/startup_stm32f407xx.s
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -c -include"C:/Users/nr.old/Desktop/ST7789-STM32-master/ST7789_demo_f407vet6/ST7789/st7789.h" -x assembler-with-cpp -MMD -MP -MF"startup/startup_stm32f407xx.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

