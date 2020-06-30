#ifndef _GPIO_H
#define _GPIO_H

//using just device header file, no tiva lib support

//#include "tm4c1233h6pm.h"		//for tivaware = lm4f120h5
#include "tm4e129.h"			//the same as lm4e129

//port macros
#define IO_SET(port, pins)		port->DATA |= (pins)					//MAP_GPIOPinWrite(port, (pins), (pins))
#define IO_CLR(port, pins)		port->DATA &=~(pins)					//MAP_GPIOPinWrite(port, (pins), (0))
#define IO_FLP(port, pins)		port->DATA ^= (pins)					//MAP_GPIOPinWrite(port, (pins), ~IO_GET(port, pins))
#define IO_GET(port, pins)		((port->DATA)&(pins))					//MAP_GPIOPinRead(port, pins)

//set pins as floating input
#define IO_IN(port, pins)		do {port->DEN |= (pins); port->DIR &=~(pins);} while (0)						//IO_INFL(port, pins)
#define IO_INFL(port, pins)		do {port->DEN |= (pins); port->PUR &=~(pins); port->PDR &=~(pins); port->DIR &=~(pins);} while (0)	//IO_IN(port, pins)						//MAP_GPIOPinTypeGPIOInput(port, pins)
#define IO_INPU(port, pins)		do {port->DEN |= (pins); port->PUR |= (pins); port->PDR &=~(pins); port->DIR &=~(pins);} while (0)	//IO_IN(port, pins)						//do {IO_IN(port, pins); HWREG((port) + (GPIO_O_PUR)) |= (pins);} while (0)
#define IO_INPD(port, pins)		do {port->DEN |= (pins); port->PUR &=~(pins); port->PDR |= (pins); port->DIR &=~(pins);} while (0)	//IO_IN(port, pins)						//do {IO_IN(port, pins); HWREG((port) + (GPIO_O_PDR)) |= (pins);} while (0)
//set pins to output - clock assumed to be already pumped to the periopherals
#define IO_OUT(port, pins)		do {port->DEN |= (pins); port->ODR &=~(pins); port->DIR |= (pins);} while (0)						//IO_OUTPP(port, pins)
#define IO_OUTPP(port, pins)	IO_OUT(port, pins)
//set pins as output OD
#define IO_OUTOD(port, pins)	do {port->DEN |= (pins); port->PUR &=~(pins); port->PDR &=~(pins); port->ODR |= (pins); port->DIR |= (pins);} while (0)		//IO_OUT(port, pins)						//do {IO_OUT(port, pins); HWREG((port) + (GPIO_O_ODR)) |= (pins);} while (0)
#define IO_OUTPU(port, pins)	do {port->DEN |= (pins); port->PUR |= (pins); port->PDR &=~(pins); port->ODR |= (pins); port->DIR |= (pins);} while (0)		//IO_OUT(port, pins)						//do {IO_OUT(port, pins); HWREG((port) + (GPIO_O_PUR)) |= (pins);} while (0)
#define IO_OUTPD(port, pins)	do {port->DEN |= (pins); port->PUR &=~(pins); port->PDR |= (pins); port->ODR |= (pins); port->DIR |= (pins);} while (0)		//IO_OUT(port, pins)						//do {IO_OUT(port, pins); HWREG((port) + (GPIO_O_PDR)) |= (pins);} while (0)

//using the bit-banding area - first 1k address
#define FIO_SET(port, pins)		port->RESERVED0[pins] = (pins)
#define FIO_CLR(port, pins)		port->RESERVED0[pins] =&(pins)
#define FIO_FLP(port, pins)		IO_FLP(port, pins)
#define FIO_GET(port, pins)		(port->RESERVED0[pins] & (pins))

//GPIO pins

//specific to STM32VLDiscovery
//led definitions
#define LED_PORT			GPIOF
#define LED_DDR				GPIOF
#define LEDG				GPIO_PIN_3			//green led on pf3
#define LEDB				GPIO_PIN_2			//blue led on pf2
#define LEDR				GPIO_PIN_1			//blue led on pf1
#define LEDs				(LEDR | LEDB | LEDG)	//all three leds

