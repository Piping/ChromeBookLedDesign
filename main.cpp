#include "mbed.h"
#include "rtos.h"
#include "led.h"
#include "bufferdserial.h"
#include <stdio.h>
#include <string.h>

const unsigned BUFFERSIZE = 4096;
//BufferedSerial gtkterm(USBTX, USBRX);
//BufferedSerial gtkterm(p13,p14);  // UART1 - p13, p14
// Line buffers for sprintf and sscanf
char data[512]="a\r\n";
 

 
BufferedSerial pc(USBTX, USBRX); // tx, rx
extern PwmOut led[4];

void serial_buffer_thread(const void *args)
{
  char str[]="Serial Worker started\r\n";
  pc.sendFrame((uint8_t*)str,strlen(str));
  while(1)
  {
    pc.recvFrame((uint8_t*)data,512);
    pc.sendFrame((uint8_t*)data,strlen(data));
  }
}
 
void led_control_thread(const void *args)
{
  pc.printf("Press 'w' to turn LED1 brightness up, 's' to turn it down\r\n");
  pc.printf("Use number 1-4 to select LED\r\n");
  float brightness[4]{0.1};
  int select = 1;
  while(1) 
  {
    char c;
    pc.recvFrame((uint8_t*)&c,1);
    switch (c)
    {
      case 'w' :
        if((c == 'w') && (brightness[select-1] < 0.5)) {
            brightness[select-1] += 0.01;
            led[select-1] = brightness[select-1];
        }
        break;
      case 's' :
        if((c == 's') && (brightness[select-1] > 0.0)) {
            brightness[select-1] -= 0.01;
            led[select-1] = brightness[select-1];
        } 
        break;
      case '1':
        select = 1;
        break;
      case '2':
        select = 2;
        break;
      case '3':
        select = 3;
        break;
      case '4':
        select = 4;
        break;                    
    }
  }
} 
 
int main() {
  Thread led_worker(led_control_thread);
  Thread serial_worker(serial_buffer_thread);
  while(1){led[0] = led[0] > 0.9 ? 0.01: led[0]+0.0001;}
}

