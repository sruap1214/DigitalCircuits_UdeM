#include <stdio.h>
#include "clock_config.h"
#include "MK64F12.h"

void Init_SPI0(void);

uint32_t SPI_send(uint16_t data);

int main(void) {
	float temperature = 0;

    /* Init board hardware. */
    BOARD_InitBootClocks();
    Init_SPI0();

    while(1) {
    	temperature = ((float)(SPI_send(0x00)>>3))/4.0;
    }
    return 0 ;
}

void Init_SPI0(void){
	// Enable clock to SPI0 and PORTD
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
	SIM->SCGC6 |= SIM_SCGC6_SPI0_MASK;

	// Enable SPI and stops transfers SPI in debug mode
	SPI0->MCR &= ~SPI_MCR_MDIS_MASK;
	SPI0->MCR |= SPI_MCR_HALT_MASK;

	// Set PTD0 as SPI0_PCS0 -- ALT2
	PORTD->PCR[0] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[0] |= PORT_PCR_MUX(2);

	// Set PTD1 as SPI0_SCK -- ALT2
	PORTD->PCR[1] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[1] |= PORT_PCR_MUX(2);

	// Set PTD2 as SPI0_SOUT -- ALT2
	PORTD->PCR[2] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[2] |= PORT_PCR_MUX(2);

	// Set PTD3 as SPI0_SIN -- ALT2
	PORTD->PCR[3] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[3] |= PORT_PCR_MUX(2);

	// Enable master mode, inactive SS is high
	SPI0->MCR |= SPI_MCR_MSTR_MASK | SPI_MCR_PCSIS_MASK;

	// SCK baud rate = (Bus Clock /PBR) x [(1+DBR)/BR]
	// SCK = (60MHz/7) x [1/8192] = 1 kHz approx 1.046 kHz
	uint32_t temp;
	temp = SPI0->CTAR[0] & ~(SPI_CTAR_DBR_MASK | SPI_CTAR_PBR_MASK | SPI_CTAR_BR_MASK);
	SPI0->CTAR[0] = temp | SPI_CTAR_DBR(0)|
			SPI_CTAR_PBR(3)|
			SPI_CTAR_BR(12);

	// Temp variable with zeros in changed fields
	temp = SPI0->CTAR[0] &
	           ~(SPI_CTAR_FMSZ_MASK | SPI_CTAR_CPOL_MASK | SPI_CTAR_CPHA_MASK | SPI_CTAR_LSBFE_MASK);

	// Dataframe de 16 bits, inactive SCK is low, CPHA is one where data is changed on the leading edge, MSB first
	SPI0->CTAR[0] = temp | SPI_CTAR_FMSZ(16-1) |
			SPI_CTAR_CPOL(0) |
			SPI_CTAR_CPHA(1) |
			SPI_CTAR_LSBFE(0);

	// Enable SPI transfer
	SPI0->MCR &= ~SPI_MCR_HALT_MASK;

}

uint32_t SPI_send(uint16_t data){
	uint32_t dataRx;

	// Push data to Tx FIFO, and PCS = 0
	SPI0->PUSHR = (SPI_PUSHR_PCS(1) | data);
	// Wait for Tx buffer empty
	while(!(SPI0->SR & SPI_SR_TCF_MASK));
	SPI0->SR |= SPI_SR_TCF_MASK;


	// Wait for data in Rx buffer
	while (!(SPI0->SR & SPI_SR_RFDF_MASK));
	//Read data
	dataRx = SPI0->POPR;
	SPI0->SR |= SPI_SR_RFDF_MASK;
	return dataRx;

}