//close sources
//HSI/PIOSC
#define SystemCoreClockHSI()		MAP_SysCtlClockSet(SYSCTL_SYSDIV_1  | SYSCTL_USE_OSC | SYSCTL_OSC_INT);	//use PIOSC/HSI=16Mhz
#define SystemCoreClockHSI_1Mhz()	MAP_SysCtlClockSet(SYSCTL_SYSDIV_16 | SYSCTL_USE_OSC | SYSCTL_OSC_INT);	//use PIOSC/HSI=8Mhz
#define SystemCoreClockHSI_2Mhz()	MAP_SysCtlClockSet(SYSCTL_SYSDIV_8  | SYSCTL_USE_OSC | SYSCTL_OSC_INT);	//use PIOSC/HSI=8Mhz
#define SystemCoreClockHSI_4Mhz()	MAP_SysCtlClockSet(SYSCTL_SYSDIV_4  | SYSCTL_USE_OSC | SYSCTL_OSC_INT);	//use PIOSC/HSI=8Mhz
#define SystemCoreClockHSI_8Mhz()	MAP_SysCtlClockSet(SYSCTL_SYSDIV_2  | SYSCTL_USE_OSC | SYSCTL_OSC_INT);	//use PIOSC/HSI=8Mhz
#define SystemCoreClockHSI_16Mhz()	MAP_SysCtlClockSet(SYSCTL_SYSDIV_1  | SYSCTL_USE_OSC | SYSCTL_OSC_INT);	//use PIOSC/HSI=16Mhz
#define SystemCoreClockHSI_10Mhz()	MAP_SysCtlClockSet(SYSCTL_SYSDIV_20 | SYSCTL_USE_PLL | SYSCTL_OSC_INT);	//use PIOSC/HSI=20Mhz under PLL
#define SystemCoreClockHSI_20Mhz()	MAP_SysCtlClockSet(SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_INT);	//use PIOSC/HSI=20Mhz under PLL
#define SystemCoreClockHSI_25Mhz()	MAP_SysCtlClockSet(SYSCTL_SYSDIV_8  | SYSCTL_USE_PLL | SYSCTL_OSC_INT);	//use PIOSC/HSI=20Mhz under PLL
#define SystemCoreClockHSI_40Mhz()	MAP_SysCtlClockSet(SYSCTL_SYSDIV_5  | SYSCTL_USE_PLL | SYSCTL_OSC_INT);	//use PIOSC/HSI=20Mhz under PLL
#define SystemCoreClockHSI_50Mhz()	MAP_SysCtlClockSet(SYSCTL_SYSDIV_4  | SYSCTL_USE_PLL | SYSCTL_OSC_INT);	//use PIOSC/HSI=20Mhz under PLL
#define SystemCoreClockHSI_80Mhz()	MAP_SysCtlClockSet(SYSCTL_SYSDIV_2_5| SYSCTL_USE_PLL | SYSCTL_OSC_INT);	//use PIOSC/HSI=20Mhz under PLL
#define SystemCoreClockHSI_100Mhz()	MAP_SysCtlClockSet(SYSCTL_SYSDIV_2  | SYSCTL_USE_PLL | SYSCTL_OSC_INT);	//use PIOSC/HSI=20Mhz under PLL
#define SystemCoreClockHSI_200Mhz()	MAP_SysCtlClockSet(SYSCTL_SYSDIV_1  | SYSCTL_USE_PLL | SYSCTL_OSC_INT);	//use PIOSC/HSI=20Mhz under PLL
//HSE/external crystal - for 16Mhz only
#define SystemCoreClockHSE()		MAP_SysCtlClockSet(SYSCTL_SYSDIV_1  | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);	//use PIOSC/HSI=16Mhz, no PLL
#define SystemCoreClockHSE_1Mhz()	MAP_SysCtlClockSet(SYSCTL_SYSDIV_16 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);	//use PIOSC/HSI=16Mhz, no PLL
#define SystemCoreClockHSE_2Mhz()	MAP_SysCtlClockSet(SYSCTL_SYSDIV_8  | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);	//use PIOSC/HSI=16Mhz, no PLL
#define SystemCoreClockHSE_4Mhz()	MAP_SysCtlClockSet(SYSCTL_SYSDIV_4  | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);	//use PIOSC/HSI=16Mhz, no PLL
#define SystemCoreClockHSE_8Mhz()	MAP_SysCtlClockSet(SYSCTL_SYSDIV_2  | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);	//use PIOSC/HSI=16Mhz, no PLL
#define SystemCoreClockHSE_16Mhz()	MAP_SysCtlClockSet(SYSCTL_SYSDIV_1  | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);	//use PIOSC/HSI=16Mhz, no PLL
#define SystemCoreClockHSE_10Mhz()	MAP_SysCtlClockSet(SYSCTL_SYSDIV_20 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);	//use PIOSC/HSI=16Mhz, use PLL
#define SystemCoreClockHSE_20Mhz()	MAP_SysCtlClockSet(SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);	//use PIOSC/HSI=16Mhz, use PLL
#define SystemCoreClockHSE_25Mhz()	MAP_SysCtlClockSet(SYSCTL_SYSDIV_8  | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);	//use PIOSC/HSI=16Mhz, use PLL
#define SystemCoreClockHSE_40Mhz()	MAP_SysCtlClockSet(SYSCTL_SYSDIV_5  | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);	//use PIOSC/HSI=16Mhz, use PLL
#define SystemCoreClockHSE_50Mhz()	MAP_SysCtlClockSet(SYSCTL_SYSDIV_4  | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);	//use PIOSC/HSI=16Mhz, use PLL
#define SystemCoreClockHSE_80Mhz()	MAP_SysCtlClockSet(SYSCTL_SYSDIV_2_5| SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);	//use PIOSC/HSI=16Mhz, use PLL
#define SystemCoreClockHSE_100Mhz()	MAP_SysCtlClockSet(SYSCTL_SYSDIV_2  | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);	//use PIOSC/HSI=16Mhz, use PLL
#define SystemCoreClockHSE_200Mhz()	MAP_SysCtlClockSet(SYSCTL_SYSDIV_1  | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);	//use PIOSC/HSI=16Mhz, use PLL

