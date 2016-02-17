#include "bufferdserial.h"

static const int LIMIT_SIZE=4096;
extern PwmOut led[4];

BufferedSerial::BufferedSerial(PinName tx,PinName rx)
:Serial(tx,rx)
{
  irq = UART1_IRQn;
  this->Serial::attach(this, &BufferedSerial::rx_isr, Serial::RxIrq);
  this->Serial::attach(this, &BufferedSerial::tx_isr, Serial::TxIrq);
}
void BufferedSerial::sendFrame(uint8_t *buffer,size_t size)
{
  //start cirtical section
  NVIC_DisableIRQ(irq);
  for(size_t i = 0; i< size; i++)
  {
    if(txBuffer.size() >= LIMIT_SIZE)
    {
    //enable the interrupt and wait till txBuffer is cleared by rx_irq
      NVIC_EnableIRQ(irq);
      while(txBuffer.size() >= LIMIT_SIZE){
        if(this->Serial::writeable())
        {
          uint8_t temp = txBuffer.front();
          txBuffer.pop();
          this->putc(temp);
        }        
      }
      //txBuffer has sapce,dont interrupt and transfer data
    //start cirtical section  
      NVIC_DisableIRQ(irq);
    }
    txBuffer.push(buffer[i]);
  }
  if(this->Serial::writeable() && !txBuffer.empty())
  {
    uint8_t temp = txBuffer.front();
    txBuffer.pop();
    this->putc(temp);
  }       
  //end critical sectino
  NVIC_EnableIRQ(irq);
  return;
}
void BufferedSerial::tx_isr(){
  while (this->Serial::writeable() && !txBuffer.empty()) {
    this->Serial::putc(txBuffer.front());
    txBuffer.pop();
  }
  return;
}
void BufferedSerial::recvFrame(uint8_t *buffer,size_t size){
  //start cirtical section
  NVIC_DisableIRQ(irq);
  for(size_t i = 0; i< size; i++)
  {
    if(rxBuffer.empty())
    {
    //enable the interrupt if rx is empty
      NVIC_EnableIRQ(irq);
      while(rxBuffer.empty()){
        led[3] = led[3] > 0.5 ? 0.01: led[3]+0.001;
      }
    //start cirtical section  
      NVIC_DisableIRQ(irq);
    }
    buffer[i] = rxBuffer.front();
    rxBuffer.pop();
  }
  //end critical sectino
  NVIC_EnableIRQ(irq);
  return;  
}
void BufferedSerial::rx_isr(){
  while(this->Serial::readable()){
    rxBuffer.push(this->getc());
  }
  return;
}