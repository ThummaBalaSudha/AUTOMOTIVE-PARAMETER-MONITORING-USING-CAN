#include <lpc21xx.h>
#include "types.h"
#include "can.h"
#include "can_defines.h"

#include "lcd.h"
#include "delay.h"
#include "ds18b20.h"

#include "defines.h"
#include "interrupt.h"

volatile int left,right;

u8 can_msg_available(void)
{
  return (C1GSR&RBS_BIT_READ)?1:0;
}

int main(void)
{
        int temp;
        unsigned char tp,tpd;

        struct CAN_Frame txFrame;
        struct CAN_Frame rxFrame;

        txFrame.ID=2;
        txFrame.vbf.RTR=0; //data frame
    txFrame.vbf.DLC=1;

    Init_CAN1();
        LCD_Init();

        FuelBar_();

        Enable_EINT0();
        Enable_EINT1();

        Write_CMD_LCD(0x80);//selecting starting line and staring pos
	 Write_str_LCD("MONITORING SYSTEM");
        delay_ms(1000);
        Write_CMD_LCD(0xC0);
        Write_str_LCD("Engine:");
        Write_CMD_LCD(0x94);
        Write_str_LCD("Fuel:");
        Write_CMD_LCD(0xD4);
        Write_str_LCD("Indicator:");
        Write_CMD_LCD(0x40+24);
        StoreLeftInd();
        Write_CMD_LCD(0x40+32);
        StoreRightInd();

        while(1)
        {
                if(ResetDS18b20() == 0)
                {
                        temp=ReadTemp();  //READING TEMPERATURE FROM DS18B20 USING 1-WIRE PROTOCOL
                        tp  = temp>>4;    //GETTING INTEGER PART
                        tpd=temp&0x08?0x35:0x30;//GETTING FRACTIONAL PART

                        Write_CMD_LCD(0xC7);
                        Write_int_LCD(tp);
                        Write_DAT_LCD('.');
                        Write_DAT_LCD(tpd);
                        Write_str_LCD(" C  ");
                }
        else   // sensor not present
        {
                Write_CMD_LCD(0xC7);
                Write_str_LCD("No Sensor");
        }
                if(left==1 && right==0)
                {
									 txFrame.Data1=0x1;
                        CAN1_Tx(txFrame);
                        Write_CMD_LCD(0xDE);
                        Write_DAT_LCD(3);
                        delay_ms(100);
                        Write_CMD_LCD(0xDE);
                        Write_DAT_LCD(' ');
                        delay_ms(100);
                }
                else if(right==1 && left==0)
                {
                        txFrame.Data1=0x2;
                        CAN1_Tx(txFrame);
                        Write_CMD_LCD(0xDF);
                        Write_DAT_LCD(4);
                        delay_ms(100);
                        Write_CMD_LCD(0xDF);
                        Write_DAT_LCD(' ');
                        delay_ms(100);
                }
                else
                {
                        txFrame.Data1=0x0;
                CAN1_Tx(txFrame);
                        Write_CMD_LCD(0xDE);
                        Write_DAT_LCD(3);
                        Write_CMD_LCD(0xDF);
                        Write_DAT_LCD(4);
                }
                delay_ms(100);
                if(can_msg_available())
                {
                        CAN1_Rx(&rxFrame);
                         if(rxFrame.ID==4)
                        {
                                Write_CMD_LCD(0x99);
                                FuelBar(rxFrame.Data1);
                                Write_CMD_LCD(0x9E);
                                Write_str_LCD("          ");

																Write_CMD_LCD(0x9E);
                                Write_int_LCD(rxFrame.Data1);
                                Write_DAT_LCD('%');
                        }
                }
        }
}

                                      