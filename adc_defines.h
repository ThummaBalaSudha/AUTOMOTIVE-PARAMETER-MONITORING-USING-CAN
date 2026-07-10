#ifndef _ADC_DEFINES_H_
#define _ADC_DEFINES_H_

//===========================================================
// ADC Configuration Macros for LPC21xx
//===========================================================

//-----------------------------------------------------------
// ADC Channel Selection Bits
// Each bit corresponds to one ADC input channel.
//-----------------------------------------------------------
#define CH0                0x01      // Select ADC Channel 0
#define CH1                0x02      // Select ADC Channel 1
#define CH2                0x04      // Select ADC Channel 2
#define CH3                0x08      // Select ADC Channel 3

// Select the ADC channels to be enabled
// Here, Channel 0 and Channel 1 are enabled.
#define CHANNEL_SEL        (CH0 | CH1)

//-----------------------------------------------------------
// Clock Configuration
//-----------------------------------------------------------

// Oscillator Frequency = 12 MHz
#define FOSC               12000000

// CPU Clock (CCLK) = 5 × FOSC = 60 MHz
#define CCLK               (5 * FOSC)

// Peripheral Clock (PCLK) = CCLK / 4 = 15 MHz
#define PCLK               (CCLK / 4)

// Required ADC Clock Frequency
// LPC21xx ADC operates correctly up to 4.5 MHz.
// Here it is configured for 3.75 MHz.
#define ADCLK              3750000

// Clock Divider Value
// Formula:
// ADCLK = PCLK / (CLKDIV + 1)
//
// CLKDIV bits are located in bits [15:8] of ADCR,
// so the calculated value is shifted left by 8 bits.
#define CLKDIV             (((PCLK / ADCLK) - 1) << 8)

//-----------------------------------------------------------
// ADCR (ADC Control Register) Bit Definitions
//-----------------------------------------------------------

// Power Down Bit
// 1 = ADC is powered ON
// 0 = ADC is powered OFF
#define PDN_BIT            (1 << 21)

// START bit position
// Used to begin ADC conversion
#define ADC_START_BIT      24

//-----------------------------------------------------------
// ADDR (ADC Data Register) Bit Definitions
//-----------------------------------------------------------

// DONE bit position
// Becomes '1' when ADC conversion is complete
#define DONE_BIT           31

#endif

#endif
