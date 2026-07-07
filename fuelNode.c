#include<LPC21xx.h>
#include "types.h"
#include "adc.h"
#include "adc_defines.h"
#include "delay.h"

#include "can.h"
#include "can_defines.h"

f32 aR;
int fuel;

int main(void)
{
        struct CAN_Frame txFrame;
        txFrame.ID=4;
        txFrame.vbf.RTR=0; //data frame
        txFrame.vbf.DLC=1;

        Init_ADC();
        Init_CAN1();

        while(1)
        {
        aR=Read_ADC(CH1);
                //fuel=(((aR-0.274)/(1.974-0.274))*100);  //min=77,max=574
                //fuel = ((aR - 84) * 100) / (612 - 84);
                //fuel = (((aR - 69) /(558 - 69))*100);
                //fuel = (((aR - 82) / (738 - 82)) * 100);
                fuel = ((aR - 82 ) *100) / (656 - 82); //2.158
                txFrame.Data1=fuel;
                while(!(C1GSR & TCS1_BIT_READ));
                CAN1_Tx(txFrame);
                delay_ms(200);
        }
}