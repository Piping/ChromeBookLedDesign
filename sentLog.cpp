
#include <stdio.h>
#include "./RS-232/rs232.h"

//compile command: gcc sentLog.cpp ./RS-232/rs232.c -Wall - -o log-o2

extern char comports[38][16]; // defined in rs232.c

int openSerialPort(const char *port, int baudrate, const char *mode){
  for(int i = 0 ; i < 38; i++){
    if(strcmp(comports[i], port) == 0){
      if(RS232_OpenComport(i, baudrate, mode))
        return -1;
      return i;
    }
  }
  return(-1); 
} 

void transferFileToSerialPort(int file_fd, int port_num, int ms){
  char c;
  while( read(file_fd, &c, 1) == 1 ){
    RS232_SendByte(port_num, c);      
    fflush(stdout); //force the character on screen
    usleep(ms*1000); //gotta wait for led light
    RS232_PollComport(port_num,(unsigned char *)&c,1);
    printf("%c", c);
  }
  printf("End of file transfer\n");
}

int main(int argc, char* argv[]){
  char mode[] = "8N1";  
  int baud_rate = 9600;
  int port;
  int file_fd;

  if(argc < 3){
    printf("Invalid command line argument\nUsage: sudo ./log /dev/ttyACM0 filename\n"); 
    return 0;
  }
  if( (port = openSerialPort(argv[1], baud_rate, mode) ) == -1){;
    printf("fail to open serial port\n");
    return 0;
  }
  
  if( (file_fd = open(argv[2], O_RDWR)) == -1 ){  
    printf("Invalid filename\n");
    return 0;
  }
  
  transferFileToSerialPort(file_fd, port, 50*8);     

	return 0;
}