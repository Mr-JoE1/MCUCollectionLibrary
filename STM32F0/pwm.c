#include "pwm.h"					//we use pwm

//hardware configuration
//end hardware configuration

//global defines

//global variables

/*
MAPR Bits 7:6 TIM1_REMAP[1:0]: TIM1 remapping
These bits are set and cleared by software. They control the mapping of TIM1 channels 1 to 4, 1N to 3N, external trigger (ETR) and Break input (BKIN) on the GPIO ports.
00: No remap (ETR/PA12, CH1/PA8, CH2/PA9, CH3/PA10, CH4/PA11, BKIN/PB12, CH1N/PB13, CH2N/PB14, CH3N/PB15)
01: Partial remap (ETR/PA12, CH1/PA8, CH2/PA9, CH3/PA10, CH4/PA11, BKIN/PA6, CH1N/PA7, CH2N/PB0, CH3N/PB1)
10: not used
11: Full remap (ETR/PE7, CH1/PE9, CH2/PE11, CH3/PE13, CH4/PE14, BKIN/PE15, CH1N/PE8, CH2N/PE10, CH3N/PE12)
*/
//initialize pwm to TxCCP_PS (prescaler) and TxCCP_PR (period)
void pwm1_init(uint16_t TxCCP_PS, uint16_t TxCCP_PR) {
	//route the clock to timer
	//route the clock to timer
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

	//source from internal clock -> disable slave mode
	TIM1->SMCR &=~TIM_SMCR_SMS;			//clear sms->use internal clock

	//stop the timer to configure it
	//TIM1->CR1 &=~TIM_CR1_CEN;			//clear cen. 0=disable the timer, 1=enable the timer
	//TIM1->CR1 &=~TIM_CR1_CKD;			//clear CKD0..1. 0b00->1x clock; 0b01->2:1 clock, 0b10->4:1 clk; 0b11->reserved
	//TIM1->CR1 &=~TIM_CR1_DIR;			//clear DIR bit. 0=upcounter, 1=downcounter
	//TIM1->CR1 &=~TIM_CR1_OPM;			//clear opm bit. 0=periodic timer, 1=one-shot timer
	//or to simply zero the register
	//TIM1->CR1 = 0;						//much easier
	TIM1->CR1 =	(0<<8) |				//0->1:1 clock, 1->2:1 clock, 2->4:1 clock, 3->reserved
				(0<<7) |				//1->APR buffered, 0->APR not buffered
				(0<<5) |				//0->edge-aligned, 1->center-aligned mode 1, 2->center-aligned mode 2, 3->center-aligned mode 3
				(0<<4) |				//0->upcounter, 1->downcounter
				(0<<3) |				//0->continous mode, 1->one pulse mode
				(0<<2) |				//update request source
				(0<<1) |				//0->UEV enabled, 1->UEV disabled
				(0<<0) |				//0->counter disabled, 1->counter enabled
				0x00;
	TIM1->CR2 = 0;						//default value
	TIM1->SMCR= 0;						//default value

	//set the prescaler
	TIM1->PSC = TxCCP_PS - 1;					//set the prescaler
	TIM1->RCR = 0;						//repetition counter = 0 (=no repetition)
	TIM1->ARR = TxCCP_PR - 1;						//auto reload register / period = 0; - need to change for downcounters
	TIM1->CNT = 0;						//reset the counter

	//clear the status register bits for capture / compare flags
	TIM1->SR &=~(TIM_SR_CC1IF | TIM_SR_CC2IF | TIM_SR_CC3IF | TIM_SR_CC4IF | TIM_SR_UIF);
	//disable the interrupt by clearing the enable bits
	TIM1->DIER &=~(TIM_DIER_CC1IE | TIM_DIER_CC2IE | TIM_DIER_CC3IE | TIM_DIER_CC4IE | TIM_DIER_UIE);

	//configure CCP1..4
#if defined(TIM1CH1toGPIO)
	//configure CCP1
	TIM1->CCMR1 = 	(TIM1->CCMR1 &~0x00ff) |
					(0<<7) |			//0->OC1REF not affedted by ETRF, 1->OC1REF cleared by ETRF high
					(6<<4) |			//0->frozen (for time base), 1->active on match, 2->inactive on match, 3->toggle, 4->inactive, 5->active, 6->pwm mode 1, 7->pwm mode 2
					(0<<3) |			//0->preload disabled, 1->preload enabled
					(0<<2) |			//0->fast disabled, 1->fast enabled
					(0<<0) |			//0->ch1 as output, 1->ch1 as input, 2->ch1 as input, 3->ch1 as input
					0x00;
	TIM1->CCER = 	(TIM1->CCER &~(0x0f << 0)) |
					(0<< 3) |			//0->normal polarity for CC1N, 1->inverse polarity
					(0<< 2) |			//0->disable CC1N, 1->enable CC1N
					(0<< 1) |			//0->normal polarity for CC1, 1->inverse polarity
					(1<< 0) |			//1->enable CC1, 0->disable CC1
					0x00;
	TIM1->CCR1 = 0;						//0% duty cycle

	//configure gpio
	TIM1CH1toGPIO();					//route TIM1CH1 to GPIO
#endif

#if defined(TIM1CH2toGPIO)
	//configure CCP2
	TIM1->CCMR1 = 	(TIM1->CCMR1 &~0xff00) |
					(0<<15) |			//0->OC1REF not affedted by ETRF, 1->OC1REF cleared by ETRF high
					(6<<12) |			//0->frozen (for time base), 1->active on match, 2->inactive on match, 3->toggle, 4->inactive, 5->active, 6->pwm mode 1, 7->pwm mode 2
					(0<<11) |			//0->preload disabled, 1->preload enabled
					(0<<10) |			//0->fast disabled, 1->fast enabled
					(0<<8) |			//0->ch1 as output, 1->ch1 as input, 2->ch1 as input, 3->ch1 as input
					0x00;
	TIM1->CCER = 	(TIM1->CCER &~(0x0f << 4)) |
					(0<< 7) |			//0->normal polarity for CC2N, 1->inverse polarity
					(0<< 6) |			//0->disable CC2N, 1->enable CC2N
					(0<< 5) |			//0->normal polarity for CC2, 1->inverse polarity
					(1<< 4) |			//1->enable CC2, 0->disable CC2
					0x00;
	TIM1->CCR2 = 0;						//0% duty cycle

	//configure gpio
	TIM1CH2toGPIO();					//route TIM1CH1 to GPIO
#endif

#if defined(TIM1CH3toGPIO)
	//configure CCP3
	TIM1->CCMR2 = 	(TIM1->CCMR1 &~0x00ff) |
					(0<<7) |			//0->OC1REF not affedted by ETRF, 1->OC1REF cleared by ETRF high
					(6<<4) |			//0->frozen (for time base), 1->active on match, 2->inactive on match, 3->toggle, 4->inactive, 5->active, 6->pwm mode 1, 7->pwm mode 2
					(0<<3) |			//0->preload disabled, 1->preload enabled
					(0<<2) |			//0->fast disabled, 1->fast enabled
					(0<<0) |			//0->ch1 as output, 1->ch1 as input, 2->ch1 as input, 3->ch1 as input
					0x00;
	TIM1->CCER = 	(TIM1->CCER &~(0x0f << 8)) |
					(0<<11) |			//0->normal polarity for CC3N, 1->inverse polarity
					(0<<10) |			//0->disable CC3N, 1->enable CC3N
					(0<< 9) |			//0->normal polarity for CC3, 1->inverse polarity
					(1<< 8) |			//1->enable CC3, 0->disable CC3
					0x00;
	TIM1->CCR3 = 0;						//0% duty cycle

	//configure gpio
	TIM1CH3toGPIO();					//route TIM1CH1 to GPIO
#endif

#if defined(TIM1CH4toGPIO)
	//configure CCP4
	TIM1->CCMR2 = 	(TIM1->CCMR1 &~0xff00) |
					(0<<15) |			//0->OC1REF not affedted by ETRF, 1->OC1REF cleared by ETRF high
					(6<<12) |			//0->frozen (for time base), 1->active on match, 2->inactive on match, 3->toggle, 4->inactive, 5->active, 6->pwm mode 1, 7->pwm mode 2
					(0<<11) |			//0->preload disabled, 1->preload enabled
					(0<<10) |			//0->fast disabled, 1->fast enabled
					(0<<8) |			//0->ch1 as output, 1->ch1 as input, 2->ch1 as input, 3->ch1 as input
					0x00;
	TIM1->CCER = 	(TIM1->CCER &~(0x0f << 12)) |
					(0<<15) |			//0->normal polarity for CC4N, 1->inverse polarity
					(0<<14) |			//0->disable CC4N, 1->enable CC4N
					(0<<13) |			//0->normal polarity for CC4, 1->inverse polarity
					(1<<12) |			//1->enable CC4, 0->disable CC4
					0x00;
	TIM1->CCR4 = 0;						//0% duty cycle

	//configure gpio
	TIM1CH4toGPIO();					//route TIM1CH1 to GPIO
#endif

	TIM1->EGR = 0xff;					//force an update
	TIM1->BKDR|= TIM_BKDR_MOE;				//enable the main output
	//enable the timer.
	TIM1->CR1 |= TIM_CR1_CEN;			//enable the timer

}


