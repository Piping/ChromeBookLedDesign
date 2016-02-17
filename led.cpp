#include "led.h"

#define ON 1;
#define OFF 0; 

//define bit from right to letf(most significant)
//only aviable on this file linkage-wise
static const unsigned bitInterval = 50;
static const unsigned syncInterval = bitInterval*8 + 100;
PwmOut led[4] = 
{
    PwmOut(LED1), 
    PwmOut(LED2), 
    PwmOut(LED3), 
    PwmOut(LED4)
};
void sync(PwmOut& led,unsigned int ms)
{
    led = ON;
    wait_ms(ms);
    led = OFF;
}
void blink(PwmOut& led,unsigned int ms,bool one)
{
    if(one)
    {   
        led = ON;
        wait_ms(ms);
        led = OFF;
    }
    else
    {
        wait_ms(ms);
    }
}
void blink_char(PwmOut& led,unsigned int ms,char c)
{
    for(int i = 0; i<8 ; i++){
        blink(led,ms,c & (1<<i));
    }
}
void blink_str(PwmOut& led,unsigned int ms,char* str,int size)
{
    for(int i=0; i < size; i++)
    {
        blink_char(led,ms,str[i]);
    }
}