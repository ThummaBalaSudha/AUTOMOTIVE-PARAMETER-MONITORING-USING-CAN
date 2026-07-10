#include <LPC21xx.h>      // LPC21xx register definitions

#include "types.h"        // User-defined data types
#include "adc.h"          // ADC function declarations
#include "adc_defines.h"  // ADC configuration macros
#include "delay.h"        // Delay function declarations

#include "can.h"          // CAN function declarations
#include "can_defines.h"  // CAN configuration macros

// Variable to store ADC reading
f32 aR;

// Variable to store calculated fuel percentage
int fuel;

int main(void)
{
    //-------------------------------------------------------
    // Create CAN frame for transmission
    //-------------------------------------------------------
    struct CAN_Frame txFrame;

    // Assign CAN Identifier
    txFrame.ID = 4;

    // RTR = 0 indicates Data Frame
    txFrame.vbf.RTR = 0;

    // DLC = 1 means one byte of data will be transmitted
    txFrame.vbf.DLC = 1;

    //-------------------------------------------------------
    // Initialize peripherals
    //-------------------------------------------------------
    Init_ADC();      // Initialize ADC module
    Init_CAN1();     // Initialize CAN Controller 1

    //-------------------------------------------------------
    // Main loop
    //-------------------------------------------------------
    while(1)
    {
        // Read analog value from ADC Channel 1
        aR = Read_ADC(CH1);

        //---------------------------------------------------
        // Convert ADC value into fuel percentage
        //
        // Calibration values:
        // Empty Tank ADC Value = 82
        // Full Tank ADC Value  = 656
        //
        // Formula:
        // Fuel % = ((Current ADC - Empty ADC) × 100)
        //          / (Full ADC - Empty ADC)
        //---------------------------------------------------
        fuel = ((aR - 82) * 100) / (656 - 82);

        // Store fuel percentage in CAN data field
        txFrame.Data1 = fuel;

        // Wait until previous CAN transmission completes
        while(!(C1GSR & TCS1_BIT_READ));

        // Transmit CAN frame
        CAN1_Tx(txFrame);

        // Wait for 200 ms before next transmission
        delay_ms(200);
    }
}
