################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../component/serial_manager/fsl_component_serial_manager.c \
../component/serial_manager/fsl_component_serial_port_uart.c 

OBJS += \
./component/serial_manager/fsl_component_serial_manager.o \
./component/serial_manager/fsl_component_serial_port_uart.o 

C_DEPS += \
./component/serial_manager/fsl_component_serial_manager.d \
./component/serial_manager/fsl_component_serial_port_uart.d 


# Each subdirectory must supply rules for building sources it contributes
component/serial_manager/%.o: ../component/serial_manager/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MK64FN1M0VLL12 -DCPU_MK64FN1M0VLL12_cm4 -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"D:\OneDrive\Documents\Docencia\UdeM\2021\2021-02\Sistemas Embebidos\03_Software\K64FRDM_Workspace\MK64F12_InterruptExample2\board" -I"D:\OneDrive\Documents\Docencia\UdeM\2021\2021-02\Sistemas Embebidos\03_Software\K64FRDM_Workspace\MK64F12_InterruptExample2\source" -I"D:\OneDrive\Documents\Docencia\UdeM\2021\2021-02\Sistemas Embebidos\03_Software\K64FRDM_Workspace\MK64F12_InterruptExample2\drivers" -I"D:\OneDrive\Documents\Docencia\UdeM\2021\2021-02\Sistemas Embebidos\03_Software\K64FRDM_Workspace\MK64F12_InterruptExample2\utilities" -I"D:\OneDrive\Documents\Docencia\UdeM\2021\2021-02\Sistemas Embebidos\03_Software\K64FRDM_Workspace\MK64F12_InterruptExample2\component\serial_manager" -I"D:\OneDrive\Documents\Docencia\UdeM\2021\2021-02\Sistemas Embebidos\03_Software\K64FRDM_Workspace\MK64F12_InterruptExample2\component\uart" -I"D:\OneDrive\Documents\Docencia\UdeM\2021\2021-02\Sistemas Embebidos\03_Software\K64FRDM_Workspace\MK64F12_InterruptExample2\component\lists" -I"D:\OneDrive\Documents\Docencia\UdeM\2021\2021-02\Sistemas Embebidos\03_Software\K64FRDM_Workspace\MK64F12_InterruptExample2\CMSIS" -I"D:\OneDrive\Documents\Docencia\UdeM\2021\2021-02\Sistemas Embebidos\03_Software\K64FRDM_Workspace\MK64F12_InterruptExample2\device" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


