#include "pwmta0.h"							//we use tmrb0

//hardware configuration
//end hardware configuration

//global defines
//for easy portability
#define TAxCTL				TA0CTL			//timera control
#define TAxR				TA0R			//timer / counter
#define TAxIV				TA0IV			//interrupt flag
#define TAxEX0				TA0EX0			//expansion register

#define TAxCCTL0			TA0CCTL0		//timer0 compare control
#define TAxCCTL1			TA0CCTL1		//timer1 cmpare control
#define TAxCCTL2			TA0CCTL2		//timer2 compare control
#define TAxCCTL3			TA0CCTL3		//timer0 compare control
#define TAxCCTL4			TA0CCTL4		//timer1 cmpare control
#define TAxCCTL5			TA0CCTL5		//timer2 compare control
#define TAxCCTL6			TA0CCTL6		//timer0 compare control
#define TAxCCR0				TA0CCR0			//timer0 compare register
#define TAxCCR1				TA0CCR1			//timer1 compare register
#define TAxCCR2				TA0CCR2			//timer2 compare register
#define TAxCCR3				TA0CCR3			//timer0 compare register
#define TAxCCR4				TA0CCR4			//timer1 compare register
#define TAxCCR5				TA0CCR5			//timer2 compare register
#define TAxCCR6				TA0CCR6			//timer0 compare register
//global variables

//initialize timer_a, ccr0
//continous mode
//up (default) or up/down mode
void pwmta0_init(uint16_t ps, uint16_t pr) {
	//clear the timer control register
	TAxCTL |= TACLR;							//TxCTL_bit.TACLR;						//set to reset timer a

	//stop timer
	TAxCTL &=~MC_0;								//TxCTL_bit.TAMC = 0x00;				//timer mode - 0x00: halted, 0x01: up mode, 0x02: continuous mode (to 0xffff), 0x03: up/down
	//TxCTL |= TASSEL_2;						//TxCTL_bit.TASSEL = 0x02;				//timer source - 0x00: TACLK, 0x01: ACLK, 0x02: SMCLK, 0x03: INCLK
	//TxCTL |= (ps & ID_3);						//TxCTL_bit.TAID = ps & 0x03;			//input divider - 0x00: /1, 0x01: /2, 0x02: /4, 0x03: /8
	//TxCTL &=~TAIE;							//TxCTL_bit.TAIE = 0;					//timer a interrupt enable: 0: interrupt disabled; 1: interrupt enabled;
	//TxCTL &=~TAIFG;							//TxCTL_bit.TAIFG = 0;					//interrupt flag: 0= no interrupt pending; 1=interrupt pending
	//TxCTL |= OUTMOD0;							//set output mode to zero
	TAxCCR0 = pr - 1;							//set the period
#if defined(TA0EX0_)
	TAxEX0 = (ps & TMRA_EX8x);	//set expansion divider
#endif
	TAxCTL = 	TASSEL_2 |						//0=TACLK, 1=ACLK, 2=SMCLK, and 3=INCLK
				(ps & TMRA_PS8x) |				//prescaler
				MC_1 |							//0=stop, 1=up to ccr0, 2=continous mode, 3=up and down
				0x00;							//TCIE disabled
	//timer is running now
	
#if defined(PWM_TA0CH1)
	//TA0CH1 output to be activated
	//set up TAxCCTLx register
	TAxCCTL1 = 	CM_0 |						//0=no capture
				CCIS_0 |					//0=input is CCIxA
				(0 << 11) |					//0=asynchronous, 1=synchronous
				(0 << 8) |					//0=compare, 1=capture
				OUTMOD_7 |					//3:reset on match/set on roll-over
				(0 << 4) |					//1=compare / capture interrupt enabled, 0=disabled
				(0 << 2) |					//0=OUT is low, 1=OUT is high
				(0x00);
	TAxCCR1 = 0;							//duty cycle starts at 0
	PWM_TA0CH1();							//activate the pin
#endif

#if defined(PWM_TA0CH2)
	//TA0CH2 output to be activated
	//set up TAxCCTLx register
	TAxCCTL2 = 	CM_0 |					//0=no capture
				CCIS_0 |					//0=input is CCIxA
				(0 << 11) |					//0=asynchronous, 1=synchronous
				(0 << 8) |					//0=compare, 1=capture
				OUTMOD_7 |					//3:reset on match/set on roll-over
				(0 << 4) |					//1=compare / capture interrupt enabled, 0=disabled
				(0 << 2) |					//0=OUT is low, 1=OUT is high
				(0x00);
	TAxCCR2 = 0;							//duty cycle starts at 0
	PWM_TA0CH2();							//activate the pin
#endif

}

//set duty cycles on ch1
void pwmta0_setdc1(uint16_t dc) {
	TAxCCR1 = dc;
}
//set ducy cycles on ch2
void pwmta0_setdc2(uint16_t dc) {
	TAxCCR2 = dc;
}
//set ducy cycles on ch3
#if defined(TA0CCR3)
void pwmta0_setdc3(uint16_t dc) {
	TAxCCR3 = dc;
}
#endif

#if defined(TA0CCR4)
//set ducy cycles on ch4
void pwmta0_setdc4(uint16_t dc) {
	TAxCCR4 = dc;
}
#endif

#if defined(TA0CCR5)
//set ducy cycles on ch5
void pwmta0_setdc5(uint16_t dc) {
	TAxCCR5 = dc;
}
#endif

#if defined(TA0CCR6)
//set ducy cycles on ch6
void pwmta0_setdc6(uint16_t dc) {
	TAxCCR6 = dc;
}
#endif

