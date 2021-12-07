#include "MK64F12.h"

#define SW1	4
#define SW2 6
#define LED_BLUE 21
#define LED_RED 22
#define MASK(x) (1UL << (x))

volatile uint8_t  band = 0;

void init_switch(void);
void init_rgb(void);


int main(void) {
	uint32_t i = 0;
	__disable_irq();
	init_rgb();
	init_switch();
	__enable_irq();
	__get_PRIMASK();

    while(1) {
        i++;
        /* 'Dummy' NOP to allow source level single stepping of
            tight while() loop */
        __asm volatile ("nop");
        if(band==1){
        	PTB->PTOR |= MASK(LED_BLUE);
        	band = 0;
        }
    }
    return 0 ;
}

void init_rgb(void){
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK; //| SIM_SCGC5_PORTC_MASK;

	PORTB->PCR[LED_BLUE] = PORT_PCR_MUX(1);
	PORTB->PCR[LED_RED] = PORT_PCR_MUX(1);

	PTB->PDDR |= MASK(LED_BLUE)|MASK(LED_RED);

	PTB->PCOR |= MASK(LED_BLUE)|MASK(LED_RED);


}
void init_switch(void){
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK; //| SIM_SCGC5_PORTC_MASK;

	PORTA->PCR[SW1] = PORT_PCR_MUX(1) | PORT_PCR_IRQC(0x0a);
	//PORTA->PCR[SW2] = PORT_PCR_MUX(1) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_IRQC(0x0a);

	PTA->PDDR &= ~MASK(SW1);

	/* Enable Interrupts */
	NVIC_SetPriority(PORTA_IRQn, 2);
	NVIC_ClearPendingIRQ(PORTA_IRQn);
	NVIC_EnableIRQ(PORTA_IRQn);

}

void PORTA_IRQHandler(void) {
	if ((PORTA->ISFR & MASK(SW1))) {
		band=1;
	}
	// clear status flags
	PORTA->ISFR = 0xffffffff;
}
