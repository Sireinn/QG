#include <reg52.h>

sbit LSA = P2^2;
sbit LSB = P2^3;
sbit LSC = P2^4;

typedef unsigned char u8;
typedef unsigned int u16;

u8 code SegTable[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};
u8 DisplayBuff[2] = {0, 0};
u8 Counter = 0;
u16 TickCount = 0;

void Timer0_Init() 
{
    TMOD |= 0x01;
    TH0 = 0xFC;
    TL0 = 0x18;
    ET0 = 1;
    EA = 1;
    TR0 = 1;
}

void Timer0_ISR() interrupt 1 
{
    static u8 i = 0;
    TH0 = 0xFC;
    TL0 = 0x18;

    P0 = 0x00;

    i++;
    if(i >= 2) 
		{
			i = 0;
		}
    switch(i) 
			{
        case 0: LSA=0; LSB=0; LSC=0; break;
        case 1: LSA=1; LSB=0; LSC=0; break;
			}

    P0 = SegTable[DisplayBuff[i]];

    TickCount++;
    if(TickCount >= 1000) 
		{
        TickCount = 0;
        Counter++;
        if(Counter > 99) 
				{
					Counter = 0;
				}
    }
}

void main()
{
    Timer0_Init();
    while(1) 
		{
        DisplayBuff[0] = Counter % 10;
        DisplayBuff[1] = Counter / 10;
    }
}