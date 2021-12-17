################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/ad7124.c \
../Core/Src/ad7124_console_app.c \
../Core/Src/ad7124_regs.c \
../Core/Src/ad7124_regs_config_a.c \
../Core/Src/buttons.c \
../Core/Src/delay.c \
../Core/Src/main.c \
../Core/Src/platform_support.c \
../Core/Src/rtcProcess.c \
../Core/Src/sdCard.c \
../Core/Src/spi.c \
../Core/Src/stm32g0xx_hal_msp.c \
../Core/Src/stm32g0xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32g0xx.c 

OBJS += \
./Core/Src/ad7124.o \
./Core/Src/ad7124_console_app.o \
./Core/Src/ad7124_regs.o \
./Core/Src/ad7124_regs_config_a.o \
./Core/Src/buttons.o \
./Core/Src/delay.o \
./Core/Src/main.o \
./Core/Src/platform_support.o \
./Core/Src/rtcProcess.o \
./Core/Src/sdCard.o \
./Core/Src/spi.o \
./Core/Src/stm32g0xx_hal_msp.o \
./Core/Src/stm32g0xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32g0xx.o 

C_DEPS += \
./Core/Src/ad7124.d \
./Core/Src/ad7124_console_app.d \
./Core/Src/ad7124_regs.d \
./Core/Src/ad7124_regs_config_a.d \
./Core/Src/buttons.d \
./Core/Src/delay.d \
./Core/Src/main.d \
./Core/Src/platform_support.d \
./Core/Src/rtcProcess.d \
./Core/Src/sdCard.d \
./Core/Src/spi.d \
./Core/Src/stm32g0xx_hal_msp.d \
./Core/Src/stm32g0xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32g0xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -DSTM32G071xx -DUSE_HAL_DRIVER -c -I../Drivers/CMSIS/Include -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/ad7124.d ./Core/Src/ad7124.o ./Core/Src/ad7124_console_app.d ./Core/Src/ad7124_console_app.o ./Core/Src/ad7124_regs.d ./Core/Src/ad7124_regs.o ./Core/Src/ad7124_regs_config_a.d ./Core/Src/ad7124_regs_config_a.o ./Core/Src/buttons.d ./Core/Src/buttons.o ./Core/Src/delay.d ./Core/Src/delay.o ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/platform_support.d ./Core/Src/platform_support.o ./Core/Src/rtcProcess.d ./Core/Src/rtcProcess.o ./Core/Src/sdCard.d ./Core/Src/sdCard.o ./Core/Src/spi.d ./Core/Src/spi.o ./Core/Src/stm32g0xx_hal_msp.d ./Core/Src/stm32g0xx_hal_msp.o ./Core/Src/stm32g0xx_it.d ./Core/Src/stm32g0xx_it.o ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/system_stm32g0xx.d ./Core/Src/system_stm32g0xx.o

.PHONY: clean-Core-2f-Src

