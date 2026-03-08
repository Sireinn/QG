#include <reg52.h>

typedef unsigned int u16;
typedef unsigned char u8;

// 硬件定义
#define SMG P0
#define KEY_MATRIX_PORT P1

// LED 指示灯 (使用 sbit 独立控制，避开 P2.2-P2.4 的 74HC138)
sbit LED1 = P2^0;
sbit LED2 = P2^1;

// 数码管位选 (P2.2-P2.4)
sbit LSA = P2^2;
sbit LSB = P2^3;
sbit LSC = P2^4;

// 全局变量
u16 time_count = 0;   // 计时 (0.01s)
u8 state = 0;         // 0:待机, 1:运行, 2:暂停

u8 gsmg_code[17] = {
    0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 
    0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71
};

// 基础函数
void delay_10us(u16 ten_us)
 { 
 while(ten_us--);
 }

// 数码管动态扫描
void Display_Time(u16 t)
{
    u8 i;
    u8 disp_buf[4];
    disp_buf[0] = t % 10;
    disp_buf[1] = (t / 10) % 10;
    disp_buf[2] = (t / 100) % 10;
    disp_buf[3] = (t / 1000) % 10;

    for (i = 0; i < 4; i++)
    {
        switch(i)
        {
            case 0: LSC=0; LSB=0; LSA=0; break;
            case 1: LSC=0; LSB=0; LSA=1; break;
            case 2: LSC=0; LSB=1; LSA=0; break;
            case 3: LSC=0; LSB=1; LSA=1; break;
        }
        SMG = gsmg_code[disp_buf[i]];
        if(i == 2) SMG |= 0x80; // 小数点
        delay_10us(500);
        SMG = 0x00; // 消影
    }
}

// 成绩判断
void Judge_Score(u16 t)
{
    if ((t >= 950) && (t <= 1050)) 
		{ 
			LED1 = 0; LED2 = 1; 
		}
    else if (((t >= 850) && (t < 950)) || ((t > 1050) && (t <= 1150))) 
		{ 
			LED1 = 0; LED2 = 0; 
		}
    else 
		{
		LED1 = 1; LED2 = 1;
		}
}

// 矩阵扫描
u8 key_matrix_ranks_scan(void)
{
    u8 key_value = 0;
    
   
    KEY_MATRIX_PORT = 0x7f; 
    if(KEY_MATRIX_PORT != 0x7f)
    {
        delay_10us(1000); // 消抖
        if(KEY_MATRIX_PORT != 0x7f)
        {
            switch(KEY_MATRIX_PORT)
            {
                case 0x77: key_value = 1; break; // S1
                case 0x7b: key_value = 2; break; // S2 
                case 0x7d: key_value = 3; break; // S3 
                case 0x7e: key_value = 4; break; // S4 
            }
            while(KEY_MATRIX_PORT != 0x7f) { Display_Time(time_count); }
        }
    }
    return key_value;
}

void main()
{
		P2 = 0xFF; 
		u8 key = 0;
    TMOD = 0x01;
    TH0 = (65536 - 9216) / 256;
    TL0 = (65536 - 9216) % 256;
    ET0 = 1; EA = 1;
    LED1 = 1; LED2 = 1;

    while(1)
    {
        key = key_matrix_ranks_scan(); 
        
        if(key == 1)      // S1 启动
        {
            state = 1; TR0 = 1; LED1 = 1; LED2 = 1;
        }
        else if(key == 2) // S2 暂停
        {
            state = 2; TR0 = 0; Judge_Score(time_count);
        }
        else if(key == 3) // S3 清零
        {
            state = 0; TR0 = 0; time_count = 0; LED1 = 1; LED2 = 1;
        }

        Display_Time(time_count);
    }
}

void Timer0_ISR(void) interrupt 1
{
    TH0 = (65536 - 9216) / 256;
    TL0 = (65536 - 9216) % 256;
    if(state == 1) time_count++;
}