//initialize pwm to TxCCP_PS (prescaler) and TxCCP_PR (period)
void pwm3_init(uint16_t TxCCP_PS, uint16_t TxCCP_PR) {
	//route the clock to timer
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

	//source from internal clock -> disable slave mode
	//TIM3->SMCR &=~TIM_SMCR_SMS;			//clear sms->use internal clock

	//stop the timer to configure it
	//TIM3->CR1 &=~TIM_CR1_CEN;			//clear cen. 0=disable the timer, 1=enable the timer
	//TIM3->CR1 &=~TIM_CR1_CKD;			//clear CKD0..1. 0b00->1x clock; 0b01->2:1 clock, 0b10->4:1 clk; 0b11->reserved
	//TIM3->CR1 &=~TIM_CR1_DIR;			//clear DIR bit. 0=upcounter, 1=downcounter
	//TIM3->CR1 &=~TIM_CR1_OPM;			//clear opm bit. 0=periodic timer, 1=one-shot timer
	//or to simply zero the register
	//TIM3->CR1 = 0;						//much easier
	TIM3->CR1 =	(0<<8) |				//0->1:1 clock, 1->2:1 clock, 2->4:1 clock, 3->reserved
				(0<<7) |				//1->APR buffered, 0->APR not buffered
				(0<<5) |				//0->edge-aligned, 1->center-aligned mode 1, 2->center-aligned mode 2, 3->center-aligned mode 3
				(0<<4) |				//0->upcounter, 1->downcounter
				(0<<3) |				//0->continous mode, 1->one pulse mode
				(0<<2) |				//update request source
				(0<<1) |				//0->UEV enabled, 1->UEV disabled
				(0<<0) |				//0->counter disabled, 1->counter enabled
				0x00;
	TIM3->CR2 = 0;						//default value
	TIM3->SMCR= 0;						//default value - use internal clock

	//set the prescaler
	TIM3->PSC = TxCCP_PS - 1;					//set the prescaler
	TIM3->RCR = 0;						//repetition counter = 0 (=no repetition)
	TIM3->ARR = TxCCP_PR - 1;						//auto reload register / period = 0; - need to change for downcounters
	TIM3->CNT = 0;						//reset the counter

	//clear the status register bits for capture / compare flags
	TIM3->SR &=~(TIM_SR_CC1IF | TIM_SR_CC2IF | TIM_SR_CC3IF | TIM_SR_CC4IF | TIM_SR_UIF);
	//disable the interrupt by clearing the enable bits
	TIM3->DIER &=~(TIM_DIER_CC1IE | TIM_DIER_CC2IE | TIM_DIER_CC3IE | TIM_DIER_CC4IE | TIM_DIER_UIE);

	//configure CCP1..4
#if defined(TIM3CH1toGPIO)
	//configure CCP1
	TIM3->CCMR1 = 	(TIM3->CCMR1 &~0x00ff) |
					(0<<7) |			//0->OC1REF not affedted by ETRF, 1->OC1REF cleared by ETRF high
					(6<<4) |			//0->frozen (for time base), 1->active on match, 2->inactive on match, 3->toggle, 4->inactive, 5->active, 6->pwm mode 1, 7->pwm mode 2
					(0<<3) |			//0->preload disabled, 1->preload enabled
					(0<<2) |			//0->fast disabled, 1->fast enabled
					(0<<0) |			//0->ch1 as output, 1->ch1 as input, 2->ch1 as input, 3->ch1 as input
					0x00;
	TIM3->CCER = 	(TIM3->CCER &~(0x0f << 0)) |
					(0<< 3) |			//0->normal polarity for CC1N, 1->inverse polarity
					(0<< 2) |			//0->disable CC1N, 1->enable CC1N
					(0<< 1) |			//0->normal polarity for CC1, 1->inverse polarity
					(1<< 0) |			//1->enable CC1, 0->disable CC1
					0x00;
	TIM3->CCR1 = 0;						//0% duty cycle

	//configure the gpio for PWM output
	TIM3CH1toGPIO();
#endif

#if defined(TIM3CH2toGPIO)
	//configure CCP2
	TIM3->CCMR1 = 	(TIM3->CCMR1 &~0xff00) |
					(0<<15) |			//0->OC1REF not affedted by ETRF, 1->OC1REF cleared by ETRF high
					(6<<12) |			//0->frozen (for time base), 1->active on match, 2->inactive on match, 3->toggle, 4->inactive, 5->active, 6->pwm mode 1, 7->pwm mode 2
					(0<<11) |			//0->preload disabled, 1->preload enabled
					(0<<10) |			//0->fast disabled, 1->fast enabled
					(0<<8) |			//0->ch1 as output, 1->ch1 as input, 2->ch1 as input, 3->ch1 as input
					0x00;
	TIM3->CCER = 	(TIM3->CCER &~(0x0f << 4)) |
					(0<< 7) |			//0->normal polarity for CC2N, 1->inverse polarity
					(0<< 6) |			//0->disable CC2N, 1->enable CC2N
					(0<< 5) |			//0->normal polarity for CC2, 1->inverse polarity
					(1<< 4) |			//1->enable CC2, 0->disable CC2
					0x00;
	TIM3->CCR2 = 0;						//0% duty cycle

	//configure the gpio for PWM output
	TIM3CH2toGPIO();
#endif

#if defined(TIM3CH3toGPIO)
	//configure CCP3
	TIM3->CCMR2 = 	(TIM3->CCMR2 &~0x00ff) |
					(0<<7) |			//0->OC1REF not affedted by ETRF, 1->OC1REF cleared by ETRF high
					(6<<4) |			//0->frozen (for time base), 1->active on match, 2->inactive on match, 3->toggle, 4->inactive, 5->active, 6->pwm mode 1, 7->pwm mode 2
					(0<<3) |			//0->preload disabled, 1->preload enabled
					(0<<2) |			//0->fast disabled, 1->fast enabled
					(0<<0) |			//0->ch1 as output, 1->ch1 as input, 2->ch1 as input, 3->ch1 as input
					0x00;
	TIM3->CCER = 	(TIM3->CCER &~(0x0f << 8)) |
					(0<<11) |			//0->normal polarity for CC3N, 1->inverse polarity
					(0<<10) |			//0->disable CC3N, 1->enable CC3N
					(0<< 9) |			//0->normal polarity for CC3, 1->inverse polarity
					(1<< 8) |			//1->enable CC3, 0->disable CC3
					0x00;
	TIM3->CCR3 = 0;						//0% duty cycle

	//configure the gpio for PWM output
	TIM3CH3toGPIO();
#endif

#if defined(TIM3CH4toGPIO)
	//configure CCP4
	TIM3->CCMR2 = 	(TIM3->CCMR2 &~0xff00) |
					(0<<15) |			//0->OC1REF not affedted by ETRF, 1->OC1REF cleared by ETRF high
					(6<<12) |			//0->frozen (for time base), 1->active on match, 2->inactive on match, 3->toggle, 4->inactive, 5->active, 6->pwm mode 1, 7->pwm mode 2
					(0<<11) |			//0->preload disabled, 1->preload enabled
					(0<<10) |			//0->fast disabled, 1->fast enabled
					(0<<8) |			//0->ch1 as output, 1->ch1 as input, 2->ch1 as input, 3->ch1 as input
					0x00;
	TIM3->CCER = 	(TIM3->CCER &~(0x0f << 12)) |
					(0<<15) |			//0->normal polarity for CC4N, 1->inverse polarity
					(0<<14) |			//0->disable CC4N, 1->enable CC4N
					(0<<13) |			//0->normal polarity for CC4, 1->inverse polarity
					(1<<12) |			//1->enable CC4, 0->disable CC4
					0x00;
	TIM3->CCR4 = 0;						//0% duty cycle

	//configure the gpio for PWM output
	TIM3CH4toGPIO();
#endif

	TIM3->EGR = 0xff;					//force an update
	TIM3->BKDR|= TIM_BKDR_MOE;				//enable the main output
	//enable the timer.
	TIM3->CR1 |= TIM_CR1_CEN;			//enable the timer

}


