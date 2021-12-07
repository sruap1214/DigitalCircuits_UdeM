#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"
#include "fsl_dspi.h"

int main(void) {

	float temperature;

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    // Command configuration for SPI
    dspi_command_data_config_t SPI0Config;
    SPI0Config.isPcsContinuous=false;
    SPI0Config.whichCtar = kDSPI_Ctar0;
    SPI0Config.whichPcs = kDSPI_Pcs0;
    SPI0Config.clearTransferCount = false;
    SPI0Config.isEndOfQueue = false;

    while(1) {
    	DSPI_MasterWriteDataBlocking(SPI0, &SPI0Config, 0x00);
    	uint32_t DataSPI = DSPI_ReadData(SPI0);
    	temperature = ((float)(DataSPI>>3))/4.0;
    	PRINTF("La temperatura es: %f\n\r",temperature);
    }
    return 0 ;
}
