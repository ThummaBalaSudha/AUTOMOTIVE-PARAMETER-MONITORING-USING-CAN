#include <lpc21xx.h>
#include "pin_function_defines.h"
#include "defines.h"
#include "interrupt.h"

extern int left;
extern int right;
void Enable_EINT0(void)
{ 
	CFGPIN(PINSEL0,1,FUNC4);
	SSETBIT(VICIntEnable,14);
	VICVectCntl0=0x20|14;
	VICVectAddr0=(unsigned)eint0_isr;
	SCLRBIT(EXTINT,0);
  SETBIT(EXTMODE,0);
  SETBIT(EXTPOLAR,0);
}

void Enable_EINT1(void)
{
	CFGPIN(PINSEL0,3,FUNC4);
	SSETBIT(VICIntEnable,15);
	VICVectCntl1=0x20|15;
	VICVectAddr1=(unsigned)eint1_isr;
	SCLRBIT(EXTINT,1);
  SETBIT(EXTMODE,1);
  SETBIT(EXTPOLAR,1);
}

void eint0_isr(void) __irq
{
	left=!left;
	right=0;
	SSETBIT(EXTINT,0);//clear EINT0 flag
	VICVectAddr=0;//dummy write to clear 
	              //interrupt flag in VIC
  //delay_ms(100);	
}
void eint1_isr(void) __irq
{
	right=!right;
	left=0;
	SSETBIT(EXTINT,1);//clear EINT1 flag
	VICVectAddr=0;//dummy write;
  //delay_ms(100);	
}