//initialize pwm to TxCCP_PS (prescaler) and TxCCP_PR (period)
void pwm14_init(uint16_t TxCCP_PS, uint16_t TxCCP_PR) {
	//route the clock to timer
	RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;

	//source from internal clock -> disable slave mode
	//TIM14->SMCR &=~TIM_SMCR_SMS;			//clear sms->use internal clock

	//stop the timer to configure it
	//TIM14->CR1 &=~TIM_CR1_CEN;			//clear cen. 0=disable the timer, 1=enable the timer
	//TIM14->CR1 &=~TIM_CR1_CKD;			//clear CKD0..1. 0b00->1x clock; 0b01->2:1 clock, 0b10->4:1 clk; 0b11->reserved
	//TIM14->CR1 &=~TIM_CR1_DIR;			//clear DIR bit. 0=upcounter, 1=downcounter
	//TIM14->CR1 &=~TIM_CR1_OPM;			//clear opm bit. 0=periodic timer, 1=one-shot timer
	//or to simply zero the register
	//TIM14->CR1 = 0;						//much easier
	TIM14->CR1 =	(0<<8) |				//0->1:1 clock, 1->2:1 clock, 2->4:1 clock, 3->reserved
				(0<<7) |				//1->APR buffered, 0->APR not buffered
				(0<<5) |				//0->edge-aligned, 1->center-aligned mode 1, 2->center-aligned mode 2, 3->center-aligned mode 3
				(0<<4) |				//0->upcounter, 1->downcounter
				(0<<3) |				//0->continous mode, 1->one pulse mode
				(0<<2) |				//update request source
				(0<<1) |				//0->UEV enabled, 1->UEV disabled
				(0<<0) |				//0->counter disabled, 1->counter enabled
				0x00;
	TIM14->CR2 = 0;						//default value
	TIM14->SMCR= 0;						//default value - use internal clock

	//set the prescaler
	TIM14->PSC = TxCCP_PS - 1;					//set the prescaler
	TIM14->RCR = 0;						//repetition counter = 0 (=no repetition)
	TIM14->ARR = TxCCP_PR - 1;						//auto reload register / period = 0; - need to change for downcounters
	TIM14->CNT = 0;						//reset the counter

	//clear the status register bits for capture / compare flags
	TIM14->SR &=~(TIM_SR_CC1IF | TIM_SR_CC2IF | TIM_SR_CC3IF | TIM_SR_CC4IF | TIM_SR_UIF);
	//disable the interrupt by clearing the enable bits
	TIM14->DIER &=~(TIM_DIER_CC1IE | TIM_DIER_CC2IE | TIM_DIER_CC3IE | TIM_DIER_CC4IE | TIM_DIER_UIE);

	//configure CCP1..4
#if defined(TIM14CH1toGPIO)
	//configure CCP1
	TIM14->CCMR1 = 	(TIM14->CCMR1 &~0x00ff) |
					(0<<7) |			//0->OC1REF not affedted by ETRF, 1->OC1REF cleared by ETRF high
					(6<<4) |			//0->frozen (for time base), 1->active on match, 2->inactive on match, 3->toggle, 4->inactive, 5->active, 6->pwm mode 1, 7->pwm mode 2
					(0<<3) |			//0->preload disabled, 1->preload enabled
					(0<<2) |			//0->fast disabled, 1->fast enabled
					(0<<0) |			//0->ch1 as output, 1->ch1 as input, 2->ch1 as input, 3->ch1 as input
					0x00;
	TIM14->CCER = 	(TIM14->CCER &~(0x0f << 0)) |
					(0<< 3) |			//0->normal polarity for CC1N, 1->inverse polarity
					(0<< 2) |			//0->disable CC1N, 1->enable CC1N
					(0<< 1) |			//0->normal polarity for CC1, 1->inverse polarity
					(1<< 0) |			//1->enable CC1, 0->disable CC1
					0x00;
	TIM14->CCR1 = 0;						//0% duty cycle

	//configure the gpio for PWM output
	TIM14CH1toGPIO();
#endif

	TIM14->EGR = 0xff;					//force an update
	TIM14->BKDR|= TIM_BKDR_MOE;				//enable the main output
	//enable the timer.
	TIM14->CR1 |= TIM_CR1_CEN;			//enable the timer

}


