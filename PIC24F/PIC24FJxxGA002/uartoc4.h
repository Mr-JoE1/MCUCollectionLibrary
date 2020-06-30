#ifndef UARTOC4_H_INCLUDED
#define UARTOC4_H_INCLUDED

#include "gpio.h"

//hardware configuration
//end hardware configuration

//global defines

//global variables

//initialize software uart / CCP
//only TX is implemented
//time base TMRx, use output compare ch 1
void uartoc4_init(uint32_t bps);

//send a char
void uartoc4_put(char ch);

//send a string
void uartoc4_puts(char *str);

//test for busy SIGNAL
char uartoc4_busy(void);

#endif /* UARTOC1_H_INCLUDED */
