#include<LPC21xx.h>
#include "delay.h"
#include "can.h"
#include "can_defines.h"

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
        int cmd = 0;
        struct CAN_Frame rxFrame;
        IODIR0 |= (0xFF << LED0);
        IOSET0 = (0xFF << LED0);
        Init_CAN1();
        while(1)
        {
                if(C1GSR & RBS_BIT_READ)
                {
                        CAN1_Rx(&rxFrame);
                        if(rxFrame.ID == 2)
                                cmd = rxFrame.Data1;
                }
                if(cmd == 1)
                {
                        for(i=LED0;i<=LED7;i++)
                        {
                                        if(C1GSR & RBS_BIT_READ)
                                        {
                                                        CAN1_Rx(&rxFrame);
                                                        if(rxFrame.ID==2)
                                              
																												cmd=rxFrame.Data1;
                                        }
																				 if(cmd != 1)
                                                break;
                                        IOCLR0=(1 << i);
                                        delay_ms(100);
                                        IOSET0=(1 << i);
                        }
                }
                else if(cmd == 2)
                {
                        for(i=LED7;i>=LED0;i--)
                        {
                                if(C1GSR & RBS_BIT_READ)
                                        {
                                                        CAN1_Rx(&rxFrame);
                                                        if(rxFrame.ID==2)
                                                                cmd=rxFrame.Data1;
                                        }
                                        if(cmd != 2)
                                                break;
                                        IOCLR0=(1 << i);
                                        delay_ms(100);
                                        IOSET0=(1 << i);
                        }
                }
                else
                {
                        IOSET0 = (0xFF<<LED0);
                }
   }
}

