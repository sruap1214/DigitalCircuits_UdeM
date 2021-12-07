#include "MK64F12.h"

#define SW1	4
#define LED_BLUE 21
#define LED_RED 22
#define LED_GREEN 26
#define MASK(x) (1UL << (x))
#define __wfi()	__asm volatile ("nop")

volatile uint8_t  band = 0;

void init_switch(void);
void init_rgb(void);
void RGB_task(void);
void Control_RGB_LEDs(uint8_t red,uint8_t green, uint8_t blue);

int main(void) {
	__disable_irq();            /* disable all IRQs */
	init_rgb();
	init_switch();
	__enable_irq();             /* global enable IRQs */
	__set_PRIMASK();
    while(1) {
        RGB_task();
        __wfi();
    }
    return 0 ;
}

void init_rgb(void){
	//Clock activation
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTE_MASK;

	// Port as GPIO
	PORTB->PCR[LED_BLUE] = PORT_PCR_MUX(1);
	PORTB->PCR[LED_RED] = PORT_PCR_MUX(1);
	PORTE->PCR[LED_GREEN] = PORT_PCR_MUX(1);

	// GPIO as output
	PTB->PDDR |= MASK(LED_BLUE)|MASK(LED_RED);
	PTE->PDDR |= MASK(LED_GREEN);


	// Turn off led. putting one
	PTB->PSOR |= MASK(LED_BLUE)|MASK(LED_RED);
	PTE->PSOR |= MASK(LED_GREEN);


}
void init_switch(void){
	//Clock activation
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK; //| SIM_SCGC5_PORTC_MASK;

	// Port as GPIO and interrupt in falling edge
	PORTA->PCR[SW1] = PORT_PCR_MUX(1) | PORT_PCR_IRQC(0x0a);
	//PORTA->PCR[SW2] = PORT_PCR_MUX(1) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_IRQC(0x0a);

	// GPIO as input
	PTA->PDDR &= ~MASK(SW1);

	/* Enable Interrupts */
	NVIC_SetPriority(PORTA_IRQn, 2);
	NVIC_ClearPendingIRQ(PORTA_IRQn);
	NVIC_EnableIRQ(PORTA_IRQn);

}
void RGB_task(void){
	static enum {L_RED,L_GREEN,L_BLUE} next_state=L_RED;
	if(band==1){
		switch(next_state){
			case L_RED:
				Control_RGB_LEDs(1,0,0);
				next_state=L_GREEN;
				break;
			case L_GREEN:
				Control_RGB_LEDs(0,1,0);
				next_state=L_BLUE;
				break;
			case L_BLUE:
				Control_RGB_LEDs(0,0,1);
				next_state=L_RED;
				break;
		}
		band = 0;
	}
}

void PORTA_IRQHandler(void) {
	if ((PORTA->ISFR & MASK(SW1))) {
		band=1;
	}
	// clear status flags
	PORTA->ISFR = 0xffffffff;
}

void Control_RGB_LEDs(uint8_t red,uint8_t green, uint8_t blue){
	if(red){
		PTB->PCOR |= MASK(LED_RED);
	}else{
		PTB->PSOR |= MASK(LED_RED);
	}
	if(green){
		PTE->PCOR |= MASK(LED_GREEN);
	}else{
		PTE->PSOR |= MASK(LED_GREEN);
	}
	if(blue){
		PTB->PCOR |= MASK(LED_BLUE);
	}else{
		PTB->PSOR |= MASK(LED_BLUE);
	}

}
