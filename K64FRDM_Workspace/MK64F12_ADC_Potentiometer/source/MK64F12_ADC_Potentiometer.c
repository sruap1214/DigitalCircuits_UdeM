#include "MK64F12.h"

#define ADC_CHANNEL (1)

void Init_ADC(void);

int main(void) {

    volatile static int i = 0 ;
    float result;

    Init_ADC();

    while(1) {
    	ADC0->SC1[0] = ADC_CHANNEL;           /* start conversion on channel 0 */
    	while(!(ADC0->SC1[0] & ADC_SC1_COCO_MASK)) { } /* wait for conversion complete */
    	result = (float)ADC0->R[0];        /* read conversion result and clear COCO flag */
    	result = 3.3*(result/65536);

        i++ ;
        /* 'Dummy' NOP to allow source level single stepping of
            tight while() loop */
        __asm volatile ("nop");
    }
    return 0 ;
}

void Init_ADC(void) {

	SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;

	// Low power configuration, long sample time, 16 bit single-ended conversion, bus clock input
	ADC0->CFG1 = ADC_CFG1_ADLPC_MASK | ADC_CFG1_ADLSMP_MASK | ADC_CFG1_MODE(3) | ADC_CFG1_ADICLK(0);

	// Software trigger, compare function disabled, DMA disabled, voltage references VREFH and VREFL
	ADC0->SC2 = ADC_SC2_REFSEL(0);
}
