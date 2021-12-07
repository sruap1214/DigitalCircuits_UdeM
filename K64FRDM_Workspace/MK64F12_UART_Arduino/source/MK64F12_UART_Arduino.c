#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"
#include "fsl_uart.h"

volatile uint8_t dataTx = 0x30;
volatile uint8_t dataRx = 0;
volatile uint8_t band = 0;

int main(void) {

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    PRINTF("Hello World\n\r");

    while(1) {
    	if(band){
    		band = 0;
    		UART_WriteByte(UART1, dataTx);
    		//UART_WriteBlocking(UART1, &dataTx, 1);
    	}
    	if(UART_GetStatusFlags(UART1) & UART_S1_RDRF_MASK){
    		dataRx = UART_ReadByte(UART1);
    		//UART_ReadBlocking(UART1, &dataRx, 1);
    		PRINTF("%c",dataRx);
    	}
    }
    return 0 ;
}

/* PIT0_IRQn interrupt handler */
void PIT_CHANNEL_0_IRQHANDLER(void) {
  uint32_t intStatus;
  /* Reading all interrupt flags of status register */
  intStatus = PIT_GetStatusFlags(PIT_PERIPHERAL, PIT_CHANNEL_0);
  PIT_ClearStatusFlags(PIT_PERIPHERAL, PIT_CHANNEL_0, intStatus);

  /* Place your code here */
  band = 1;
  dataTx++;

  /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
     Store immediate overlapping exception return operation might vector to incorrect interrupt. */
  #if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
  #endif
}

