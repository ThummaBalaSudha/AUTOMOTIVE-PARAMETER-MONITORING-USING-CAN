#include <LPC21xx.h>      // LPC21xx register definitions

#include "delay.h"        // Delay functions
#include "lcd.h"          // LCD function declarations
#include "defines.h"      // Bit manipulation macros
#include "types.h"        // User-defined data types

//-----------------------------------------------------------
// LCD Pin Connections
//-----------------------------------------------------------
#define LCD_DAT 8      // LCD Data lines connected from P0.8 to P0.15
#define RS      17     // Register Select pin
#define RW      19     // Read/Write pin
#define EN      18     // Enable pin


//-----------------------------------------------------------
// Function : LCD_Init()
// Purpose  : Initialize the 16x2 LCD
//-----------------------------------------------------------
void LCD_Init(void)
{
    // Configure LCD data pins as output
    WRITEBYTE(IODIR0, LCD_DAT, 0xFF);

    // Configure LCD control pins as output
    WRITEBIT(IODIR0, RS, 1);
    WRITEBIT(IODIR0, RW, 1);
    WRITEBIT(IODIR0, EN, 1);

    // Wait for LCD power-up
    delay_ms(16);

    // Initialization sequence
    Write_CMD_LCD(0x30);
    delay_ms(6);

    Write_CMD_LCD(0x30);
    delay_ms(1);

    Write_CMD_LCD(0x30);
    delay_ms(1);

    // 8-bit mode, 2-line display, 5x7 font
    Write_CMD_LCD(0x38);

    // Display OFF
    Write_CMD_LCD(0x10);

    // Clear LCD
    Write_CMD_LCD(0x01);

    // Auto increment cursor
    Write_CMD_LCD(0x06);

    // Display ON, Cursor ON, Blinking Cursor
    Write_CMD_LCD(0x0F);
}


//-----------------------------------------------------------
// Function : Write_CMD_LCD()
// Purpose  : Send a command to LCD
//-----------------------------------------------------------
void Write_CMD_LCD(char cmd)
{
    // RS = 0 selects Command Register
    WRITEBIT(IOCLR0, RS, 1);

    Write_LCD(cmd);
}


//-----------------------------------------------------------
// Function : Write_DAT_LCD()
// Purpose  : Send one character to LCD
//-----------------------------------------------------------
void Write_DAT_LCD(char dat)
{
    // RS = 1 selects Data Register
    WRITEBIT(IOSET0, RS, 1);

    Write_LCD(dat);
}


//-----------------------------------------------------------
// Function : Write_LCD()
// Purpose  : Common function to write command/data
//-----------------------------------------------------------
void Write_LCD(char ch)
{
    // RW = 0 (Write Operation)
    WRITEBIT(IOCLR0, RW, 1);

    // Put byte on data bus
    WRITEBYTE(IOPIN0, LCD_DAT, ch);

    // Generate Enable Pulse
    WRITEBIT(IOSET0, EN, 1);
    WRITEBIT(IOCLR0, EN, 1);

    // Wait for LCD execution
    delay_ms(2);
}


//-----------------------------------------------------------
// Function : Write_str_LCD()
// Purpose  : Display a string on LCD
//-----------------------------------------------------------
void Write_str_LCD(char *p)
{
    while(*p)
        Write_DAT_LCD(*p++);
}


//-----------------------------------------------------------
// Function : Write_int_LCD()
// Purpose  : Display a signed integer on LCD
//-----------------------------------------------------------
void Write_int_LCD(signed int n)
{
    char a[10], i = 0;

    // Handle negative numbers
    if(n < 0)
    {
        Write_DAT_LCD('-');
        n = -n;
    }

    // Convert integer into ASCII digits
    do
    {
        a[i++] = (n % 10) + '0';
        n /= 10;
    }
    while(n);

    // Display digits in correct order
    for(; i > 0; i--)
        Write_DAT_LCD(a[i-1]);
}


//-----------------------------------------------------------
// Function : Write_float_LCD()
// Purpose  : Display floating-point value
// Arguments:
//      f -> Float value
//      i -> Digits after decimal point
//-----------------------------------------------------------
void Write_float_LCD(float f, char i)
{
    unsigned long int n = f;

    // Display integer part
    Write_int_LCD(n);

    // Display decimal point
    Write_DAT_LCD('.');

    // Display fractional part
    for(; i > 0; i--)
    {
        f = f * 10;
        n = f;

        Write_DAT_LCD((n % 10) + '0');
    }
}


//-----------------------------------------------------------
// Function : StoreLeftInd()
// Purpose  : Store Left Arrow custom character
//             into LCD CGRAM
//-----------------------------------------------------------
void StoreLeftInd(void)
{
    unsigned char i;

    unsigned char a[] =
    {
        0x01,
        0x03,
        0x07,
        0x0F,
        0x07,
        0x03,
        0x01,
        0x00
    };

    for(i = 0; i < 8; i++)
        Write_DAT_LCD(a[i]);
}


//-----------------------------------------------------------
// Function : StoreRightInd()
// Purpose  : Store Right Arrow custom character
//-----------------------------------------------------------
void StoreRightInd(void)
{
    unsigned char i;

    unsigned char b[] =
    {
        0x10,
        0x18,
        0x1C,
        0x1E,
        0x1C,
        0x18,
        0x10,
        0x00
    };

    for(i = 0; i < 8; i++)
        Write_DAT_LCD(b[i]);
}


//-----------------------------------------------------------
// Function : FuelBar_()
// Purpose  : Store custom fuel bar symbols in CGRAM
//
// Location 0 -> Empty Block
// Location 1 -> Half Block
// Location 2 -> Full Block
//-----------------------------------------------------------
void FuelBar_(void)
{
    u8 i;

    // Empty Block
    u8 empty[8] =
    {
        0x1F,0x11,0x11,0x11,
        0x11,0x11,0x1F,0x00
    };

    // Half Filled Block
    u8 half[8] =
    {
        0x1F,0x11,0x11,0x11,
        0x1F,0x1F,0x1F,0x00
    };

    // Fully Filled Block
    u8 full[8] =
    {
        0x1F,0x1F,0x1F,0x1F,
        0x1F,0x1F,0x1F,0x00
    };

    // Start CGRAM Address
    Write_CMD_LCD(0x40);

    // Store Empty Character at location 0
    for(i=0;i<8;i++)
        Write_DAT_LCD(empty[i]);

    // Store Half Character at location 1
    for(i=0;i<8;i++)
        Write_DAT_LCD(half[i]);

    // Store Full Character at location 2
    for(i=0;i<8;i++)
        Write_DAT_LCD(full[i]);
}


//-----------------------------------------------------------
// Function : FuelBar()
// Purpose  : Display fuel level using custom characters
// Input    : fuel percentage (0-100)
//-----------------------------------------------------------
void FuelBar(u8 fuel)
{
    u8 blocks = fuel / 20;   // Number of full blocks
    u8 rem    = fuel % 20;   // Remaining percentage
    u8 i;

    // Display Full Blocks
    for(i=0; i<blocks; i++)
        Write_DAT_LCD(2);

    // Display Half Block if required
    if(rem >= 10)
    {
        Write_DAT_LCD(1);
        blocks++;
    }

    // Fill remaining positions with Empty Blocks
    for(i=blocks; i<5; i++)
        Write_DAT_LCD(0);
}
            
