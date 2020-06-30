#ifndef UARTOC1_H_INCLUDED
#define UARTOC1_H_INCLUDED

#include "gpio.h"

//hardware configuration
//end hardware configuration

//global defines

//global variables

//initialize software uart / CCP
//only TX is implemented
//time base TMRx, use output compare ch 1
void uartoc1_init(uint32_t bps);

//send a char
void uartoc1_put(char ch);

//send a string
void uartoc1_puts(char *str);

//test for busy SIGNAL
char uartoc1_busy(void);

#endif /* UARTOC1_H_INCLUDED */
