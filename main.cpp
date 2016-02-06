#include "mbed.h"
#include "./RS-232/rs232.h"

#include <stdio.h>

#define ON 1;
#define OFF 0; 

void blink(DigitalOut& led,unsigned int ms,bool one);
void blink_char(DigitalOut& led,unsigned int ms,char c);
void blink_str(DigitalOut& led,unsigned int ms,char* str);
void sync(DigitalOut& led,unsigned int ms);
void setupUSB(Serial &host,int baudrate,int format_bits,mbed::SerialBase::Parity format_pari, int format_stopBit);

//define bit from right to letf(most significant)
const char BIT[8] = { 0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
const unsigned byteInterval = 50;
const unsigned syncInterval = 200;
const unsigned BUFFERSIZE = 4096;
char testString[] = "12345678ABCDEGHI";
char buffer[4096];

DigitalOut led(LED1);

Serial pc(USBTX, USBRX);

void blinkBuffer(char * buffer)
{
    sync(led,syncInterval);//1000ms = 1s
    blink_str(led,byteInterval,buffer); // 100ms = 0.1s
    sync(led,syncInterval);  
}
int main(void) 
{

    while(true)
    {
      pc.printf("Idle\r\n");
      wait_ms(1000);
      if(pc.readable()) // more background knowledge is needed
      {
        pc.printf("Start Reading One File\r\n"); 
        while(pc.gets(buffer,BUFFERSIZE))
        {
          pc.printf("I got '%s'\r\n", buffer);
          blink_str(led,byteInterval,buffer); // this will last a few seconds
        }
        pc.printf("Finish Reading One File\r\n"); 
      }
    }
    return 0;
}
void setupUSB(Serial &host,int baudrate,int format_bits,mbed::SerialBase::Parity format_pari, int format_stopBit)
{
    host.baud(baudrate);
    host.format(format_bits,format_pari,format_stopBit);
}

void sync(DigitalOut& led,unsigned int ms)
{
    led = ON;
    wait_ms(ms);
    led = OFF;
}
void blink(DigitalOut& led,unsigned int ms,bool one)
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
void blink_char(DigitalOut& led,unsigned int ms,char c)
{
    for(int i = 0; i<8 ; i++){
        blink(led,ms,c & BIT[i]);
    }
}
void blink_str(DigitalOut& led,unsigned int ms,char* str)
{
    for(int i=0; str[i]!= '\0'; i++)
    {
        blink_char(led,ms,str[i]);
    }
}