//initialize pwm to TxCCP_PS (prescaler) and TxCCP_PR (period)
void pwm15_init(uint16_t TxCCP_PS, uint16_t TxCCP_PR) {
	//route the clock to timer
	RCC->APB2ENR |= RCC_APB2ENR_TIM15EN;

	//source from internal clock -> disable slave mode
	//TIM15->SMCR &=~TIM_SMCR_SMS;			//clear sms->use internal clock

	//stop the timer to configure it
	//TIM15->CR1 &=~TIM_CR1_CEN;			//clear cen. 0=disable the timer, 1=enable the timer
	//TIM15->CR1 &=~TIM_CR1_CKD;			//clear CKD0..1. 0b00->1x clock; 0b01->2:1 clock, 0b10->4:1 clk; 0b11->reserved
	//TIM15->CR1 &=~TIM_CR1_DIR;			//clear DIR bit. 0=upcounter, 1=downcounter
	//TIM15->CR1 &=~TIM_CR1_OPM;			//clear opm bit. 0=periodic timer, 1=one-shot timer
	//or to simply zero the register
	//TIM15->CR1 = 0;						//much easier
	TIM15->CR1 =	(0<<8) |				//0->1:1 clock, 1->2:1 clock, 2->4:1 clock, 3->reserved
				(0<<7) |				//1->APR buffered, 0->APR not buffered
				(0<<5) |				//0->edge-aligned, 1->center-aligned mode 1, 2->center-aligned mode 2, 3->center-aligned mode 3
				(0<<4) |				//0->upcounter, 1->downcounter
				(0<<3) |				//0->continous mode, 1->one pulse mode
				(0<<2) |				//update request source
				(0<<1) |				//0->UEV enabled, 1->UEV disabled
				(0<<0) |				//0->counter disabled, 1->counter enabled
				0x00;
	TIM15->CR2 = 0;						//default value
	TIM15->SMCR= 0;						//default value - use internal clock

	//set the prescaler
	TIM15->PSC = TxCCP_PS - 1;					//set the prescaler
	TIM15->RCR = 0;						//repetition counter = 0 (=no repetition)
	TIM15->ARR = TxCCP_PR - 1;						//auto reload register / period = 0; - need to change for downcounters
	TIM15->CNT = 0;						//reset the counter

	//clear the status register bits for capture / compare flags
	TIM15->SR &=~(TIM_SR_CC1IF | TIM_SR_CC2IF | TIM_SR_CC3IF | TIM_SR_CC4IF | TIM_SR_UIF);
	//disable the interrupt by clearing the enable bits
	TIM15->DIER &=~(TIM_DIER_CC1IE | TIM_DIER_CC2IE | TIM_DIER_CC3IE | TIM_DIER_CC4IE | TIM_DIER_UIE);

	//configure CCP1..4
#if defined(TIM15CH1toGPIO)
	//configure CCP1
	TIM15->CCMR1 = 	(TIM15->CCMR1 &~0x00ff) |
					(0<<7) |			//0->OC1REF not affedted by ETRF, 1->OC1REF cleared by ETRF high
					(6<<4) |			//0->frozen (for time base), 1->active on match, 2->inactive on match, 3->toggle, 4->inactive, 5->active, 6->pwm mode 1, 7->pwm mode 2
					(0<<3) |			//0->preload disabled, 1->preload enabled
					(0<<2) |			//0->fast disabled, 1->fast enabled
					(0<<0) |			//0->ch1 as output, 1->ch1 as input, 2->ch1 as input, 3->ch1 as input
					0x00;
	TIM15->CCER = 	(TIM15->CCER &~(0x0f << 0)) |
					(0<< 3) |			//0->normal polarity for CC1N, 1->inverse polarity
					(0<< 2) |			//0->disable CC1N, 1->enable CC1N
					(0<< 1) |			//0->normal polarity for CC1, 1->inverse polarity
					(1<< 0) |			//1->enable CC1, 0->disable CC1
					0x00;
	TIM15->CCR1 = 0;						//0% duty cycle

	//configure the gpio for PWM output
	TIM15CH1toGPIO();
#endif

#if defined(TIM15CH2toGPIO)
	//configure CCP2
	TIM15->CCMR1 = 	(TIM15->CCMR1 &~0xff00) |
					(0<<15) |			//0->OC1REF not affedted by ETRF, 1->OC1REF cleared by ETRF high
					(6<<12) |			//0->frozen (for time base), 1->active on match, 2->inactive on match, 3->toggle, 4->inactive, 5->active, 6->pwm mode 1, 7->pwm mode 2
					(0<<11) |			//0->preload disabled, 1->preload enabled
					(0<<10) |			//0->fast disabled, 1->fast enabled
					(0<<8) |			//0->ch1 as output, 1->ch1 as input, 2->ch1 as input, 3->ch1 as input
					0x00;
	TIM15->CCER = 	(TIM15->CCER &~(0x0f << 4)) |
					(0<< 7) |			//0->normal polarity for CC2N, 1->inverse polarity
					(0<< 6) |			//0->disable CC2N, 1->enable CC2N
					(0<< 5) |			//0->normal polarity for CC2, 1->inverse polarity
					(1<< 4) |			//1->enable CC2, 0->disable CC2
					0x00;
	TIM15->CCR2 = 0;						//0% duty cycle

	//configure the gpio for PWM output
	TIM15CH2toGPIO();
#endif

	TIM15->EGR = 0xff;					//force an update
	TIM15->BKDR|= TIM_BKDR_MOE;				//enable the main output
	//enable the timer.
	TIM15->CR1 |= TIM_CR1_CEN;			//enable the timer

}

