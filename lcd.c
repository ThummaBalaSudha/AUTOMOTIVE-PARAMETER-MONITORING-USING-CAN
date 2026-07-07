#include<LPC21xx.h>

#include "delay.h"
#include "lcd.h"
#include "defines.h"
#include "types.h"

#define LCD_DAT 8
#define RS 17
#define RW 19
#define EN 18

void LCD_Init(void)
{
	WRITEBYTE(IODIR0,LCD_DAT,0xFF);
	WRITEBIT(IODIR0,RS,1);
	WRITEBIT(IODIR0,RW,1);
	WRITEBIT(IODIR0,EN,1);
	delay_ms(16);
    Write_CMD_LCD(0x30);
    delay_ms(6);
	Write_CMD_LCD(0x30);
    delay_ms(1);
    Write_CMD_LCD(0x30);
    delay_ms(1);
    Write_CMD_LCD(0x38);//set 8-bit mode of operation with 2 lines
						// and 5X7 character font 
	Write_CMD_LCD(0x10);//Turning OFF the display
	Write_CMD_LCD(0x01);//clearing the display
    Write_CMD_LCD(0x06);//Shifting the cursor to the right side
                        //after writing each byte onto the display
	Write_CMD_LCD(0x0f);//Enabling the display with blinking cursor
}

void Write_CMD_LCD(char cmd)
{
    WRITEBIT(IOCLR0,RS,1);
    Write_LCD(cmd);
}

void Write_DAT_LCD(char dat)
{
	WRITEBIT(IOSET0,RS,1);
	Write_LCD(dat);
}

void Write_LCD(char ch)
{
	WRITEBIT(IOCLR0,RW,1);
	WRITEBYTE(IOPIN0,LCD_DAT,ch);
	WRITEBIT(IOSET0,EN,1);
	WRITEBIT(IOCLR0,EN,1);
	delay_ms(2);
}

void Write_str_LCD(char *p)
{
	while(*p)
		Write_DAT_LCD(*p++);	
}

void Write_int_LCD(signed int n)
{
	char a[10],i=0;
    if(n<0)
	{
		Write_DAT_LCD('-');
		n=-n;
	}
	do                       
	{
		a[i++]=n%10+48;	
		n=n/10;
	}while(n);
  for(;i>0;i--)
		Write_DAT_LCD(a[i-1]);
}

void Write_float_LCD(float f,char i)
//1st arg(f) is the float number to be displayed  on LCD
//2nd arg(i) is number digits to be displayed after the dot('.')                      
{
  unsigned long int n=f;
	Write_int_LCD(n);
	Write_DAT_LCD('.');
  for(;i>0;i--)
	{
		f=f*10;
    n=f;	
		Write_DAT_LCD(n%10+48);
	}	
}

void StoreLeftInd(void)
{
	unsigned char i,a[]={0x01,0x03,0x07,0x0f,0x07,0x03,0x01,0x00};
	for(i=0;i<8;i++)
	{
		Write_DAT_LCD(a[i]);
	}
}

void StoreRightInd(void)
{
	unsigned char i,b[]={0x10,0x18,0x1C,0x1E,0x1C,0x18,0x10,0x00};
	for(i=0;i<8;i++)
	{
		Write_DAT_LCD(b[i]);
	}
}

void FuelBar_(void)
{
    u8 i;

    u8 empty[8] = {0x1f,0x11,0x11,0x11,0x11,0x11,0x1f,0x00};
    u8 half[8]  = {0x1f,0x11,0x11,0x11,0x1f,0x1f,0x1f,0x00};
    u8 full[8]  = {0x1f,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f,0x00};

    // CGRAM start
    Write_CMD_LCD(0x40);

    // Store EMPTY ? location 0
    for(i=0;i<8;i++) Write_DAT_LCD(empty[i]);

    // Store HALF ? location 1
    for(i=0;i<8;i++) Write_DAT_LCD(half[i]);

    // Store FULL ? location 2
    for(i=0;i<8;i++) Write_DAT_LCD(full[i]);
}

void FuelBar(u8 fuel)
{
    u8 blocks = fuel / 20;     // full blocks
    u8 rem    = fuel % 20;     // remaining
    u8 i;

    // Print FULL blocks
    for(i=0;i<blocks;i++)
        Write_DAT_LCD(2);

    // Print HALF block (if needed)
    if(rem >= 10)
    {
        Write_DAT_LCD(1);
        blocks++;
    }

    // Print EMPTY blocks
    for(i=blocks;i<5;i++)
        Write_DAT_LCD(0);
}
            