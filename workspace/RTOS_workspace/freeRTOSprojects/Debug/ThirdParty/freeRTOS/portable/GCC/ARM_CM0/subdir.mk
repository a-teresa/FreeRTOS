################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ThirdParty/freeRTOS/portable/GCC/ARM_CM0/port.c 

OBJS += \
./ThirdParty/freeRTOS/portable/GCC/ARM_CM0/port.o 

C_DEPS += \
./ThirdParty/freeRTOS/portable/GCC/ARM_CM0/port.d 


# Each subdirectory must supply rules for building sources it contributes
ThirdParty/freeRTOS/portable/GCC/ARM_CM0/%.o ThirdParty/freeRTOS/portable/GCC/ARM_CM0/%.su ThirdParty/freeRTOS/portable/GCC/ARM_CM0/%.cyclo: ../ThirdParty/freeRTOS/portable/GCC/ARM_CM0/%.c ThirdParty/freeRTOS/portable/GCC/ARM_CM0/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L053xx -c -I../Core/Inc -I"/Users/anateresaneto/Desktop/app/FreeRTOS/workspace/RTOS_workspace/freeRTOSprojects/ThirdParty/freeRTOS" -I"/Users/anateresaneto/Desktop/app/FreeRTOS/workspace/RTOS_workspace/freeRTOSprojects/ThirdParty/freeRTOS/portable/GCC/ARM_CM0" -I"/Users/anateresaneto/Desktop/app/FreeRTOS/workspace/RTOS_workspace/freeRTOSprojects/ThirdParty/freeRTOS/include" -I../Drivers/STM32L0xx_HAL_Driver/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-ThirdParty-2f-freeRTOS-2f-portable-2f-GCC-2f-ARM_CM0

clean-ThirdParty-2f-freeRTOS-2f-portable-2f-GCC-2f-ARM_CM0:
	-$(RM) ./ThirdParty/freeRTOS/portable/GCC/ARM_CM0/port.cyclo ./ThirdParty/freeRTOS/portable/GCC/ARM_CM0/port.d ./ThirdParty/freeRTOS/portable/GCC/ARM_CM0/port.o ./ThirdParty/freeRTOS/portable/GCC/ARM_CM0/port.su

.PHONY: clean-ThirdParty-2f-freeRTOS-2f-portable-2f-GCC-2f-ARM_CM0