/*
MAPR2 Bit 1 TIM16_REMAP: TIM16 remapping
This bit is set and cleared by software. It controls the mapping of the alternate functions of TIM16 channel 1 onto the GPIO ports.
0: No remap (CH1/PB8)
1: Remap (CH1/PA6)
*/
//initialize pwm to TxCCP_PS (prescaler) and TxCCP_PR (period)
void pwm16_init(uint16_t TxCCP_PS, uint16_t TxCCP_PR) {
	//route the clock to timer
	RCC->APB2ENR |= RCC_APB2ENR_TIM16EN;

	//source from internal clock -> disable slave mode
	//TIM16->SMCR &=~TIM_SMCR_SMS;			//clear sms->use internal clock

	//stop the timer to configure it
	//TIM16->CR1 &=~TIM_CR1_CEN;			//clear cen. 0=disable the timer, 1=enable the timer
	//TIM16->CR1 &=~TIM_CR1_CKD;			//clear CKD0..1. 0b00->1x clock; 0b01->2:1 clock, 0b10->4:1 clk; 0b11->reserved
	//TIM16->CR1 &=~TIM_CR1_DIR;			//clear DIR bit. 0=upcounter, 1=downcounter
	//TIM16->CR1 &=~TIM_CR1_OPM;			//clear opm bit. 0=periodic timer, 1=one-shot timer
	//or to simply zero the register
	//TIM16->CR1 = 0;						//much easier
	TIM16->CR1 =	(0<<8) |				//0->1:1 clock, 1->2:1 clock, 2->4:1 clock, 3->reserved
				(0<<7) |				//1->APR buffered, 0->APR not buffered
				(0<<5) |				//0->edge-aligned, 1->center-aligned mode 1, 2->center-aligned mode 2, 3->center-aligned mode 3
				(0<<4) |				//0->upcounter, 1->downcounter
				(0<<3) |				//0->continous mode, 1->one pulse mode
				(0<<2) |				//update request source
				(0<<1) |				//0->UEV enabled, 1->UEV disabled
				(0<<0) |				//0->counter disabled, 1->counter enabled
				0x00;
	TIM16->CR2 = 0;						//default value
	TIM16->SMCR= 0;						//default value - use internal clock

	//set the prescaler
	TIM16->PSC = TxCCP_PS - 1;					//set the prescaler
	TIM16->RCR = 0;						//repetition counter = 0 (=no repetition)
	TIM16->ARR = TxCCP_PR - 1;						//auto reload register / period = 0; - need to change for downcounters
	TIM16->CNT = 0;						//reset the counter

	//clear the status register bits for capture / compare flags
	TIM16->SR &=~(TIM_SR_CC1IF | TIM_SR_CC2IF | TIM_SR_CC3IF | TIM_SR_CC4IF | TIM_SR_UIF);
	//disable the interrupt by clearing the enable bits
	TIM16->DIER &=~(TIM_DIER_CC1IE | TIM_DIER_CC2IE | TIM_DIER_CC3IE | TIM_DIER_CC4IE | TIM_DIER_UIE);

	//configure CCP1..4
#if defined(TIM16CH1toGPIO)
	//configure CCP1
	TIM16->CCMR1 = 	(TIM16->CCMR1 &~0x00ff) |
					(0<<7) |			//0->OC1REF not affedted by ETRF, 1->OC1REF cleared by ETRF high
					(6<<4) |			//0->frozen (for time base), 1->active on match, 2->inactive on match, 3->toggle, 4->inactive, 5->active, 6->pwm mode 1, 7->pwm mode 2
					(0<<3) |			//0->preload disabled, 1->preload enabled
					(0<<2) |			//0->fast disabled, 1->fast enabled
					(0<<0) |			//0->ch1 as output, 1->ch1 as input, 2->ch1 as input, 3->ch1 as input
					0x00;
	TIM16->CCER = 	(TIM16->CCER &~(0x0f << 0)) |
					(0<< 3) |			//0->normal polarity for CC1N, 1->inverse polarity
					(0<< 2) |			//0->disable CC1N, 1->enable CC1N
					(0<< 1) |			//0->normal polarity for CC1, 1->inverse polarity
					(1<< 0) |			//1->enable CC1, 0->disable CC1
					0x00;
	TIM16->CCR1 = 0;						//0% duty cycle

	//configure the gpio for PWM output
	TIM16CH1toGPIO();
#endif

	TIM16->EGR = 0xff;					//force an update
	TIM16->BKDR|= TIM_BKDR_MOE;				//enable the main output
	//enable the timer.
	TIM16->CR1 |= TIM_CR1_CEN;			//enable the timer

}

