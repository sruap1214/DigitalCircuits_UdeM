#include "MK64F12.h"

#define LED_BLUE (21)
#define LED_RED (22)
#define SW1_POS (4)
#define MASK(x) (1UL << (x))
#define MASK(x) (1UL << (x))

int main(void){
	// Enable Clock to Port A and B
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK;

	// Make 3 pins GPIO
	PORTB->PCR[LED_BLUE] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[LED_BLUE] |= PORT_PCR_MUX(1);
	PORTB->PCR[LED_RED] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[LED_RED] |= PORT_PCR_MUX(1);
	PORTA->PCR[SW1_POS] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[SW1_POS] |= PORT_PCR_MUX(1);


	PTB->PDDR |= MASK(LED_BLUE) | MASK(LED_RED); // set bits to outputs
	PTA->PDDR &= ~MASK(SW1_POS); // clear switch bit to input

	PTB->PDOR = MASK(LED_RED);  // turn on LED1, turn off LED2

	while (1) {
		if (PTA->PDIR & MASK(SW1_POS)) { // switch is not pressed, so light LED 2
			PTB->PDOR = MASK(LED_RED);
		} else {	                   // switch is pressed, so light LED 1
			PTB->PDOR = MASK(LED_BLUE);
		}
	}
}
