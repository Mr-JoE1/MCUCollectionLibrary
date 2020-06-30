/*
 * tmra0.h
 *
 *  Created on: Oct 1, 2016
 *      Author: Ken
 */

#ifndef TA0OC_H_
#define TA0OC_H_



//#include "msp.h"
#include "gpio.h"					//we use gpio

//hardware configuration
//end hardware configuration

//global defines
#define TMRA_PS1x		(0b00<<6)	//prescaler @ 1x
#define TMRA_PS2x		(0b01<<6)	//prescaler @ 2x
#define TMRA_PS4x		(0b10<<6)	//prescaler @ 4x
#define TMRA_PS8x		(0b11<<6)	//prescaler @ 8x

#define TMRA_EX1x		0b000		//expansion divider
#define TMRA_EX2x		0b001		//expansion divider
#define TMRA_EX3x		0b010		//expansion divider
#define TMRA_EX4x		0b011		//expansion divider
#define TMRA_EX5x		0b100		//expansion divider
#define TMRA_EX6x		0b101		//expansion divider
#define TMRA_EX7x		0b110		//expansion divider
#define TMRA_EX8x		0b111		//expansion divider

//initialize tmr_a
//continuous mode
//ps. bit 7..6: input divider, bit 2..0: expansion divider
void tmra0_init(uint16_t ps);

//initialize tmra_ccr0
void tmra0_setpr0(uint16_t pr);

//activate ccr0 interrupt handler
void tmra0_act0(void (*isrptr)(void));

//initialize tmra_ccr1
void tmra0_setpr1(uint16_t pr);

//activate ccr1 interrupt handler
void tmra0_act1(void (*isrptr)(void));

//initialize tmra_ccr2
void tmra0_setpr2(uint16_t pr);

//activate ccr0 interrupt handler
void tmra0_act2(void (*isrptr)(void));

//initialize tmra_ccr0
void tmra0_setpr3(uint16_t pr);

//activate ccr0 interrupt handler
void tmra0_act3(void (*isrptr)(void));

//initialize tmra_ccr0
void tmra0_setpr4(uint16_t pr);

//activate ccr0 interrupt handler
void tmra0_act4(void (*isrptr)(void));

//initialize tmra_ccr0
void tmra0_setpr5(uint16_t pr);

//activate ccr0 interrupt handler
void tmra0_act5(void (*isrptr)(void));

//initialize tmra_ccr0
void tmra0_setpr6(uint16_t pr);

//activate ccr0 interrupt handler
void tmra0_act6(void (*isrptr)(void));



#endif /* TMRA0_H_ */
