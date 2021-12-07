#include "MK64F12.h"
#include "clock_config.h"

#define SW1	4
#define SW2 6
#define LED_BLUE 21
#define LED_RED 22
#define LED_GREEN 26
#define MASK(x) (1UL << (x))
#define W_DELAY_SLOW	500000
#define W_DELAY_FAST	100000
#define RGB_DELAY_SLOW	2000000		/*Equivalente a 2s*/
#define RGB_DELAY_FAST	500000		/*Equivalente a 0.5s*/

volatile uint8_t  bandSW1 = 0;
volatile uint8_t  bandSW2 = 0;
volatile uint8_t  bandPIT = 0;
volatile uint8_t g_flash_LED = 0;
volatile uint32_t g_w_delay = W_DELAY_SLOW;
volatile uint32_t g_RGB_delay = RGB_DELAY_SLOW;


void init_switch(void);
void init_rgb(void);
void Task_RGB_FSM_Timer(void);
void Task_WB_FSM_Timer(void);
void Control_RGB_LEDs(uint8_t led_r,uint8_t led_g,uint8_t led_b);
void Init_PIT(uint8_t channel, unsigned period_us, uint8_t gen_interrupts);
void Start_PIT(uint8_t channel);
void Stop_PIT(uint8_t channel);


int main(void) {
	/*Initialize clocks*/
	BOARD_InitBootClocks();

	__disable_irq();
	init_rgb();
	init_switch();
	__enable_irq();
	//__get_PRIMASK();

    while(1) {
        Task_RGB_FSM_Timer();
        Task_WB_FSM_Timer();
    }
    return 0 ;
}

void init_rgb(void){
	/*Activate clock of ports*/
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK| SIM_SCGC5_PORTE_MASK;

	/* Use pins as GPIO */
	PORTB->PCR[LED_BLUE] = PORT_PCR_MUX(1);
	PORTB->PCR[LED_RED] = PORT_PCR_MUX(1);
	PORTE->PCR[LED_GREEN] = PORT_PCR_MUX(1);

	/* Use pin as digital outputs */
	PTB->PDDR |= MASK(LED_BLUE)|MASK(LED_RED);
	PTE->PDDR |= MASK(LED_GREEN);

	/* Turn off all leds*/
	PTB->PSOR |= MASK(LED_BLUE)|MASK(LED_RED);
	PTE->PSOR |= MASK(LED_GREEN);


}
void init_switch(void){
	/*Activate clock of ports*/
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK| SIM_SCGC5_PORTC_MASK;

	/* Use pins as GPIO and interrupt on falling edge*/
	PORTA->PCR[SW1] = PORT_PCR_MUX(1) | PORT_PCR_IRQC(0x0a);
	PORTC->PCR[SW2] = PORT_PCR_MUX(1) | PORT_PCR_IRQC(0x0a);

	/* Use pins an digital inputs*/
	PTA->PDDR &= ~MASK(SW1);
	PTC->PDDR &= ~MASK(SW2);

	/* Enable Interrupts */
	NVIC_SetPriority(PORTA_IRQn, 2);
	NVIC_ClearPendingIRQ(PORTA_IRQn);
	NVIC_EnableIRQ(PORTA_IRQn);

	NVIC_SetPriority(PORTC_IRQn, 2);
	NVIC_ClearPendingIRQ(PORTC_IRQn);
	NVIC_EnableIRQ(PORTC_IRQn);

}

void PORTA_IRQHandler(void) {
	if ((PORTA->ISFR & MASK(SW1))) {
		g_flash_LED=~g_flash_LED;
	}
	// clear status flags
	PORTA->ISFR = 0xffffffff;
}

void PORTC_IRQHandler(void) {
	if ((PORTC->ISFR & MASK(SW2))) {
		if(bandSW2){
			g_w_delay = W_DELAY_SLOW;
			g_RGB_delay = RGB_DELAY_SLOW;
			bandSW2=0;
		}else{
			g_w_delay = W_DELAY_FAST;
			g_RGB_delay = RGB_DELAY_FAST;
			bandSW2=1;
		}

	}
	// clear status flags
	PORTC->ISFR = 0xffffffff;
}

void PIT0_IRQHandler(void){
	// check to see which channel triggered interrupt
	if (PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK) {
		// clear status flag for timer channel 0
		PIT->CHANNEL[0].TFLG &= PIT_TFLG_TIF_MASK;

		bandPIT = 1;
		PTE->PTOR |= MASK(LED_GREEN);

	}
}

