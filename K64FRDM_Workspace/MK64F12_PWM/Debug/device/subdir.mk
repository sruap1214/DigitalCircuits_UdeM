################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../device/system_MK64F12.c 

OBJS += \
./device/system_MK64F12.o 

C_DEPS += \
./device/system_MK64F12.d 


# Each subdirectory must supply rules for building sources it contributes
device/%.o: ../device/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MK64FN1M0VLL12 -DCPU_MK64FN1M0VLL12_cm4 -DSDK_OS_BAREMETAL -DSERIAL_PORT_TYPE_UART=1 -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\SANTIAGO RUA PEREZ\OneDrive\Documents\Docencia\UdeM\2021\2021-02\Sistemas Embebidos\03_Software\K64FRDM_Workspace\MK64F12_PWM\board" -I"C:\Users\SANTIAGO RUA PEREZ\OneDrive\Documents\Docencia\UdeM\2021\2021-02\Sistemas Embebidos\03_Software\K64FRDM_Workspace\MK64F12_PWM\source" -I"C:\Users\SANTIAGO RUA PEREZ\OneDrive\Documents\Docencia\UdeM\2021\2021-02\Sistemas Embebidos\03_Software\K64FRDM_Workspace\MK64F12_PWM\drivers" -I"C:\Users\SANTIAGO RUA PEREZ\OneDrive\Documents\Docencia\UdeM\2021\2021-02\Sistemas Embebidos\03_Software\K64FRDM_Workspace\MK64F12_PWM\utilities" -I"C:\Users\SANTIAGO RUA PEREZ\OneDrive\Documents\Docencia\UdeM\2021\2021-02\Sistemas Embebidos\03_Software\K64FRDM_Workspace\MK64F12_PWM\component\serial_manager" -I"C:\Users\SANTIAGO RUA PEREZ\OneDrive\Documents\Docencia\UdeM\2021\2021-02\Sistemas Embebidos\03_Software\K64FRDM_Workspace\MK64F12_PWM\component\uart" -I"C:\Users\SANTIAGO RUA PEREZ\OneDrive\Documents\Docencia\UdeM\2021\2021-02\Sistemas Embebidos\03_Software\K64FRDM_Workspace\MK64F12_PWM\component\lists" -I"C:\Users\SANTIAGO RUA PEREZ\OneDrive\Documents\Docencia\UdeM\2021\2021-02\Sistemas Embebidos\03_Software\K64FRDM_Workspace\MK64F12_PWM\CMSIS" -I"C:\Users\SANTIAGO RUA PEREZ\OneDrive\Documents\Docencia\UdeM\2021\2021-02\Sistemas Embebidos\03_Software\K64FRDM_Workspace\MK64F12_PWM\device" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


