################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/7seg.c \
../src/helper_functions.c \
../src/keypad.c \
../src/led_button.c \
../src/main.c \
../src/timer.c 

OBJS += \
./src/7seg.o \
./src/helper_functions.o \
./src/keypad.o \
./src/led_button.o \
./src/main.o \
./src/timer.o 

C_DEPS += \
./src/7seg.d \
./src/helper_functions.d \
./src/keypad.d \
./src/led_button.d \
./src/main.d \
./src/timer.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32 -DSTM32L4 -DSTM32L476RGTx -DNUCLEO_L476RG -DDEBUG -I"C:/Users/USER/workspace/interrupt/inc" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


