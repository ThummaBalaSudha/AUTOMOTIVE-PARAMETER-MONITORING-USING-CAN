#include <LPC21xx.h>      // LPC21xx register definitions

#include "types.h"        // User-defined data types (u8, u16, f32, etc.)
#include "defines.h"      // Common macros like SETBIT, CLRBIT, READBIT, WRITEBYTE
#include "adc_defines.h"  // ADC-specific macro definitions
#include "delay.h"        // Delay function declarations

//-----------------------------------------------------------
// Function: Init_ADC()
// Purpose : Initialize the ADC peripheral
//-----------------------------------------------------------
void Init_ADC(void)
{
    // Configure ADC Control Register (ADCR)
    // PDN_BIT     -> Powers ON the ADC module
    // CLKDIV      -> Sets ADC clock divider
    // CHANNEL_SEL -> Selects the default ADC channel
    ADCR = PDN_BIT | CLKDIV | CHANNEL_SEL;
}

//-----------------------------------------------------------
// Function: Read_ADC()
// Purpose : Read analog value from the selected ADC channel
// Input   : chNo - ADC channel number (0 to 7)
// Return  : 10-bit ADC digital value (0 to 1023)
//-----------------------------------------------------------
f32 Read_ADC(u8 chNo)
{
    u16 adcVal = 0;     // Variable to store ADC result
    f32 eAR;            // Variable for converted analog voltage (currently unused)

    // Select the required ADC channel
    WRITEBYTE(ADCR, 0, chNo);

    // Start ADC conversion
    SETBIT(ADCR, ADC_START_BIT);

    // Small delay to allow conversion to begin
    delay_us(3);

    // Wait until ADC conversion is complete
    // DONE_BIT becomes 1 after conversion finishes
    while(!READBIT(ADDR, DONE_BIT));

    // Stop ADC conversion
    CLRBIT(ADCR, ADC_START_BIT);

    // Extract the 10-bit ADC result
    // ADC result is stored in bits [15:6] of ADDR
    adcVal = (ADDR >> 6) & 0x3FF;

    // Convert ADC value into analog voltage (optional)
    // Formula:
    // Voltage = (ADC Value × Reference Voltage) / 1023
    //
    // eAR = (adcVal * 3.3) / 1023;
    // return eAR;

    // Return the raw ADC digital value
    return adcVal;
}
