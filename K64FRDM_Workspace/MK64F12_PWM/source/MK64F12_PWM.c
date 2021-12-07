#include "clock_config.h"
#include "MK64F12.h"

#define PWM_PERIOD (15000)			// Equivale a 1 kHz = 60 MHz(bus clock)/4(preescaler) se cuenta 15000 ticks
#define LED_GREEN (1)

void Init_Green_LED_PWM(uint16_t period);

int main(void) {

    BOARD_InitBootClocks();

    uint16_t i=0;
	volatile int32_t delay;
	Init_Green_LED_PWM(PWM_PERIOD);

	// Flash forever
	while (1) {
			for (i=0; i<PWM_PERIOD; i++) {
				FTM0->CONTROLS[0].CnV = i;
				for (delay=0; delay<1000; delay++)
					;
			}
			for (i=PWM_PERIOD-1; i>0; i--) {
				FTM0->CONTROLS[0].CnV = i;
				for (delay=0; delay<1000; delay++)
					;
			}
	}
    return 0 ;
}

void Init_Green_LED_PWM(uint16_t period){
	// Enable clock to port C
	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;;

	// Blue FTM0_CH0, Mux Alt 4
	// Set pin to FTMs
	PORTC->PCR[LED_GREEN] &= ~PORT_PCR_MUX_MASK;
	PORTC->PCR[LED_GREEN] |= PORT_PCR_MUX(4);

	// Configure TPM
	SIM->SCGC6 |= SIM_SCGC6_FTM0_MASK;

	//load the counter and mod
	FTM0->MOD = period-1;
	//set TPM count direction to up (CPWMS = 0)
	//			Disable TOF interrupt
	//			No clock selected
	//		  divide by 2 prescaler
	FTM0->SC =  FTM_SC_PS(2);
	// Continue operation in debug mode
	//FTM0->CONF |= FTM_CONF_BDMMODE(0);
	// Set channel 0 to edge-aligned high-true PWM
	FTM0->CONTROLS[0].CnSC = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK;
	// Set initial duty cycle
	FTM0->CONTROLS[0].CnV = 0;
	//FTM0->CONTROLS[0].CnV = 3750;
	// Start TPM
	FTM0->SC |= FTM_SC_CLKS(1);
}
