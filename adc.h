#ifndef _ADC_H_          // Checks whether ADC_H is already defined
#define _ADC_H_          // Defines ADC_H to prevent multiple inclusion

#include "types.h"       // Contains user-defined data types (u8, f32, etc.)

//-----------------------------------------------------------
// Function : Init_ADC()
// Purpose  : Initializes the ADC peripheral by configuring
//            the ADC Control Register (ADCR).
// Input    : None
// Return   : None
//-----------------------------------------------------------
void Init_ADC(void);

//-----------------------------------------------------------
// Function : Read_ADC()
// Purpose  : Reads the analog input from the specified ADC
//            channel and returns the 10-bit digital value.
// Input    : chNo - ADC channel number (0 to 7)
// Return   : ADC value (0 to 1023) as float
//-----------------------------------------------------------
f32 Read_ADC(u8 chNo);

#endif                  // End of ADC_H include guard