/*
MAPR2 Bit 2 TIM17_REMAP: TIM17 remapping
This bit is set and cleared by software. It controls the mapping of the alternate functions of TIM17 channel 1 onto the GPIO ports.
0: No remap (CH1/PB9)
1: Remap (CH1/PA7)
*/
//initialize pwm to TxCCP_PS (prescaler) and TxCCP_PR (period)
void pwm17_init(uint16_t TxCCP_PS, uint16_t TxCCP_PR) {
	//route the clock to timer
	RCC->APB2ENR |= RCC_APB2ENR_TIM17EN;

	//source from internal clock -> disable slave mode
	//TIM17->SMCR &=~TIM_SMCR_SMS;			//clear sms->use internal clock

	//stop the timer to configure it
	//TIM17->CR1 &=~TIM_CR1_CEN;			//clear cen. 0=disable the timer, 1=enable the timer
	//TIM17->CR1 &=~TIM_CR1_CKD;			//clear CKD0..1. 0b00->1x clock; 0b01->2:1 clock, 0b10->4:1 clk; 0b11->reserved
	//TIM17->CR1 &=~TIM_CR1_DIR;			//clear DIR bit. 0=upcounter, 1=downcounter
	//TIM17->CR1 &=~TIM_CR1_OPM;			//clear opm bit. 0=periodic timer, 1=one-shot timer
	//or to simply zero the register
	//TIM17->CR1 = 0;						//much easier
	TIM17->CR1 =	(0<<8) |				//0->1:1 clock, 1->2:1 clock, 2->4:1 clock, 3->reserved
				(0<<7) |				//1->APR buffered, 0->APR not buffered
				(0<<5) |				//0->edge-aligned, 1->center-aligned mode 1, 2->center-aligned mode 2, 3->center-aligned mode 3
				(0<<4) |				//0->upcounter, 1->downcounter
				(0<<3) |				//0->continous mode, 1->one pulse mode
				(0<<2) |				//update request source
				(0<<1) |				//0->UEV enabled, 1->UEV disabled
				(0<<0) |				//0->counter disabled, 1->counter enabled
				0x00;
	TIM17->CR2 = 0;						//default value
	TIM17->SMCR= 0;						//default value - use internal clock

	//set the prescaler
	TIM17->PSC = TxCCP_PS - 1;					//set the prescaler
	TIM17->RCR = 0;						//repetition counter = 0 (=no repetition)
	TIM17->ARR = TxCCP_PR - 1;						//auto reload register / period = 0; - need to change for downcounters
	TIM17->CNT = 0;						//reset the counter

	//clear the status register bits for capture / compare flags
	TIM17->SR &=~(TIM_SR_CC1IF | TIM_SR_CC2IF | TIM_SR_CC3IF | TIM_SR_CC4IF | TIM_SR_UIF);
	//disable the interrupt by clearing the enable bits
	TIM17->DIER &=~(TIM_DIER_CC1IE | TIM_DIER_CC2IE | TIM_DIER_CC3IE | TIM_DIER_CC4IE | TIM_DIER_UIE);

	//configure CCP1..4
#if defined(TIM17CH1toGPIO)
	//configure CCP1
	TIM17->CCMR1 = 	(TIM17->CCMR1 &~0x00ff) |
					(0<<7) |			//0->OC1REF not affedted by ETRF, 1->OC1REF cleared by ETRF high
					(6<<4) |			//0->frozen (for time base), 1->active on match, 2->inactive on match, 3->toggle, 4->inactive, 5->active, 6->pwm mode 1, 7->pwm mode 2
					(0<<3) |			//0->preload disabled, 1->preload enabled
					(0<<2) |			//0->fast disabled, 1->fast enabled
					(0<<0) |			//0->ch1 as output, 1->ch1 as input, 2->ch1 as input, 3->ch1 as input
					0x00;
	TIM17->CCER = 	(TIM17->CCER &~(0x0f << 0)) |
					(0<< 3) |			//0->normal polarity for CC1N, 1->inverse polarity
					(0<< 2) |			//0->disable CC1N, 1->enable CC1N
					(0<< 1) |			//0->normal polarity for CC1, 1->inverse polarity
					(1<< 0) |			//1->enable CC1, 0->disable CC1
					0x00;
	TIM17->CCR1 = 0;						//0% duty cycle

	//configure the gpio for PWM output
	TIM17CH1toGPIO();
#endif
	TIM17->EGR = 0xff;					//force an update
	TIM17->BKDR|= TIM_BKDR_MOE;				//enable the main output
	//enable the timer.
	TIM17->CR1 |= TIM_CR1_CEN;			//enable the timer

}
