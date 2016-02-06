#include "mbed.h"

#define ON 1;
#define OFF 0; 

void blink(DigitalOut& led,unsigned int timeInterval,bool one);
void blink_char(DigitalOut& led,unsigned int timeInterval,char c);
void blink_str(DigitalOut& led,unsigned int timeInterval,char* str);
void sync(DigitalOut& led,unsigned int timeInterval);

//define bit from right to letf(most significant)
const char BIT[8] = { 0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

char* testString = "12345678ABCDEGHI";
DigitalOut led(LED1);

void blink(DigitalOut& led,unsigned int timeInterval,bool one)
{
    if(one)
    {   
        led = ON;
        wait_ms(timeInterval);
        led = OFF;
    }
    else
    {
        wait_ms(timeInterval);
    }
}
void blink_char(DigitalOut& led,unsigned int timeInterval,char c)
{
    for(int i = 0; i<8 ; i++){
        blink(led,timeInterval,c & BIT[i]);
    }
}
void blink_str(DigitalOut& led,unsigned int timeInterval,char* str)
{
    for(int i=0; str[i]!= '\0'; i++)
    {
        blink_char(led,timeInterval,str[i]);
    }
}
void sync(DigitalOut& led,unsigned int timeInterval)
{
    led = ON;
    wait_ms(timeInterval);
    led = OFF;
}


int main(void) {
    while(true)
    {
        sync(led,1000);//1000ms = 1s
        blink_str(led,100,testString); // 100ms = 0.1s
        sync(led,1000); 
    }
    return 0;
}