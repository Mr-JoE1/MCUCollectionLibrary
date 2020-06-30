#ifndef EXTINT_H_INCLUDED
#define EXTINT_H_INCLUDED

//#include <pic24.h>						//we use pic24f
#include "gpio.h"

//hardware configuration
//#define INT0_PIN		7				//map int0 to rp7 - int0 cannot be remapped
#define INT1_PIN		8				//map int1 to rp8
#define INT2_PIN		9				//map int1 to rp8
//end hardware configuration

//reset the external interrupt
void int0_init(void);

//install user isr
void int0_act(void (*isr_ptr) (void));

//reset the external interrupt
void int1_init(void);

//install user isr
void int1_act(void (*isr_ptr) (void));
//reset the external interrupt
void int2_init(void);

//install user isr
void int2_act(void (*isr_ptr) (void));

#endif
