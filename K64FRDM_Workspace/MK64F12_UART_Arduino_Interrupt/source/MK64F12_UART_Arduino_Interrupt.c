#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"

/* ----    User definitions    -------*/
#define Q_SIZE (32)

/* ----    User variables    -------*/
typedef struct {
	unsigned char Data[Q_SIZE];
	unsigned int Head; // points to oldest data element
	unsigned int Tail; // points to next free space
	unsigned int Size; // quantity of elements in queue
} Q_T;

Q_T tx_q, rx_q;

volatile uint8_t dataTx = 0x30;
volatile uint8_t dataRx = 0;
volatile uint8_t band = 0;
volatile uint8_t band2 = 0;

/* ----    User functions    -------*/
void Q_Init(Q_T *q);
int Q_Empty(Q_T *q);
int Q_Full(Q_T *q);
int Q_Enqueue(Q_T *q, unsigned char d);
unsigned char Q_Dequeue(Q_T *q);



int main(void) {

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    PRINTF("Hello World\n");

    Q_Init(&tx_q);
    Q_Init(&rx_q);


    while(1) {
    	if(band){
			band = 0;
			UART_WriteByte(UART1, dataTx);
			//UART_WriteBlocking(UART1, &dataTx, 1);
		}
    	if(band2 == 2){
    		band2 = 0;
    		while(!Q_Empty(&rx_q)){
				PRINTF("%c",Q_Dequeue(&rx_q));
			}
    	}


    }
    return 0 ;
}


void Q_Init(Q_T *q) {
	unsigned int i;
	for (i=0; i<Q_SIZE; i++)
		q->Data[i] = 0;  // to simplify our lives when debugging
	q->Head = 0;
	q->Tail = 0;
	q->Size = 0;
}
int Q_Empty(Q_T *q) {
	return q->Size == 0;
}
int Q_Full(Q_T *q) {
	return q->Size == Q_SIZE;
}
int Q_Enqueue(Q_T *q, unsigned char d) {
	// What if queue is full?
	if (!Q_Full(q)) {
		q->Data[q->Tail++] = d;
		q->Tail %= Q_SIZE;
		q->Size++;
		return 1; // success
	} else
	return 0; // failure
}
unsigned char Q_Dequeue(Q_T *q) {
	// Must check to see if queue is empty before dequeueing
	unsigned char t=0;
	if (!Q_Empty(q)) {
		t = q->Data[q->Head];
		q->Data[q->Head++] = 0; // to simplify debugging
		q->Head %= Q_SIZE;
		q->Size--;
	}
	return t;
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
  band2++;

  /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
     Store immediate overlapping exception return operation might vector to incorrect interrupt. */
  #if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
  #endif
}

/* UART1_RX_TX_IRQn interrupt handler */
void UART1_SERIAL_RX_TX_IRQHANDLER(void) {
  uint32_t intStatus;
  /* Reading all interrupt flags of status registers */
  intStatus = UART_GetStatusFlags(UART1_PERIPHERAL);

  /* Flags can be cleared by reading the status register and reading/writing data registers.
    See the reference manual for details of each flag.
    The UART_ClearStatusFlags() function can be also used for clearing of flags in case the content of data regsiter is not used.
    For example:
        status_t status;
        intStatus &= ~(kUART_RxOverrunFlag | kUART_NoiseErrorFlag | kUART_FramingErrorFlag | kUART_ParityErrorFlag);
        status = UART_ClearStatusFlags(UART1_PERIPHERAL, intStatus);
  */

  /* Place your code here */
	if (intStatus & UART_S1_RDRF_MASK) {
		// received a character
		if (!Q_Full(&rx_q)) {
			Q_Enqueue(&rx_q, UART_ReadByte(UART1));
		} else {
			// error - queue full.
			while (1);
		}
	}

  UART_ClearStatusFlags(UART1_PERIPHERAL, intStatus);
  /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
     Store immediate overlapping exception return operation might vector to incorrect interrupt. */
  #if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
  #endif
}