//reset the mcu
void mcu_init(void);

//nop
#define NOP()				__nop()						//asm(" nop")				//__asm("NOP")
#define NOP2()				{NOP(); NOP();}
#define NOP4()				{NOP2(); NOP2();}
#define NOP8()				{NOP4(); NOP4();}
#define NOP16()				{NOP8(); NOP8();}
#define NOP24()				{NOP16(); NOP8();}
#define NOP32()				{NOP16(); NOP16();}
#define NOP40()				{NOP32(); NOP8();}
#define NOP64()				{NOP32(); NOP32();}

//f_cpu speed
//#define SystemCoreClock		40000000ul					//cpu runs at 2Mhz -> Fxtal = 2Mhz
#define F_CPU				SystemCoreClock				//(SysCtlClockGet()/*40000000ul*/)					//cpu runs at 2Mhz -> Fxtal = 2Mhz
//#define SystemCoreClockUpdate()	SysCtlClockGet()				//for compatibility

//void (*mcu_reset)(void) = 0x0000; 							//jump to 0x0000 -> software reset

#ifndef ei
#define ei()				__enable_irq()
#endif

#ifndef di
#define di()				__disable_irq()
#endif

//DID0/DID1
#define DID0				SYSCTL_DID0_R			//DID0
#define DID1				SYSCTL_DID1_R			//DID1

//simple multiples
#define x1(val)				(val)								//multiply val by 1
#define x2(val)				(((val) << 1))						//multiply val by 2
#define x3(val)				(x2(val) + (val))					//multiply val by 3
#define x4(val)				(((val) << 2))						//multiply val by 4
#define x5(val)				(x4(val) + (val))					//multiply val by 5
#define x6(val)				(x4(val) + x2(val))					//multiply val by 6
#define x7(val)				(x6(val) + (val))					//multiply val by 7
#define x8(val)				((val) << 3)						//multiply val by 8
#define x9(val)				(x8(val) + (val))					//multiply val by 9

//multiples of 10s
#define x10(val)			(x8(val) + x2(val))					//multiply val by 10
#define x100(val)			(x10(x10(val)))						//multiply val by 100
#define x1000(val)			(x100(x10(val)))					//multiply val by 1000
#define x1k(val)			x1000(val)							//multiply val by 1000
#define x10k(val)			(x100(x100(val)))					//multiply val by 10000

