#include "MK64F12.h"
#include "clock_config.h"


#define D7	16
#define D6	17
#define D5	9
#define D4	1
#define RS	23
#define EN	2

#define MASK(x) (1UL << (x))


void LCD_init(void);
void Lcd_CmdWrite(unsigned char command);
void LCD_Set_Cursor(uint8_t row, uint8_t col);
void Lcd_DataWrite(unsigned char dat);
void LcdDataBus(unsigned char Data);
void LCD_Write_String(char *str);
void delayMs(int n);

int main(void){

	SIM->SCGC5 = SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK;

	BOARD_InitBootClocks();
	// LCD Init
	LCD_init();

	/*Lcd_DataWrite('H');
	Lcd_DataWrite('o');
	Lcd_DataWrite('l');
	Lcd_DataWrite('l');
	Lcd_DataWrite('a');
	Lcd_DataWrite(' ');
	Lcd_DataWrite('w');
	Lcd_DataWrite('o');
	Lcd_DataWrite('r');
	Lcd_DataWrite('l');
	Lcd_DataWrite('d');*/

	LCD_Set_Cursor(1, 5);
	LCD_Write_String("Hola Mundo!");


	while (1) {
		__NOP();
	}
}

void LCD_init(void)
{
    SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK;       /* enable clock to Port A, B and C */

    PORTA->PCR[1] = PORT_PCR_MUX(1);      /* make PTA0 pin as GPIO */
    PORTA->PCR[2] = PORT_PCR_MUX(1);      /* make PTA1 pin as GPIO */
    PORTB->PCR[9] = PORT_PCR_MUX(1);      /* make PTB9 pin as GPIO */
    PORTB->PCR[23] = PORT_PCR_MUX(1);      /* make PTB23 pin as GPIO */
    PORTC->PCR[16] = PORT_PCR_MUX(1);      /* make PTC16 pin as GPIO */
    PORTC->PCR[17] = PORT_PCR_MUX(1);      /* make PTC17 pin as GPIO */

    PTA->PDDR |= MASK(D4) | MASK(EN);          /* make all as output pins */
    PTB->PDDR |= MASK(D5) | MASK(RS);
    PTC->PDDR |= MASK(D7) | MASK(D6);

    Lcd_DataWrite(0x00);
    delayMs(30);                /* initialization sequence */
    Lcd_CmdWrite(0x33);          /* init */
    Lcd_CmdWrite(0x32);          /* init */

    Lcd_CmdWrite(0x28);          /* set 4-bit data, 2-line, 5x7 font */
    Lcd_CmdWrite(0x0C);          /* move cursor right */
    Lcd_CmdWrite(0x01);          /* clear screen, move cursor to home */
    Lcd_CmdWrite(0x06);          /* turn on display, cursor blinking */
}

/* Delay n milliseconds
 * The CPU core clock is set to MCGFLLCLK at 120 MHz in SystemInit().
 */
void delayMs(int n) {
    int i;
    int j;
    for(i = 0 ; i < n; i++)
        for(j = 0 ; j < 12000; j++) { }
}
void Lcd_CmdWrite(unsigned char cmd)
{
	LcdDataBus((cmd>>4) & 0x0F);     //Send higher nibble
    PTB->PCOR = MASK(RS);	   	   // Send LOW pulse on RS pin for selecting Command register
    PTA->PSOR = MASK(EN);	  	  // Generate a Low-to-High pulse on EN pin
    delayMs(4);
    PTA->PCOR = MASK(EN);	  	  // Generate a High-to-low pulse on EN pin


    delayMs(4);

    LcdDataBus(cmd & 0x0F); //Send Lower nibble
    PTB->PCOR = MASK(RS);	   	   // Send LOW pulse on RS pin for selecting Command register
    PTA->PSOR = MASK(EN);	  	  // Generate a Low-to-High pulse on EN pin
    delayMs(4);
    PTA->PCOR = MASK(EN);	  	  // Generate a High-to-low pulse on EN pin

    delayMs(5);
}
void Lcd_DataWrite(unsigned char dat)
{
	LcdDataBus((dat>>4) & 0x0F);     //Send higher nibble
    PTB->PSOR = MASK(RS);	   	   // Send LOW pulse on RS pin for selecting Command register
    PTA->PSOR = MASK(EN);	  	  // Generate a Low-to-High pulse on EN pin
    delayMs(4);
    PTA->PCOR = MASK(EN);	  	  // Generate a High-to-low pulse on EN pin


    delayMs(4);

    LcdDataBus(dat & 0x0F); //Send Lower nibble
    PTB->PSOR = MASK(RS);	   	   // Send LOW pulse on RS pin for selecting Command register
    PTA->PSOR = MASK(EN);	  	  // Generate a Low-to-High pulse on EN pin
    delayMs(4);
    PTA->PCOR = MASK(EN);	  	  // Generate a High-to-low pulse on EN pin

    delayMs(5);
}
void LcdDataBus(unsigned char Data){
	  if(Data & 1)
		  PTA->PSOR|= MASK(D4);
	  else
		  PTA->PCOR|= MASK(D4);
	  if(Data & 2)
		  PTB->PSOR|= MASK(D5);
	  else
		  PTB->PCOR|= MASK(D5);
	  if(Data & 4)
		  PTC->PSOR|= MASK(D6);
	  else
		  PTC->PCOR|= MASK(D6);
	  if(Data & 8)
		  PTC->PSOR|= MASK(D7);
	  else
		  PTC->PCOR|= MASK(D7);
}
void LCD_Write_String(char *str)
{
  int i;
  for(i=0;str[i]!='\0';i++)
	  Lcd_DataWrite(str[i]);
}


void LCD_Set_Cursor(uint8_t row, uint8_t col)
{
	unsigned char offset;
	if(row == 0){
		offset = 0x00;
	}else{
		offset = 0x40;
	}Lcd_CmdWrite(0x80 | (col + offset));
}
