#ifndef PWM4_H_INCLUDED
#define PWM4_H_INCLUDED

#include "gpio.h"
//#include "delay.h"						//we use software delays

//hardware configuration
#define PWM4_TO_RP()						PPS_OC4_TO_RPA4()
//end hardware configuration

//global defines
//TMR prescaler
#define TMRPS_1x				0
#define TMRPS_2x				1
#define TMRPS_4x				2
#define TMRPS_8x				3
#define TMRPS_16x				4
#define TMRPS_32x				5
#define TMRPS_64x				6
#define TMRPS_256x				7

//global variables

//reset pwm
//ps: 3-bit prescaler, 1:1->256:1
//pr: period register, 16-bit
void pwm4_init(uint32_t ps, uint32_t pr);

//set pwm duty cycle parameters
//void pwm4_setdc(unsigned short duty_cycle);
#define pwm4_setdc(dc)				OC4RS = (dc)
#define pwm4_getdc()				(OC4RS)

#endif /* PWM4_H_INCLUDED */