#define x20(val)			(x2(x10(val)))
#define x30(val)			(x3(x10(val)))
#define x40(val)			(x4(x10(val)))
#define x50(val)			(x5(x10(val)))
#define x60(val)			(x6(x10(val)))
#define x70(val)			(x7(x10(val)))
#define x80(val)			(x8(x10(val)))
#define x90(val)			(x9(x10(val)))

//multiples of 100s
#define x200(val)			(x2(x100(val)))
#define x300(val)			(x3(x100(val)))
#define x400(val)			(x4(x100(val)))
#define x500(val)			(x5(x100(val)))
#define x600(val)			(x6(x100(val)))
#define x700(val)			(x7(x100(val)))
#define x800(val)			(x8(x100(val)))
#define x900(val)			(x9(x100(val)))

//custom definitions
#define x34(val)			(x30(val) + x4(val))				//multiply val by 34
#define x97(val)			(x90(val) + x7(val))				//multiply val by 97x

//byte-bit definitions
typedef struct {
	volatile unsigned char B0:1;	//bit 0
	volatile unsigned char B1:1;	//bit 1
	volatile unsigned char B2:1;	//bit 2
	volatile unsigned char B3:1;	//bit 3
	volatile unsigned char B4:1;	//bit 4
	volatile unsigned char B5:1;	//bit 5
	volatile unsigned char B6:1;	//bit 6
	volatile unsigned char B7:1;	//bit 7
} B8_TypeDef;

//word definitions
typedef struct {
	volatile unsigned char B0:1;	//bit 0
	volatile unsigned char B1:1;	//bit 1
	volatile unsigned char B2:1;	//bit 2
	volatile unsigned char B3:1;	//bit 3
	volatile unsigned char B4:1;	//bit 4
	volatile unsigned char B5:1;	//bit 5
	volatile unsigned char B6:1;	//bit 6
	volatile unsigned char B7:1;	//bit 7
	volatile unsigned char B8:1;	//bit 8
	volatile unsigned char B9:1;	//bit 9
	volatile unsigned char B10:1;	//bit 10
	volatile unsigned char B11:1;	//bit 11
	volatile unsigned char B12:1;	//bit 12
	volatile unsigned char B13:1;	//bit 13
	volatile unsigned char B14:1;	//bit 14
	volatile unsigned char B15:1;	//bit 15
} B16_TypeDef;

//Dword definitions
typedef struct {
	volatile unsigned char B0:1;	//bit 0
	volatile unsigned char B1:1;	//bit 1
	volatile unsigned char B2:1;	//bit 2
	volatile unsigned char B3:1;	//bit 3
	volatile unsigned char B4:1;	//bit 4
	volatile unsigned char B5:1;	//bit 5
	volatile unsigned char B6:1;	//bit 6
	volatile unsigned char B7:1;	//bit 7
	volatile unsigned char B8:1;	//bit 8
	volatile unsigned char B9:1;	//bit 9
	volatile unsigned char B10:1;	//bit 10
	volatile unsigned char B11:1;	//bit 11
	volatile unsigned char B12:1;	//bit 12
	volatile unsigned char B13:1;	//bit 13
	volatile unsigned char B14:1;	//bit 14
	volatile unsigned char B15:1;	//bit 15
	volatile unsigned char B16:1;	//bit 16
	volatile unsigned char B17:1;	//bit 17
	volatile unsigned char B18:1;	//bit 18
	volatile unsigned char B19:1;	//bit 19
	volatile unsigned char B20:1;	//bit 20
	volatile unsigned char B21:1;	//bit 21
	volatile unsigned char B22:1;	//bit 22
	volatile unsigned char B23:1;	//bit 23
	volatile unsigned char B24:1;	//bit 24
	volatile unsigned char B25:1;	//bit 25
	volatile unsigned char B26:1;	//bit 26
	volatile unsigned char B27:1;	//bit 28
	volatile unsigned char B28:1;	//bit 28
	volatile unsigned char B29:1;	//bit 29
	volatile unsigned char B30:1;	//bit 30
	volatile unsigned char B31:1;	//bit 31
} B32_TypeDef;


#endif
