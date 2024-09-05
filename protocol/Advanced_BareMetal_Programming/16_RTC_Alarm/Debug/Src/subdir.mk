################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/adc_dma.c \
../Src/dma.c \
../Src/i2c_dma.c \
../Src/main.c \
../Src/mpu9250.c \
../Src/pwm_dma.c \
../Src/rtc.c \
../Src/spi_dma.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/tim.c \
../Src/uart.c 

OBJS += \
./Src/adc_dma.o \
./Src/dma.o \
./Src/i2c_dma.o \
./Src/main.o \
./Src/mpu9250.o \
./Src/pwm_dma.o \
./Src/rtc.o \
./Src/spi_dma.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/tim.o \
./Src/uart.o 

C_DEPS += \
./Src/adc_dma.d \
./Src/dma.d \
./Src/i2c_dma.d \
./Src/main.d \
./Src/mpu9250.d \
./Src/pwm_dma.d \
./Src/rtc.d \
./Src/spi_dma.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/tim.d \
./Src/uart.d 


# Each subdirectory must supply rules for building sources it contributes
Src/adc_dma.o: ../Src/adc_dma.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F411RETx -DSTM32F411xE -c -I../Inc -I"C:/Users/Gaming Laptop RTX/Documents/Advanced_BareMetal_Programming/16_RTC_Alarm/chip_headers/CMSIS/Include" -I"C:/Users/Gaming Laptop RTX/Documents/Advanced_BareMetal_Programming/16_RTC_Alarm/chip_headers/CMSIS/Device/ST/STM32F4xx/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/adc_dma.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/dma.o: ../Src/dma.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F411RETx -DSTM32F411xE -c -I../Inc -I"C:/Users/Gaming Laptop RTX/Documents/Advanced_BareMetal_Programming/16_RTC_Alarm/chip_headers/CMSIS/Include" -I"C:/Users/Gaming Laptop RTX/Documents/Advanced_BareMetal_Programming/16_RTC_Alarm/chip_headers/CMSIS/Device/ST/STM32F4xx/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/dma.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/i2c_dma.o: ../Src/i2c_dma.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F411RETx -DSTM32F411xE -c -I../Inc -I"C:/Users/Gaming Laptop RTX/Documents/Advanced_BareMetal_Programming/16_RTC_Alarm/chip_headers/CMSIS/Include" -I"C:/Users/Gaming Laptop RTX/Documents/Advanced_BareMetal_Programming/16_RTC_Alarm/chip_headers/CMSIS/Device/ST/STM32F4xx/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/i2c_dma.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/main.o: ../Src/main.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F411RETx -DSTM32F411xE -c -I../Inc -I"C:/Users/Gaming Laptop RTX/Documents/Advanced_BareMetal_Programming/16_RTC_Alarm/chip_headers/CMSIS/Include" -I"C:/Users/Gaming Laptop RTX/Documents/Advanced_BareMetal_Programming/16_RTC_Alarm/chip_headers/CMSIS/Device/ST/STM32F4xx/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/main.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/mpu9250.o: ../Src/mpu9250.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F411RETx -DSTM32F411xE -c -I../Inc -I"C:/Users/Gaming Laptop RTX/Documents/Advanced_BareMetal_Programming/16_RTC_Alarm/chip_headers/CMSIS/Include" -I"C:/Users/Gaming Laptop RTX/Documents/Advanced_BareMetal_Programming/16_RTC_Alarm/chip_headers/CMSIS/Device/ST/STM32F4xx/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/mpu9250.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/pwm_dma.o: ../Src/pwm_dma.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F411RETx -DSTM32F411xE -c -I../Inc -I"C:/Users/Gaming Laptop RTX/Documents/Advanced_BareMetal_Programming/16_RTC_Alarm/chip_headers/CMSIS/Include" -I"C:/Users/Gaming Laptop RTX/Documents/Advanced_BareMetal_Programming/16_RTC_Alarm/chip_headers/CMSIS/Device/ST/STM32F4xx/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/pwm_dma.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/rtc.o: ../Src/rtc.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F411RETx -DSTM32F411xE -c -I../Inc -I"C:/Users/Gaming Laptop RTX/Documents/Advanced_BareMetal_Programming/16_RTC_Alarm/chip_headers/CMSIS/Include" -I"C:/Users/Gaming Laptop RTX/Documents/Advanced_BareMetal_Programming/16_RTC_Alarm/chip_headers/CMSIS/Device/ST/STM32F4xx/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/rtc.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/spi_dma.o: ../Src/spi_dma.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F411RETx -DSTM32F411xE -c -I../Inc -I"C:/Users/Gaming Laptop RTX/Documents/Advanced_BareMetal_Programming/16_RTC_Alarm/chip_headers/CMSIS/Include" -I"C:/Users/Gaming Laptop RTX/Documents/Advanced_BareMetal_Programming/16_RTC_Alarm/chip_headers/CMSIS/Device/ST/STM32F4xx/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/spi_dma.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/syscalls.o: ../Src/syscalls.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F411RETx -DSTM32F411xE -c -I../Inc -I"C:/Users/Gaming Laptop RTX/Documents/Advanced_BareMetal_Programming/16_RTC_Alarm/chip_headers/CMSIS/Include" -I"C:/Users/Gaming Laptop RTX/Documents/Advanced_BareMetal_Programming/16_RTC_Alarm/chip_headers/CMSIS/Device/ST/STM32F4xx/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/syscalls.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/sysmem.o: ../Src/sysmem.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F411RETx -DSTM32F411xE -c -I../Inc -I"C:/Users/Gaming Laptop RTX/Documents/Advanced_BareMetal_Programming/16_RTC_Alarm/chip_headers/CMSIS/Include" -I"C:/Users/Gaming Laptop RTX/Documents/Advanced_BareMetal_Programming/16_RTC_Alarm/chip_headers/CMSIS/Device/ST/STM32F4xx/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/sysmem.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/tim.o: ../Src/tim.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F411RETx -DSTM32F411xE -c -I../Inc -I"C:/Users/Gaming Laptop RTX/Documents/Advanced_BareMetal_Programming/16_RTC_Alarm/chip_headers/CMSIS/Include" -I"C:/Users/Gaming Laptop RTX/Documents/Advanced_BareMetal_Programming/16_RTC_Alarm/chip_headers/CMSIS/Device/ST/STM32F4xx/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/tim.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/uart.o: ../Src/uart.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F411RETx -DSTM32F411xE -c -I../Inc -I"C:/Users/Gaming Laptop RTX/Documents/Advanced_BareMetal_Programming/16_RTC_Alarm/chip_headers/CMSIS/Include" -I"C:/Users/Gaming Laptop RTX/Documents/Advanced_BareMetal_Programming/16_RTC_Alarm/chip_headers/CMSIS/Device/ST/STM32F4xx/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/uart.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

