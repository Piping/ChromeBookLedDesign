#ifndef MBED_BUFFERED_SERIAL_H
#define MBED_BUFFERED_SERIAL_H

#include "mbed.h"
#include "rtos.h"

#include <queue>

class BufferedSerial : public Serial
{
public:
  BufferedSerial(PinName tx,PinName rx);
  void sendFrame(uint8_t *buffer,size_t size);
  void recvFrame(uint8_t *buffer,size_t size);
private:
  void tx_isr();
  void rx_isr();
  IRQn irq; // interrupt request line for disable interrupt
  std::queue<uint8_t> txBuffer;
  std::queue<uint8_t> rxBuffer;
  uint8_t txLine[128];
  uint8_t rxLine[128];
};


#endif