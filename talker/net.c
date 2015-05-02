#include <avr/io.h>
#include "net.h"
#include "status.h"

// scan net for avaliable devices

void netScan()
{

}



// read value from device
uint8_t netRead(uint8_t address, uint8_t *data, uint8_t count)
{
  uint8_t c = 0;

  TWI_SEND_START();

  TWI_WAIT();

  SET_STATUS(S_TWI_START);

  TWDR = (address<<1) | 0x01; // address and read mode

  TWI_SEND_ACK();

  TWI_WAIT();

  SET_STATUS(S_TWI_ADDR);

  while ( c < count) // while there is data to recieve
    {
      if ( TWSR == 0x40 || TWSR == 0x50) // ack recieved
	{
	  if (c == count - 1) 
	    {
	      TWI_SEND_NACK();
	    }
	  else
	    {
	      TWI_SEND_ACK();
	    }
	  
	  TWI_WAIT();

	  SET_STATUS(S_TWI_READ);

	  data[c++] = TWDR; // read data, increment counter
	}
      else // nack recieved
	{
	  TWI_SEND_STOP();
	  return c; // return number of bytes read
	}
    }

  
  TWI_SEND_STOP();

  return c; // return number of bytes read

}

// write value to device
uint8_t netWrite(uint8_t address, uint8_t data)
{
  TWI_SEND_START();

  TWI_WAIT();
  
  TWDR = (address<<1); // address and write mode

  TWI_SEND_ACK();

  TWI_WAIT();

  if ( TWSR == 0x18 ) // sla+w, ack recieved
    {
      TWDR = data;

      TWI_SEND_NACK();

      TWI_WAIT();

      TWI_SEND_STOP();

      return 1; 
    }
  else
    {
      TWI_SEND_STOP();
      return 0;
    }

}

uint8_t netWriteString(uint8_t address, uint8_t *data)
{
  uint8_t dataCounter = 0;

  TWI_SEND_START();

  TWI_WAIT();
  
  TWDR = (address<<1); // address and write mode

  TWI_SEND_ACK(); // send address

  TWI_WAIT();

  while (data[dataCounter] != 0)
    {

      if ( TWSR == 0x18 || TWSR == 0x28 ) // sla+w, ack recieved
	{
	  TWDR = data[dataCounter++]; // send data, increment counter

	  TWI_SEND_ACK(); // send data

	  TWI_WAIT();
	}
      else
	{
	  TWI_SEND_STOP();
	  return 0;
	}
    }
  TWI_SEND_STOP();
  return 1;
}

/* netTest: 
   transmit address on bus, and reply
   true if decive exists. */
uint8_t netTest(uint8_t address)
{
  TWI_SEND_START();
  TWI_WAIT();
  TWDR = (address<<1); // address and write mode
  TWI_SEND_ACK();
  TWI_WAIT();

  if (TWSR == 0x18) // ACK recieved from slave
    {
      TWI_SEND_STOP();
      return 1;
    }
  else
    {
      TWI_SEND_STOP();
      return 0; 
    }
}
