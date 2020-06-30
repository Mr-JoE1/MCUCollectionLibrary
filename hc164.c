//#include <regx51.h>
//#include <iostm8.h>								//we use stm8s
#include "gpio.h"
#include "hc164.h"

void hc164_init(void) {
	IO_CLR(HC164_PORT, HC164_SCK | HC164_SDO);	//clear sck / sdo
	IO_OUT(HC164_DDR, HC164_SCK | HC164_SDO);	//sck / sdo as output
}

void hc164_write(unsigned char byte_t) {
	unsigned char mask = 0x80;

	do {
		IO_CLR(HC164_PORT, HC164_SCK);			//clear sck
		if (byte_t & mask) IO_SET(HC164_PORT, HC164_SDO);	//send 1
		else IO_CLR(HC164_PORT, HC164_SDO);		//send 0
		IO_SET(HC164_PORT, HC164_SCK);			//send data on sck's rising edge
		mask = mask >> 1;
	} while (mask);
	IO_CLR(HC164_PORT, HC164_SCK);				//clear sck
}
