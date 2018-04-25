################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/stm_lib/src/misc.c \
../src/stm_lib/src/stm32f10x_exti.c \
../src/stm_lib/src/stm32f10x_flash.c \
../src/stm_lib/src/stm32f10x_fsmc.c \
../src/stm_lib/src/stm32f10x_gpio.c \
../src/stm_lib/src/stm32f10x_rcc.c \
../src/stm_lib/src/stm32f10x_spi.c \
../src/stm_lib/src/stm32f10x_tim.c \
../src/stm_lib/src/stm32f10x_usart.c 

OBJS += \
./src/stm_lib/src/misc.o \
./src/stm_lib/src/stm32f10x_exti.o \
./src/stm_lib/src/stm32f10x_flash.o \
./src/stm_lib/src/stm32f10x_fsmc.o \
./src/stm_lib/src/stm32f10x_gpio.o \
./src/stm_lib/src/stm32f10x_rcc.o \
./src/stm_lib/src/stm32f10x_spi.o \
./src/stm_lib/src/stm32f10x_tim.o \
./src/stm_lib/src/stm32f10x_usart.o 

C_DEPS += \
./src/stm_lib/src/misc.d \
./src/stm_lib/src/stm32f10x_exti.d \
./src/stm_lib/src/stm32f10x_flash.d \
./src/stm_lib/src/stm32f10x_fsmc.d \
./src/stm_lib/src/stm32f10x_gpio.d \
./src/stm_lib/src/stm32f10x_rcc.d \
./src/stm_lib/src/stm32f10x_spi.d \
./src/stm_lib/src/stm32f10x_tim.d \
./src/stm_lib/src/stm32f10x_usart.d 


# Each subdirectory must supply rules for building sources it contributes
src/stm_lib/src/%.o: ../src/stm_lib/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DSTM32F10X_HD -DSTM32F103VC -DDEBUG -DUSE_STDPERIPH_DRIVER -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/STMicroelectronics" -I"D:\Wiznet\WIZnet_product\WIZ750SR_series\WIZ750SR\Rev1.1\04.Firmware_Source\Eclipse\workspace\W5100S_loopback\W5100S_Loopback\src" -I"D:\Wiznet\WIZnet_product\WIZ750SR_series\WIZ750SR\Rev1.1\04.Firmware_Source\Eclipse\workspace\W5100S_loopback\W5100S_Loopback\src\stm_lib" -I"D:\Wiznet\WIZnet_product\WIZ750SR_series\WIZ750SR\Rev1.1\04.Firmware_Source\Eclipse\workspace\W5100S_loopback\W5100S_Loopback\src\syscalls" -I"D:\Wiznet\WIZnet_product\WIZ750SR_series\WIZ750SR\Rev1.1\04.Firmware_Source\Eclipse\workspace\W5100S_loopback\W5100S_Loopback\src\cmsis" -I"D:\Wiznet\WIZnet_product\WIZ750SR_series\WIZ750SR\Rev1.1\04.Firmware_Source\Eclipse\workspace\W5100S_loopback\W5100S_Loopback\src\cmsis_boot" -I"D:\Wiznet\WIZnet_product\WIZ750SR_series\WIZ750SR\Rev1.1\04.Firmware_Source\Eclipse\workspace\W5100S_loopback\W5100S_Loopback\src\stm_lib\inc" -I"D:\Wiznet\WIZnet_product\WIZ750SR_series\WIZ750SR\Rev1.1\04.Firmware_Source\Eclipse\workspace\W5100S_loopback\W5100S_Loopback\src\ioLibrary_Driver" -I"D:\Wiznet\WIZnet_product\WIZ750SR_series\WIZ750SR\Rev1.1\04.Firmware_Source\Eclipse\workspace\W5100S_loopback\W5100S_Loopback\src\ioLibrary_Driver\Ethernet" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


