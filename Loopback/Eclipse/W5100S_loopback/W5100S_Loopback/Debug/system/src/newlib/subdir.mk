################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../system/src/newlib/_exit.c \
../system/src/newlib/_sbrk.c \
../system/src/newlib/_startup.c \
../system/src/newlib/_syscalls.c \
../system/src/newlib/assert.c 

CPP_SRCS += \
../system/src/newlib/_cxx.cpp 

OBJS += \
./system/src/newlib/_cxx.o \
./system/src/newlib/_exit.o \
./system/src/newlib/_sbrk.o \
./system/src/newlib/_startup.o \
./system/src/newlib/_syscalls.o \
./system/src/newlib/assert.o 

C_DEPS += \
./system/src/newlib/_exit.d \
./system/src/newlib/_sbrk.d \
./system/src/newlib/_startup.d \
./system/src/newlib/_syscalls.d \
./system/src/newlib/assert.d 

CPP_DEPS += \
./system/src/newlib/_cxx.d 


# Each subdirectory must supply rules for building sources it contributes
system/src/newlib/%.o: ../system/src/newlib/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/STMicroelectronics" -std=gnu++11 -fabi-version=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

system/src/newlib/%.o: ../system/src/newlib/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DSTM32F10X_HD -DDEBUG -DUSE_STDPERIPH_DRIVER -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/STMicroelectronics" -I"D:\Wiznet\WIZnet_product\WIZ750SR_series\WIZ750SR\Rev1.1\04.Firmware_Source\Eclipse\workspace\W5100S_loopback\W5100S_Loopback\src" -I"D:\Wiznet\WIZnet_product\WIZ750SR_series\WIZ750SR\Rev1.1\04.Firmware_Source\Eclipse\workspace\W5100S_loopback\W5100S_Loopback\src\stm_lib" -I"D:\Wiznet\WIZnet_product\WIZ750SR_series\WIZ750SR\Rev1.1\04.Firmware_Source\Eclipse\workspace\W5100S_loopback\W5100S_Loopback\src\syscalls" -I"D:\Wiznet\WIZnet_product\WIZ750SR_series\WIZ750SR\Rev1.1\04.Firmware_Source\Eclipse\workspace\W5100S_loopback\W5100S_Loopback\src\cmsis" -I"D:\Wiznet\WIZnet_product\WIZ750SR_series\WIZ750SR\Rev1.1\04.Firmware_Source\Eclipse\workspace\W5100S_loopback\W5100S_Loopback\src\cmsis_boot" -I"D:\Wiznet\WIZnet_product\WIZ750SR_series\WIZ750SR\Rev1.1\04.Firmware_Source\Eclipse\workspace\W5100S_loopback\W5100S_Loopback\src\stm_lib\inc" -I"D:\Wiznet\WIZnet_product\WIZ750SR_series\WIZ750SR\Rev1.1\04.Firmware_Source\Eclipse\workspace\W5100S_loopback\W5100S_Loopback\src\ioLibrary_Driver" -I"D:\Wiznet\WIZnet_product\WIZ750SR_series\WIZ750SR\Rev1.1\04.Firmware_Source\Eclipse\workspace\W5100S_loopback\W5100S_Loopback\src\ioLibrary_Driver\Ethernet" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


