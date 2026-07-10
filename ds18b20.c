#include <LPC21xx.h>      // LPC21xx register definitions
#include "delay.h"        // Delay function declarations

//===========================================================
// DS18B20 1-Wire Interface Definitions
//===========================================================

// Data pin connected to P0.16
#define D   (1<<16)

// Read current status of P0.16
#define R   (IOPIN0 & (1<<16))


//-----------------------------------------------------------
// Function : ResetDS18b20()
// Purpose  : Sends Reset Pulse and checks Presence Pulse
// Return   :
//      1 -> Device Present
//      0 -> Device Not Present
//-----------------------------------------------------------
unsigned char ResetDS18b20(void)
{
    unsigned int presence;

    // Configure data pin as output
    IODIR0 |= D;

    // Keep bus HIGH (Idle State)
    IOPIN0 |= D;

    delay_us(1);

    // Pull bus LOW for about 480 us (Reset Pulse)
    IOPIN0 &= ~D;

    delay_us(478);

    // Release the bus
    IOPIN0 |= D;

    // Wait for Presence Pulse
    delay_us(54);

    // Read bus status
    presence = IOPIN0;

    // Wait until reset sequence completes
    delay_us(423);

    // If line is LOW, DS18B20 responded
    if (presence & R)
        return 1;
    else
        return 0;
}


//-----------------------------------------------------------
// Function : ReadBit()
// Purpose  : Read one bit from DS18B20
// Return   : 0 or 1
//-----------------------------------------------------------
unsigned char ReadBit(void)
{
    unsigned int B;

    // Pull bus LOW to initiate read slot
    IOPIN0 &= ~D;

    delay_us(1);

    // Release bus
    IOPIN0 |= D;

    // Configure pin as input
    IODIR0 &= ~D;

    // Wait for sensor to transmit bit
    delay_us(10);

    // Read bus
    B = IOPIN0;

    // Configure pin back as output
    IODIR0 |= D;

    if (B & R)
        return 1;
    else
        return 0;
}


//-----------------------------------------------------------
// Function : WriteBit()
// Purpose  : Write one bit to DS18B20
// Input    : Dbit (0 or 1)
//-----------------------------------------------------------
void WriteBit(unsigned char Dbit)
{
    // Start write time slot
    IOPIN0 &= ~D;

    delay_us(1);

    // If writing logic '1', release line immediately
    if (Dbit)
        IOPIN0 |= D;

    // Wait until slot completes
    delay_us(58);

    // Release bus
    IOPIN0 |= D;

    delay_us(1);
}


//-----------------------------------------------------------
// Function : ReadByte()
// Purpose  : Read one byte (8 bits) from DS18B20
// Return   : Received byte
//-----------------------------------------------------------
unsigned char ReadByte(void)
{
    unsigned char i;
    unsigned char Din = 0;

    // Read 8 bits (LSB first)
    for(i = 0; i < 8; i++)
    {
        Din |= ReadBit() ? (0x01 << i) : Din;

        delay_us(45);
    }

    return Din;
}


//-----------------------------------------------------------
// Function : WriteByte()
// Purpose  : Write one byte (8 bits) to DS18B20
// Input    : Dout (Byte to transmit)
//-----------------------------------------------------------
void WriteByte(unsigned char Dout)
{
    unsigned char i;

    // Send byte bit-by-bit (LSB first)
    for(i = 0; i < 8; i++)
    {
        WriteBit(Dout & 0x01);

        // Shift next bit to LSB
        Dout = Dout >> 1;

        delay_us(1);
    }

    delay_us(98);
}


//-----------------------------------------------------------
// Function : ReadTemp()
// Purpose  : Read temperature from DS18B20
// Return   : Raw 16-bit temperature value
//-----------------------------------------------------------
int ReadTemp(void)
{
    unsigned char n;
    unsigned char buff[2];
    int temp;

    // Reset DS18B20
    ResetDS18b20();

    // Skip ROM command
    WriteByte(0xCC);

    // Start temperature conversion
    WriteByte(0x44);

    // Wait until conversion completes
    while(ReadByte() == 0xFF);

    // Reset device again
    ResetDS18b20();

    // Skip ROM
    WriteByte(0xCC);

    // Read Scratchpad command
    WriteByte(0xBE);

    // Read first two bytes
    // Byte0 -> Temperature LSB
    // Byte1 -> Temperature MSB
    for(n = 0; n < 2; n++)
    {
        buff[n] = ReadByte();
    }

    // Combine MSB and LSB into 16-bit temperature
    temp = buff[1];
    temp = temp << 8;
    temp = temp | buff[0];

    return temp;
}
