################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/MK64F12_SPI_Termocouple.c \
../source/semihost_hardfault.c 

OBJS += \
./source/MK64F12_SPI_Termocouple.o \
./source/semihost_hardfault.o 

C_DEPS += \
./source/MK64F12_SPI_Termocouple.d \
./source/semihost_hardfault.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MK64FN1M0VLL12 -DCPU_MK64FN1M0VLL12_cm4 -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\SANTIAGO RUA PEREZ\OneDrive\Documents\Docencia\UdeM\2021\2021-02\Sistemas Embebidos\03_Software\K64FRDM_Workspace\MK64F12_SPI_Termocouple\board" -I"C:\Users\SANTIAGO RUA PEREZ\OneDrive\Documents\Docencia\UdeM\2021\2021-02\Sistemas Embebidos\03_Software\K64FRDM_Workspace\MK64F12_SPI_Termocouple\source" -I"C:\Users\SANTIAGO RUA PEREZ\OneDrive\Documents\Docencia\UdeM\2021\2021-02\Sistemas Embebidos\03_Software\K64FRDM_Workspace\MK64F12_SPI_Termocouple\drivers" -I"C:\Users\SANTIAGO RUA PEREZ\OneDrive\Documents\Docencia\UdeM\2021\2021-02\Sistemas Embebidos\03_Software\K64FRDM_Workspace\MK64F12_SPI_Termocouple\utilities" -I"C:\Users\SANTIAGO RUA PEREZ\OneDrive\Documents\Docencia\UdeM\2021\2021-02\Sistemas Embebidos\03_Software\K64FRDM_Workspace\MK64F12_SPI_Termocouple\component\serial_manager" -I"C:\Users\SANTIAGO RUA PEREZ\OneDrive\Documents\Docencia\UdeM\2021\2021-02\Sistemas Embebidos\03_Software\K64FRDM_Workspace\MK64F12_SPI_Termocouple\component\uart" -I"C:\Users\SANTIAGO RUA PEREZ\OneDrive\Documents\Docencia\UdeM\2021\2021-02\Sistemas Embebidos\03_Software\K64FRDM_Workspace\MK64F12_SPI_Termocouple\component\lists" -I"C:\Users\SANTIAGO RUA PEREZ\OneDrive\Documents\Docencia\UdeM\2021\2021-02\Sistemas Embebidos\03_Software\K64FRDM_Workspace\MK64F12_SPI_Termocouple\CMSIS" -I"C:\Users\SANTIAGO RUA PEREZ\OneDrive\Documents\Docencia\UdeM\2021\2021-02\Sistemas Embebidos\03_Software\K64FRDM_Workspace\MK64F12_SPI_Termocouple\device" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


