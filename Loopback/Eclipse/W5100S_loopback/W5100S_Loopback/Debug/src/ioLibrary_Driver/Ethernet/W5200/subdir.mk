################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/ioLibrary_Driver/Ethernet/W5200/w5200.c 

OBJS += \
./src/ioLibrary_Driver/Ethernet/W5200/w5200.o 

C_DEPS += \
./src/ioLibrary_Driver/Ethernet/W5200/w5200.d 


# Each subdirectory must supply rules for building sources it contributes
src/ioLibrary_Driver/Ethernet/W5200/%.o: ../src/ioLibrary_Driver/Ethernet/W5200/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DSTM32F10X_HD -DSTM32F103VC -DDEBUG -DUSE_STDPERIPH_DRIVER -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/STMicroelectronics" -I"D:\Wiznet\WIZnet_product\WIZ750SR_series\WIZ750SR\Rev1.1\04.Firmware_Source\Eclipse\workspace\W5100S_loopback\W5100S_Loopback\src" -I"D:\Wiznet\WIZnet_product\WIZ750SR_series\WIZ750SR\Rev1.1\04.Firmware_Source\Eclipse\workspace\W5100S_loopback\W5100S_Loopback\src\stm_lib" -I"D:\Wiznet\WIZnet_product\WIZ750SR_series\WIZ750SR\Rev1.1\04.Firmware_Source\Eclipse\workspace\W5100S_loopback\W5100S_Loopback\src\syscalls" -I"D:\Wiznet\WIZnet_product\WIZ750SR_series\WIZ750SR\Rev1.1\04.Firmware_Source\Eclipse\workspace\W5100S_loopback\W5100S_Loopback\src\cmsis" -I"D:\Wiznet\WIZnet_product\WIZ750SR_series\WIZ750SR\Rev1.1\04.Firmware_Source\Eclipse\workspace\W5100S_loopback\W5100S_Loopback\src\cmsis_boot" -I"D:\Wiznet\WIZnet_product\WIZ750SR_series\WIZ750SR\Rev1.1\04.Firmware_Source\Eclipse\workspace\W5100S_loopback\W5100S_Loopback\src\stm_lib\inc" -I"D:\Wiznet\WIZnet_product\WIZ750SR_series\WIZ750SR\Rev1.1\04.Firmware_Source\Eclipse\workspace\W5100S_loopback\W5100S_Loopback\src\ioLibrary_Driver" -I"D:\Wiznet\WIZnet_product\WIZ750SR_series\WIZ750SR\Rev1.1\04.Firmware_Source\Eclipse\workspace\W5100S_loopback\W5100S_Loopback\src\ioLibrary_Driver\Ethernet" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


