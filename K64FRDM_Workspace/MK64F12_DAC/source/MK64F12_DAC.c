#include "MK64F12.h"

#define DAC_RESOLUTION (4096)

void Init_DAC(void);
void Triangle_Output(void);

int main(void) {
	Init_DAC();
	Triangle_Output();
    return 0 ;
}

void Init_DAC(void) {
  // Enable clock to DAC
	//SIM->SCGC6 |= SIM_SCGC6_DAC0_MASK;
	SIM->SCGC2 |= SIM_SCGC2_DAC0_MASK;

	// Disable buffer mode
	DAC0->C1 = (uint8_t)0;

	// Enable DAC, select VDDA as reference voltage
	DAC0->C0 = (uint8_t)(DAC_C0_DACEN_MASK | DAC_C0_DACRFS_MASK);
}

void Triangle_Output(void) {
	int i=0, change=1;

	while (1) {
		DAC0->DAT[0].DATL = DAC_DATL_DATA0(i);
		DAC0->DAT[0].DATH = DAC_DATH_DATA1(i >> 8);

		i += change;
		if (i == 0) {
			change = 1;
		} else if (i == DAC_RESOLUTION-1) {
			change = -1;
		}
	}
}
