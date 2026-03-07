#include <reg52.h>

#define SMG_PORT P0
sbit LSA = P2^2;
sbit LSB = P2^3;
sbit LSC = P2^4;

unsigned char code smg_code[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
unsigned char display_buf[8] = {0, 0, 0, 0, 0, 0, 0, 0};

unsigned int ms_count = 0;
unsigned long total_seconds = 0;
volatile bit update_flag = 0; // 标志位：更新显存

void Timer0_Init(void)
{
    TMOD &= 0xF0;
    TMOD |= 0x01;
    TH0 = 0xFC;
    TL0 = 0x6F;
    ET0 = 1;
    EA = 1;
    TR0 = 1;
}

// 显存更新函数 
void Update_Display_Buffer(unsigned long num)
{
    unsigned char i;
    for (i = 0; i < 8; i++)
    {
        display_buf[7 - i] = num % 10;
        num /= 10;
    }
}

void Timer0_ISR(void) interrupt 1
{
    static unsigned char frac_acc = 0;
    static unsigned char digit = 0;

    // 计时补偿 
    frac_acc += 6;
    if (frac_acc >= 10)
    {
        frac_acc -= 10;
        TH0 = 0xFC; TL0 = 0x6E;
    }
    else
    {
        TH0 = 0xFC; TL0 = 0x6F;
    }

    // 仅计数，设置标志位
    ms_count++;
    if (ms_count >= 1000)
    {
        ms_count = 0;
        total_seconds++;
        update_flag = 1; // 通知主循环去计算，中断立刻退出
    }

    //  扫描逻辑 
    SMG_PORT = 0x00; 
    switch(digit)
    {
        case 0: LSC=1; LSB=1; LSA=1; break; // LED8
        case 1: LSC=1; LSB=1; LSA=0; break; // LED7
        case 2: LSC=1; LSB=0; LSA=1; break; // LED6
        case 3: LSC=1; LSB=0; LSA=0; break; // LED5
        case 4: LSC=0; LSB=1; LSA=1; break; // LED4
        case 5: LSC=0; LSB=1; LSA=0; break; // LED3
        case 6: LSC=0; LSB=0; LSA=1; break; // LED2
        case 7: LSC=0; LSB=0; LSA=0; break; // LED1
    }
    SMG_PORT = smg_code[display_buf[digit]];
    
    digit++;
    if (digit >= 8) digit = 0;
}

void main(void)
{
    Timer0_Init();
    while(1)
    {
        // 只有当秒数更新时，才在主循环里做耗时的除法运算
        if (update_flag == 1)
        {
            Update_Display_Buffer(total_seconds);
            update_flag = 0; // 清除标志
        }
    }
}