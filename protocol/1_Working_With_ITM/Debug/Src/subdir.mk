################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/main.c \
../Src/systeminit.c 

OBJS += \
./Src/main.o \
./Src/systeminit.o 

C_DEPS += \
./Src/main.d \
./Src/systeminit.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32F410RBTx -DSTM32 -DNUCLEO_F410RB -DSTM32F4 -DSTM32F410Rx -c -I../Inc -I"/Users/anateresaneto/Desktop/app/FreeRTOS/protocol/1_Working_With_ITM/Inc/CMSIS/Device/ST/STM32F4xx/Include" -I"/Users/anateresaneto/Desktop/app/FreeRTOS/protocol/1_Working_With_ITM/Inc/CMSIS/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/systeminit.cyclo ./Src/systeminit.d ./Src/systeminit.o ./Src/systeminit.su

.PHONY: clean-Src
