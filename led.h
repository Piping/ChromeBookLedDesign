#ifndef MBED_LED_H
#define MBED_LED_H

#include "mbed.h"

void blink(PwmOut& led,unsigned int ms,bool one);
void blink_char(PwmOut& led,unsigned int ms,char c);
void blink_str(PwmOut& led,unsigned int ms,char* str,int size);
void sync(PwmOut& led,unsigned int ms);

#endif