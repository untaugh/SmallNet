#include "usart.h"

void usartSend(uint8_t d)
{
  while (!(UCSR0A & (1<<UDRE0))); // wait data register
  UDR0 = d; // write data
}

void usartByte(uint8_t b)
{
  usartSend(48 + b/100);
  usartSend(48 + (b/10)%10);
  usartSend(48 + b%10);
}

void usartPrintLn(uint8_t * line, uint8_t n)
{
  uint8_t c_counter = 0;

  uint8_t c = line[c_counter++]; 

  while (c_counter < n && c != 0)
    {
      usartSend(c);
      c = line[c_counter++]; 
    }

  usartSend('\n');	
}

void usartReadLine(uint8_t *buffer, uint8_t length)
{
  uint8_t c = 0;  // data counter
  
  while (c < length && buffer[c] != '\n')
    {
      while (!(UCSR0A & (1<<RXC0))); // wait for data recieve
      buffer[c] = UDR0; // recieved data

      //usartSend(buffer[c]);

      if (buffer[c] == '\n' || buffer[c] == '\r') // look for new line
	{
	  buffer[c] = 0;
	  break; // end of line reached
	}
      c++; // increment
    }
}
