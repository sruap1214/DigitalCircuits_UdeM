#include "clock_config.h"
#include "MK64F12.h"

#define LED_BLUE 21
#define MASK(x) (1UL << (x))

void init_rgb(void);

/* Force the counter to be placed into memory. */
volatile uint32_t band = 0 ;


int main(void) {
    /* Init board hardware. */
    BOARD_InitBootClocks();
    init_rgb();
    /* Init SysTick to 120000 of ticks = 1 ms */
    SysTick_Config(120000UL);
    while(1) {
    	if(band>=1000){
			PTB->PTOR |= MASK(LED_BLUE);
			band = 0;
    	}
    }
    return 0;

}

void SysTick_Handler(void){
	band++;
}

void init_rgb(void){
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK; // habilita el clock
	PORTB->PCR[LED_BLUE] = PORT_PCR_MUX(1); // pone como GPIO
	PTB->PDDR |= MASK(LED_BLUE); // lo pone como salida
	PTB->PCOR |= MASK(LED_BLUE); // lo apaga
}
