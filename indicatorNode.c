#include <LPC21xx.h>      // LPC21xx register definitions
#include "delay.h"        // Delay function declarations
#include "can.h"          // CAN function declarations
#include "can_defines.h"  // CAN macro definitions

//-----------------------------------------------------------
// LED Pin Definitions
// LEDs are connected from P0.10 to P0.17
//-----------------------------------------------------------
#define LED0 10
#define LED1 11
#define LED2 12
#define LED3 13
#define LED4 14
#define LED5 15
#define LED6 16
#define LED7 17

int main()
{
    int i;
    int cmd = 0;                  // Variable to store received command
    struct CAN_Frame rxFrame;     // CAN frame for received data

    //-------------------------------------------------------
    // Configure LED pins as Output
    //-------------------------------------------------------
    IODIR0 |= (0xFF << LED0);

    // Turn OFF all LEDs initially
    // (Active-Low LEDs: HIGH = OFF)
    IOSET0 = (0xFF << LED0);

    //-------------------------------------------------------
    // Initialize CAN Controller
    //-------------------------------------------------------
    Init_CAN1();

    //-------------------------------------------------------
    // Main Loop
    //-------------------------------------------------------
    while(1)
    {
        //---------------------------------------------------
        // Check whether a CAN frame has been received
        //---------------------------------------------------
        if(C1GSR & RBS_BIT_READ)
        {
            // Receive CAN frame
            CAN1_Rx(&rxFrame);

            // Accept only frames with CAN ID = 2
            if(rxFrame.ID == 2)
                cmd = rxFrame.Data1;
        }

        //---------------------------------------------------
        // Command = 1
        // LEDs blink from LEFT to RIGHT
        //---------------------------------------------------
        if(cmd == 1)
        {
            for(i = LED0; i <= LED7; i++)
            {
                // Check whether a new command is received
                while blinking
                if(C1GSR & RBS_BIT_READ)
                {
                    CAN1_Rx(&rxFrame);

                    if(rxFrame.ID == 2)
                        cmd = rxFrame.Data1;
                }

                // Exit if command changes
                if(cmd != 1)
                    break;

                // Turn LED ON (Active Low)
                IOCLR0 = (1 << i);

                delay_ms(100);

                // Turn LED OFF
                IOSET0 = (1 << i);
            }
        }

        //---------------------------------------------------
        // Command = 2
        // LEDs blink from RIGHT to LEFT
        //---------------------------------------------------
        else if(cmd == 2)
        {
            for(i = LED7; i >= LED0; i--)
            {
                // Check for new CAN command
                if(C1GSR & RBS_BIT_READ)
                {
                    CAN1_Rx(&rxFrame);

                    if(rxFrame.ID == 2)
                        cmd = rxFrame.Data1;
                }

                // Exit if command changes
                if(cmd != 2)
                    break;

                // Turn LED ON
                IOCLR0 = (1 << i);

                delay_ms(100);

                // Turn LED OFF
                IOSET0 = (1 << i);
            }
        }

        //---------------------------------------------------
        // No valid command
        // Keep all LEDs OFF
        //---------------------------------------------------
        else
        {
            IOSET0 = (0xFF << LED0);
        }
    }
}
