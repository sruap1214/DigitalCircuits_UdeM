/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

#ifndef _PERIPHERALS_H_
#define _PERIPHERALS_H_

/***********************************************************************************************************************
 * Included files
 **********************************************************************************************************************/
#include "fsl_common.h"
#include "fsl_pit.h"
#include "fsl_uart.h"
#include "fsl_clock.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/
/* Definitions for BOARD_InitPeripherals functional group */
/* BOARD_InitPeripherals defines for PIT */
/* Definition of peripheral ID. */
#define PIT_PERIPHERAL PIT
/* Definition of clock source. */
#define PIT_CLOCK_SOURCE kCLOCK_BusClk
/* Definition of clock source frequency. */
#define PIT_CLK_FREQ 60000000UL
/* Definition of ticks count for channel 0 - deprecated. */
#define PIT_0_TICKS 59999999U
/* PIT interrupt vector ID (number) - deprecated. */
#define PIT_0_IRQN PIT0_IRQn
/* PIT interrupt handler identifier - deprecated. */
#define PIT_0_IRQHANDLER PIT0_IRQHandler
/* Definition of channel number for channel 0. */
#define PIT_CHANNEL_0 kPIT_Chnl_0
/* Definition of ticks count for channel 0. */
#define PIT_CHANNEL_0_TICKS 59999999U
/* PIT interrupt vector ID (number). */
#define PIT_CHANNEL_0_IRQN PIT0_IRQn
/* PIT interrupt handler identifier. */
#define PIT_CHANNEL_0_IRQHANDLER PIT0_IRQHandler
/* Definition of peripheral ID */
#define UART1_PERIPHERAL UART1
/* Definition of the clock source frequency */
#define UART1_CLOCK_SOURCE 120000000UL
/* UART1 interrupt vector ID (number). */
#define UART1_SERIAL_RX_TX_IRQN UART1_RX_TX_IRQn
/* UART1 interrupt handler identifier. */
#define UART1_SERIAL_RX_TX_IRQHANDLER UART1_RX_TX_IRQHandler
/* UART1 interrupt vector ID (number). */
#define UART1_SERIAL_ERROR_IRQN UART1_ERR_IRQn
/* UART1 interrupt handler identifier. */
#define UART1_SERIAL_ERROR_IRQHANDLER UART1_ERR_IRQHandler

/***********************************************************************************************************************
 * Global variables
 **********************************************************************************************************************/
extern const pit_config_t PIT_config;
extern const uart_config_t UART1_config;

/***********************************************************************************************************************
 * Initialization functions
 **********************************************************************************************************************/

void BOARD_InitPeripherals(void);

/***********************************************************************************************************************
 * BOARD_InitBootPeripherals function
 **********************************************************************************************************************/
void BOARD_InitBootPeripherals(void);

#if defined(__cplusplus)
}
#endif

#endif /* _PERIPHERALS_H_ */