void Task_RGB_FSM_Timer(void) {
	static enum {ST_RED, ST_RED_WAIT, ST_GREEN, ST_GREEN_WAIT, ST_BLUE, ST_BLUE_WAIT} next_state = ST_RED;

	if (!g_flash_LED) { 	// only run task when NOT in flash mode
		switch (next_state) {
			case ST_RED:
				Control_RGB_LEDs(1, 0, 0);
				Init_PIT(0, g_RGB_delay,1);
				Start_PIT(0);
				next_state = ST_RED_WAIT;
				break;
			case ST_RED_WAIT:
				if (bandPIT) {
					Stop_PIT(0);
					bandPIT=0;
					next_state = ST_GREEN;
				}
				break;
			case ST_GREEN:
				Control_RGB_LEDs(0, 1, 0);
				Init_PIT(0, g_RGB_delay,1);
				Start_PIT(0);
				next_state = ST_GREEN_WAIT;
				break;
			case ST_GREEN_WAIT:
				if (bandPIT) {
					Stop_PIT(0);
					bandPIT=0;
					next_state = ST_BLUE;
				}
				break;
			case ST_BLUE:
				Control_RGB_LEDs(0, 0, 1);
				Init_PIT(0, g_RGB_delay,1);
				Start_PIT(0);
				next_state = ST_BLUE_WAIT;
				break;
			case ST_BLUE_WAIT:
				if (bandPIT) {
					Stop_PIT(0);
					bandPIT=0;
					next_state = ST_RED;
				}
				break;
			default:
				next_state = ST_RED;
				break;
		}
	}else{
		next_state = ST_RED;
	}
}
void Task_WB_FSM_Timer(void) {
	static enum {ST_WHITE, ST_WHITE_WAIT, ST_BLACK, ST_BLACK_WAIT} next_state = ST_WHITE;

	if (g_flash_LED) { 	// only run task when NOT in flash mode
		switch (next_state) {
			case ST_WHITE:
				Control_RGB_LEDs(1, 1, 1);
				Init_PIT(0, g_w_delay,1);
				Start_PIT(0);
				next_state = ST_WHITE_WAIT;
				break;
			case ST_WHITE_WAIT:
				if (bandPIT) {
					Stop_PIT(0);
					bandPIT=0;
					next_state = ST_BLACK;
				}
				break;
			case ST_BLACK:
				Control_RGB_LEDs(0, 0, 0);
				Init_PIT(0, g_w_delay,1);
				Start_PIT(0);
				next_state = ST_BLACK_WAIT;
				break;
			case ST_BLACK_WAIT:
				if (bandPIT) {
					Stop_PIT(0);
					bandPIT=0;
					next_state = ST_WHITE;
				}
				break;
			default:
				next_state = ST_WHITE;
				break;
		}
	}else{
		next_state = ST_WHITE;
	}
}

void Control_RGB_LEDs(uint8_t led_r,uint8_t led_g,uint8_t led_b){
	if(led_r){
		PTB->PCOR |= MASK(LED_RED);
	}else{
		PTB->PSOR |= MASK(LED_RED);
	}
	if(led_g){
		PTE->PCOR |= MASK(LED_GREEN);
	}else{
		PTE->PSOR |= MASK(LED_GREEN);
	}
	if(led_b){
		PTB->PCOR |= MASK(LED_BLUE);
	}else{
		PTB->PSOR |= MASK(LED_BLUE);
	}

}

void Init_PIT(uint8_t channel, unsigned period_us, uint8_t gen_interrupts) {
	// Enable clock to PIT module
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;

	// Enable module, freeze timers in debug mode
	PIT->MCR &= ~PIT_MCR_MDIS_MASK;
	PIT->MCR |= PIT_MCR_FRZ_MASK;

	// Initialize PIT to count down from argument
	PIT->CHANNEL[channel].LDVAL = PIT_LDVAL_TSV((period_us*60)-1); // 60 MHz bus clock frequency

	// No chaining
	PIT->CHANNEL[channel].TCTRL &= PIT_TCTRL_CHN_MASK;

	if (gen_interrupts) {
		// Generate interrupts
		PIT->CHANNEL[channel].TCTRL |= PIT_TCTRL_TIE_MASK;

		// Configure NVIC
		NVIC_SetPriority(PIT0_IRQn+channel, 64); // 0, 64, 128 or 192
		NVIC_ClearPendingIRQ(PIT0_IRQn+channel);
		NVIC_EnableIRQ(PIT0_IRQn+channel);
	}
}
void Start_PIT(uint8_t channel) {
// Enable counter
	PIT->CHANNEL[channel].TCTRL |= PIT_TCTRL_TEN_MASK;
}

void Stop_PIT(uint8_t channel) {
// Disable counter
	PIT->CHANNEL[channel].TCTRL &= ~PIT_TCTRL_TEN_MASK;
}

