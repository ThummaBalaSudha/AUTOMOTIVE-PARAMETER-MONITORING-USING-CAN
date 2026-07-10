#include <LPC21xx.h>              // LPC21xx register definitions
#include "pin_function_defines.h" // Pin function selection macros
#include "defines.h"              // Common bit manipulation macros
#include "interrupt.h"            // Interrupt function declarations

//-----------------------------------------------------------
// These variables are defined in another source file.
// They store the current indicator status.
//-----------------------------------------------------------
extern int left;
extern int right;


//-----------------------------------------------------------
// Function : Enable_EINT0()
// Purpose  : Configure External Interrupt 0
//            (Used for Left Indicator Switch)
//-----------------------------------------------------------
void Enable_EINT0(void)
{
    // Configure P0.1 as EINT0 pin
    CFGPIN(PINSEL0, 1, FUNC4);

    // Enable EINT0 interrupt in VIC
    SSETBIT(VICIntEnable, 14);

    // Enable Vector Slot 0 for EINT0
    VICVectCntl0 = 0x20 | 14;

    // Load ISR address into Vector Address Register
    VICVectAddr0 = (unsigned)eint0_isr;

    // Clear any pending interrupt flag
    SCLRBIT(EXTINT, 0);

    // Configure interrupt as Edge Triggered
    SETBIT(EXTMODE, 0);

    // Configure Falling Edge Trigger
    SETBIT(EXTPOLAR, 0);
}


//-----------------------------------------------------------
// Function : Enable_EINT1()
// Purpose  : Configure External Interrupt 1
//            (Used for Right Indicator Switch)
//-----------------------------------------------------------
void Enable_EINT1(void)
{
    // Configure P0.3 as EINT1 pin
    CFGPIN(PINSEL0, 3, FUNC4);

    // Enable EINT1 interrupt in VIC
    SSETBIT(VICIntEnable, 15);

    // Enable Vector Slot 1 for EINT1
    VICVectCntl1 = 0x20 | 15;

    // Load ISR address into Vector Address Register
    VICVectAddr1 = (unsigned)eint1_isr;

    // Clear pending interrupt
    SCLRBIT(EXTINT, 1);

    // Configure Edge Triggered Interrupt
    SETBIT(EXTMODE, 1);

    // Configure Rising Edge Trigger
    SETBIT(EXTPOLAR, 1);
}


//-----------------------------------------------------------
// Function : eint0_isr()
// Purpose  : ISR for External Interrupt 0
//            Toggles Left Indicator
//-----------------------------------------------------------
void eint0_isr(void) __irq
{
    // Toggle left indicator status
    left = !left;

    // Turn OFF right indicator
    right = 0;

    // Clear EINT0 interrupt flag
    SSETBIT(EXTINT, 0);

    // Inform VIC that interrupt servicing is complete
    VICVectAddr = 0;

    // Optional software debounce
    // delay_ms(100);
}


//-----------------------------------------------------------
// Function : eint1_isr()
// Purpose  : ISR for External Interrupt 1
//            Toggles Right Indicator
//-----------------------------------------------------------
void eint1_isr(void) __irq
{
    // Toggle right indicator status
    right = !right;

    // Turn OFF left indicator
    left = 0;

    // Clear EINT1 interrupt flag
    SSETBIT(EXTINT, 1);

    // Inform VIC that interrupt servicing is complete
    VICVectAddr = 0;

    // Optional software debounce
    // delay_ms(100);
}
