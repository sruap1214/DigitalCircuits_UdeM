#include <stdio.h>
#include "MK64F12.h"


# define PIN_RED(x)		(x)<<22
# define PIN_BLUE(x)	(x)<<21
# define PIN_GREEN(x)	(x)<<26

int main(void) {

	// Clock habilitados para los modulos
	SIM->SCGC5|=1<<10;	// Activar el Clock del puerto B
	SIM->SCGC5|=SIM_SCGC5_PORTB(1); // Activar el clock del puerto B

	SIM->SCGC5|=1<<13;  // Activar el Clock del puerto E
	SIM->SCGC5|=SIM_SCGC5_PORTE(1);  // Activar el Clock del puerto E

	// Poner puerto como GPIO
	PORTB->PCR[21]|=PORT_PCR_MUX(1);
	PORTB->PCR[22]|=PORT_PCR_MUX(1);
	PORTE->PCR[26]|=PORT_PCR_MUX(1);

	// Configurar pines como salidas
	GPIOB->PDDR|=1<<21;
	GPIOB->PDDR|=1<<22;
	GPIOE->PDDR|=1<<26;

	// Poner 1 en la salida.
	GPIOB->PSOR|=1<<21;
	GPIOB->PSOR|=1<<22;
	GPIOE->PSOR|=1<<26;

    volatile static int i = 0 ;
    while(1) {
        i++ ;
        __asm volatile ("nop");

        // Escribe 1 al pin
        GPIOB->PCOR|=PIN_RED(1);
        GPIOB->PSOR|=PIN_RED(1);

        GPIOB->PCOR|=PIN_BLUE(1);
        GPIOB->PSOR|=PIN_BLUE(1);

        GPIOE->PCOR|=PIN_GREEN(1);
        GPIOE->PSOR|=PIN_GREEN(1);

        // Toogle
        /*
        GPIOB->PTOR|=1<<21;
        GPIOB->PTOR|=1<<22;
        GPIOE->PTOR|=1<<26;
        */
    }
    return 0 ;
}
