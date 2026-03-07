#include <reg52.h>
#include <string.h>

typedef unsigned char u8;
typedef unsigned int u16;

#define fosc 11059200L
#define baud 9600

sbit led = P2^0;

u8 rx_buf[20];
u8 rx_idx = 0;
bit cmd_ready = 0;

void uart_init()
{
    SCON = 0x50;
    TMOD &= 0x0F;
    TMOD |= 0x20;
    TH1 = TL1 = -(fosc/12/32/baud);
    TR1 = 1;
    ES = 1;
    EA = 1;
}

void uart_send_string(char *s)
{
    while(*s)
    {
        SBUF = *s++;
        while(!TI);
        TI = 0;
    }
}

void main()
{
    u16 send_timer = 0;
    
    uart_init();
    
    while(1)
    {
        send_timer++;
        if(send_timer >= 50000)
        {
            uart_send_string("Hello 51!\r\n");
            send_timer = 0;
        }

        if(cmd_ready)
        {
            if(strstr(rx_buf, "LED_ON") != NULL)
            {
                led = 0;
            }
            else if(strstr(rx_buf, "LED_OFF") != NULL)
            {
                led = 1;
            }
            
            memset(rx_buf, 0, sizeof(rx_buf));
            rx_idx = 0;
            cmd_ready = 0;
        }
    }
}

void uart_isr() interrupt 4
{
    u8 ch;
    if(RI)
    {
        RI = 0;
        ch = SBUF;
        if(ch == '\n' || ch == '\r')
        {
            if(rx_idx > 0)
            {
                rx_buf[rx_idx] = '\0';
                cmd_ready = 1;
            }
        }
        else
        {
            if(rx_idx < 19)
            {
                rx_buf[rx_idx++] = ch;
            }
        }
    }
}