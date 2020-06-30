#include "uart2.h"				//uart2 transmission using interrupt

//global defines
#define UARTx				UART2
//#define UARTx_IRQn			UART2_IRQn

//global variables
//static volatile char *_UxTX_ptr;
//static volatile char _UxTX_BUSY=0;		//0=u1 transmission done, 1=u1 transmission in process

//initialize uart
//high speed (HSE set)
//default: tx enabled(CTL bit 8 set), rx disabled -> CTL bit 9 cleared
void uart2_init(uint32_t baudrate) {
	//uart2: tx on PD7, rx on PD6
	//route clock to GPIO
	SYSCTL->RCGCGPIO |= (1<<3);				//0->GPIOA, 1->GPIOB, 2->GPIOC, ...
	while ((SYSCTL->PRGPIO & (1<<3)) == 0) continue;	//1->peripheral ready, 0-> not ready
#if defined(UART2_RX)
	//configure PD6 for alternate function / mux group 1
	UART2_RX();								//GPIOA->DEN |= (1<<0); GPIOA->AFSEL |= (1<<0); GPIOA->PCTL = (GPIOA->PCTL &~(0x0f << (0*4))) | (0x01 << (0*4));
#endif
#if defined(UART2_TX)
	//configure PD7 for alternate function / mux group 1
	UART2_TX();								//GPIOA->DEN |= (1<<1); GPIOA->AFSEL |= (1<<1); GPIOA->PCTL = (GPIOA->PCTL &~(0x0f << (1*4))) | (0x01 << (1*4));
#endif
	
	//route clock to UART
	SYSCTL->RCGCUART |= (1<<2);				//0->UART0, 1->UART1, ...
	while ((SYSCTL->PRUART & (1<<2)) == 0) continue;	//1->peripheral ready, 0-> not ready

	//stop the uart
	UARTx->CTL &=~(1<<0);					//1->enable uart, 0->disable uart

	//set HSE for high speed
	UARTx->CTL =	(0<<15) |				//0->clear to send disabled, 1->clear to send enabled
					(0<<14) |				//0->request to send disabled, 1->request to send enabled
					(0<<11) |				//1->RTS signal received, 0->RTS not received
#if defined(UART2_RX)
					(1<< 9) |				//1->enable uart receive, 0->disable uart receive
#else
					(0<< 9) |				//1->enable uart receive, 0->disable uart receive
#endif
#if defined(UART2_TX)
					(1<< 8) |				//1->enable uart transmit, 0->disable uart transmit
#else
					(0<< 8) |				//1->enable uart transmit, 0->disable uart transmit
#endif
					(0<< 7) |				//0->normal operation, 1->TX/RX loop back enabled
					(1<< 5) |				//0->HSE disabled, divide by 16, 1->HSE enabled, divide by 8
					(0<< 4) |				//0->set TXRIS when UARTIFLS is met, 1->TXRIS set only after all transmitted data have been sent
					(0<< 3) | 				//0->normal operation, 1->UART in smart card mode
					(0<< 2) |				//0->low level bits transmitted as active high, 1->uart in SIR low-power mode
					(0<< 1) |				//0->normal operation, 1->IrDA SIR enabled
					(0<< 0) |				//0->uart is disabled, 1->uart is enabled
					0x00;
	//if (GPIOA->AFSEL & (1<<0)) UART0->CTL |= (1<<9); else UART0->CTL &=~(1<<9);	//enable receiving if PA0 is configured for alternate function
	//if (GPIOA->AFSEL & (1<<1)) UART0->CTL |= (1<<8); else UART0->CTL &=~(1<<8);	//enable transmission if PA1 is configured for alternate function

	if (UARTx->CTL & (1<<5)) {				//HSE bit set -> divide by 8
		//calculate the integer baud rate
		UARTx->IBRD = F_UART / baudrate / 8;
		//calculate the fractional baud rate = 64*fraction
		UARTx->FBRD = (8 * F_UART / baudrate - UART0->IBRD * 64);
	} else {								//HSE bit cleared -> divide by 16
		//calculate the integer baud rate
		UARTx->IBRD = F_UART / baudrate / 16;
		//calculate the fractional baud rate = 64*fraction
		UARTx->FBRD = (4 * F_UART / baudrate - UART0->IBRD * 64);
	}

	//set fifo levels
	UARTx->IFLS	=	(2<<3) |				//RX fifo: 2=>=1/2 full (default)
					(2<<0) |				//TX fifo: 2=<=1/2 full (default)
					0x00;
	//configure serial parameters in LCRH
	UARTx->LCRH = 	(0<<7) |				//0->stick parity disabled, 1->stick parity enabled
					(3<<5) |				//word length. 0->5bit, 1->6bit, 2->7bit, 3->8bit
					(1<<4) |				//0->fifo disabled, 1->fifo enabled
					(0<<3) |				//0->1 stop bit, 1->two stop bits
					(0<<2) |				//0->odd parity, 1->even parity. no effect if parity is disabled (bit 1 cleared)
					(0<<1) |				//0->parity disabled, 1->parity enabled
					(0<<0) | 				//0->normal use, 1->send break
					0x00;

	//configure the uart clock source
	UARTx->CC = 0x00;						//0x00->system clock used, 0x05->PIOSC used

	//clear the flags
	UARTx->IM &=~(1<<5);					//all interrupt disabled
	UARTx->ICR|= (1<<5);					//1->clear flags

	//enable NVIC
	//NVIC_EnableIRQ(UARTx_IRQn);
	
	//start the uart
	UARTx->CTL |= (1<<0);					//1->enable uart, 0->disable uart
}

//send a char
//need to check if uart is busy first
void uart2_put(char ch) {
	while ((UARTx->FR & (1<<5))) continue;	//if buffer is full (=no space in fifo), exit the loop: 0->there is space, 1->tx buffer is full
	UARTx->DR = ch;
}

//send a string
void uart2_puts(char * str) {
	while (*str) uart2_put(*str++);
}

//read usartx status
//1 if usartx is busy
//0 if usartx is available for new transmission
char uart2_busy(void) {
	//return UARTx->FR & (1<<3);		//1->busy, 0->not busy, most conservative
	return UARTx->FR & (1<<5);		//1->tx buffer full, 0->tx buffer not full
	//return ~(UARTx->FR & (1<<7));		//1->tx buffer empty, 0->tx buffer not empty
	//return UARTBusy(UART0_BASE);
}

//1=rx data available
char uart2_available(void) {
	return (UARTx->FR & (1<<4))?0:1;	//1->receiver is empty, 0->receiver has data
}

