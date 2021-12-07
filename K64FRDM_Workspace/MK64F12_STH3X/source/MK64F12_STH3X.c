#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"
#include "fsl_i2c.h"

/*   Direccion */
#define SHT31_DEFAULT_ADDR          0x44U

/*   Comandos */
#define SHT31_MEAS_HIGHREP_STRETCH  0x2C06U
#define SHT31_MEAS_MEDREP_STRETCH   0x2C0DU
#define SHT31_MEAS_LOWREP_STRETCH   0x2C10U
#define SHT31_MEAS_HIGHREP          0x2400U
#define SHT31_MEAS_MEDREP           0x240BU
#define SHT31_MEAS_LOWREP           0x2416
#define SHT31_READSTATUS            0xF32D
#define SHT31_CLEARSTATUS           0x3041
#define SHT31_SOFTRESET             0x30A2U
#define SHT31_HEATEREN              0x306D
#define SHT31_HEATERDIS             0x3066



#define I2C_Wait()				while((I2C_MasterGetStatusFlags(I2C0) & I2C_S_IICIF_MASK)==0) {} \
								I2C0->S |= I2C_S_IICIF_MASK;

void delaySimple(uint32_t delay);
void writeCommand(uint16_t cmd, uint8_t address);
void reset(void);
uint16_t readStatus(uint8_t address);
void Init_SHT3(void);
uint8_t crc8(const uint8_t *data, int len);
_Bool readTempHum(void);


volatile uint8_t bandTimer = 0;

int main(void) {

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    PRINTF("Hello World\n\r");
    Init_SHT3();

    while(1) {
    	if(bandTimer){
    		bandTimer=0;
    		readTempHum();
    	}

    }
    return 0 ;
}

void delaySimple(uint32_t delay){
	int i=0;
	for(i=0;i<delay;i++){
		asm("NOP");
	}

}
void writeCommand(uint16_t cmd, uint8_t address) {
	uint8_t buf[2];

    buf[0] = (cmd >> 8);
    buf[1] = (cmd & 0xFF);

    I2C_MasterStart(I2C0, address, kI2C_Write);							// Start transmission with Slave, write mode
    I2C_Wait(); 														// Wait for ACK
    I2C_MasterWriteBlocking(I2C0, buf, 2, kI2C_TransferDefaultFlag);	// Send command
}

void reset(void) {
    writeCommand(SHT31_SOFTRESET,SHT31_DEFAULT_ADDR);
    delaySimple(10000);
}

uint16_t readStatus(uint8_t address) {
	//Send command read status
    writeCommand(SHT31_READSTATUS,SHT31_DEFAULT_ADDR);

    // Read status
    uint8_t readbuffer[3];
    //I2C_MasterClearStatusFlags(I2C0, (uint32_t)kClearFlags);
    I2C_MasterStart(I2C0, address, kI2C_Read);	// Start transmission with Slave, read mode
    I2C_Wait();	// Wait for ACK
    I2C_MasterReadBlocking(I2C0, &readbuffer[0], 3, kI2C_TransferDefaultFlag); // read 3 data

    uint16_t stat = readbuffer[0];
    stat <<= 8;
    stat |= readbuffer[1];

    if (readbuffer[2] != crc8((uint8_t *) readbuffer, 2)) {
    	PRINTF("CRC erroneo \r\n", stat);
        return false;
    }

    PRINTF("%d\r\n", stat);
    return stat;
}
void Init_SHT3(void){
	reset();
	readStatus(SHT31_DEFAULT_ADDR);
}

uint8_t crc8(const uint8_t *data, int len) {
	uint8_t crc = 0xFF;
	size_t i, j;
	for (i = 0; i < len; i++) {
		crc ^= data[i];
		for (j = 0; j < 8; j++) {
			if ((crc & 0x80) != 0)
				crc = (uint8_t)((crc << 1) ^ 0x31);
			else
				crc <<= 1;
		}
	}
	return crc;
}


_Bool readTempHum(void) {
	uint8_t readbuffer[6];

    writeCommand(SHT31_MEAS_HIGHREP,SHT31_DEFAULT_ADDR);

    delaySimple(500000);

    I2C_MasterStart(I2C0, SHT31_DEFAULT_ADDR, kI2C_Read);
    I2C_Wait();	// Wait for ACK
    I2C_MasterReadBlocking(I2C0, &readbuffer[0], 6, kI2C_TransferDefaultFlag);

    uint16_t ST, SRH;
    ST = readbuffer[0];
    ST <<= 8;
    ST |= readbuffer[1];

    if (readbuffer[2] != crc8((uint8_t *) readbuffer, 2)) {
        return false;
    }

    SRH = readbuffer[3];
    SRH <<= 8;
    SRH |= readbuffer[4];

    if (readbuffer[5] != crc8((uint8_t *) readbuffer+3, 2)) {
        return false;
    }

    double stemp = ST;
    stemp *= 175;
    stemp /= 0xffff;
    stemp = -45 + stemp;


    double shum = SRH;
    shum *= 100;
    shum /= 0xFFFF;

    PRINTF("La temperatura fue de: %f y la humedad de %f\n\r",stemp,shum);
    return true;
}

/* PIT0_IRQn interrupt handler */
void PIT_CHANNEL_0_IRQHANDLER(void) {
  uint32_t intStatus;
  /* Reading all interrupt flags of status register */
  intStatus = PIT_GetStatusFlags(PIT_PERIPHERAL, PIT_CHANNEL_0);
  PIT_ClearStatusFlags(PIT_PERIPHERAL, PIT_CHANNEL_0, intStatus);

  /* Place your code here */
  bandTimer = 1;


  /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
     Store immediate overlapping exception return operation might vector to incorrect interrupt. */
  #if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
  #endif
}

