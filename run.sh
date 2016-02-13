#/bin/bash

make
cp Blink.bin /media/declan/MBED/
sudo ./log /dev/ttyACM0 main.cpp
