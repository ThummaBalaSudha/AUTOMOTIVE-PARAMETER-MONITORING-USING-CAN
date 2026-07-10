#include <LPC21xx.h>      // LPC21xx register definitions

#include "types.h"        // User-defined data types
#include "can.h"          // CAN driver
#include "can_defines.h"  // CAN macros

#include "lcd.h"          // LCD driver
#include "delay.h"        // Delay functions
#include "ds18b20.h"      // DS18B20 temperature sensor driver

#include "defines.h"      // Common macros
#include "interrupt.h"    // External interrupt functions

//-----------------------------------------------------------
// Global variables
//-----------------------------------------------------------

// Indicator status variables
// Updated inside External Interrupt ISRs
volatile int left, right;


//-----------------------------------------------------------
// Function : can_msg_available()
// Purpose  : Check whether a CAN message has been received.
// Return   :
//      1 -> Message available
//      0 -> No message
//-----------------------------------------------------------
u8 can_msg_available(void)
{
    return (C1GSR & RBS_BIT_READ) ? 1 : 0;
}

int main(void)
{
    int temp;
    unsigned char tp, tpd;

    //-------------------------------------------------------
    // CAN frame structures
    //-------------------------------------------------------
    struct CAN_Frame txFrame;
    struct CAN_Frame rxFrame;

    //-------------------------------------------------------
    // Configure CAN Transmit Frame
    //-------------------------------------------------------
    txFrame.ID = 2;              // CAN ID for Indicator Node
    txFrame.vbf.RTR = 0;         // Data Frame
    txFrame.vbf.DLC = 1;         // One byte of data

    //-------------------------------------------------------
    // Initialize peripherals
    //-------------------------------------------------------
    Init_CAN1();          // Initialize CAN controller
    LCD_Init();           // Initialize LCD

    // Store custom fuel bar characters into LCD CGRAM
    FuelBar_();

    // Enable Left and Right Indicator Interrupts
    Enable_EINT0();
    Enable_EINT1();

    //-------------------------------------------------------
    // LCD Welcome Screen
    //-------------------------------------------------------
    Write_CMD_LCD(0x80);
    Write_str_LCD("MONITORING SYSTEM");

    delay_ms(1000);

    // Display static labels
    Write_CMD_LCD(0xC0);
    Write_str_LCD("Engine:");

    Write_CMD_LCD(0x94);
    Write_str_LCD("Fuel:");

    Write_CMD_LCD(0xD4);
    Write_str_LCD("Indicator:");

    //-------------------------------------------------------
    // Store custom Left Arrow into CGRAM location 3
    //-------------------------------------------------------
    Write_CMD_LCD(0x40 + 24);
    StoreLeftInd();

    //-------------------------------------------------------
    // Store custom Right Arrow into CGRAM location 4
    //-------------------------------------------------------
    Write_CMD_LCD(0x40 + 32);
    StoreRightInd();

    //-------------------------------------------------------
    // Main Loop
    //-------------------------------------------------------
    while(1)
    {
        //---------------------------------------------------
        // Read Engine Temperature
        //---------------------------------------------------
        if(ResetDS18b20() == 0)
        {
            // Read raw temperature
            temp = ReadTemp();

            // Integer part
            tp = temp >> 4;

            // Fractional part
            // 0x35 = '5'
            // 0x30 = '0'
            tpd = (temp & 0x08) ? 0x35 : 0x30;

            // Display Temperature
            Write_CMD_LCD(0xC7);
            Write_int_LCD(tp);

            Write_DAT_LCD('.');

            Write_DAT_LCD(tpd);

            Write_str_LCD(" C  ");
        }
        else
        {
            // Sensor not detected
            Write_CMD_LCD(0xC7);
            Write_str_LCD("No Sensor");
        }

        //---------------------------------------------------
        // LEFT Indicator Selected
        //---------------------------------------------------
        if(left == 1 && right == 0)
        {
            // Send command through CAN
            txFrame.Data1 = 0x01;

            CAN1_Tx(txFrame);

            // Blink Left Arrow on LCD
            Write_CMD_LCD(0xDE);
            Write_DAT_LCD(3);

            delay_ms(100);

            Write_CMD_LCD(0xDE);
            Write_DAT_LCD(' ');

            delay_ms(100);
        }

        //---------------------------------------------------
        // RIGHT Indicator Selected
        //---------------------------------------------------
        else if(right == 1 && left == 0)
        {
            txFrame.Data1 = 0x02;

            CAN1_Tx(txFrame);

            // Blink Right Arrow
            Write_CMD_LCD(0xDF);
            Write_DAT_LCD(4);

            delay_ms(100);

            Write_CMD_LCD(0xDF);
            Write_DAT_LCD(' ');

            delay_ms(100);
        }

        //---------------------------------------------------
        // No Indicator Selected
        //---------------------------------------------------
        else
        {
            txFrame.Data1 = 0x00;

            CAN1_Tx(txFrame);

            // Display both indicator symbols
            Write_CMD_LCD(0xDE);
            Write_DAT_LCD(3);

            Write_CMD_LCD(0xDF);
            Write_DAT_LCD(4);
        }

        delay_ms(100);

        //---------------------------------------------------
        // Receive Fuel Percentage from Fuel Node
        //---------------------------------------------------
        if(can_msg_available())
        {
            CAN1_Rx(&rxFrame);

            // Accept only Fuel Node messages
            if(rxFrame.ID == 4)
            {
                // Display graphical fuel bar
                Write_CMD_LCD(0x99);

                FuelBar(rxFrame.Data1);

                // Clear previous percentage
                Write_CMD_LCD(0x9E);
                Write_str_LCD("          ");

                // Display Fuel Percentage
                Write_CMD_LCD(0x9E);

                Write_int_LCD(rxFrame.Data1);

                Write_DAT_LCD('%');
            }
        }
    }
}

                                      
