#include <avr/io.h>
#define F_CPU 16000000
#define BAUD 9600
#include <util/setbaud.h>
#include <util/delay.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "talker.h"
#include "net.h"
#include "usart.h"
#include "status.h"

int main()
{
  // init usart
  UBRR0H = UBRRH_VALUE;
  UBRR0L = UBRRL_VALUE;
  UCSR0B |= (1<<TXEN0)|(1<<RXEN0); // enable reciever and transmitter

  // init twi
  TWBR = 72; // 72=100kHz

  // init status leds
  INIT_STATUS();
  
  uint8_t c; // used for loops

  uint8_t buffer[100], cmd[10];

  for (c=0; c<100; c++) // clean buffer
    {
      buffer[c] = 0;
    }
  
  while(1)
    {
      SET_STATUS(S_START);

      for (c=0; c<100; c++) // clean buffer
    {
      buffer[c] = 0;
    }

      for (c=0; c<10; c++)
	{
	  cmd[c] = 0; 
	}
      
      usartReadLine(buffer, 100);  // read line

      SET_STATUS(S_USART_READ);

      //usartPrintLn(buffer, 100);

      strToCmd(buffer, cmd);  // translate line to commands

      //usartByte(cmd[0]); usartSend(' ');
      //usartByte(cmd[1]); usartSend(' ');
      //usartByte(cmd[2]); usartSend(' ');
      //usartSend('\n');

      uint8_t command = cmd[0];
      uint8_t address = cmd[1];
      uint8_t data = cmd[2];

      uint8_t readBuffer[5]; // used for storing return values form devices

      SET_STATUS(S_USART_CMD);


      switch(command)
	{
	case('r'): // read data from device, also reads type
	  SET_STATUS(S_READ_NET);
	  if (netRead(address, readBuffer, 2))
	    {
	      usartByte(readBuffer[0]); // type of devic
	      usartSend(' '); // new line
	      usartByte(readBuffer[1]); // value from device
	      usartSend('\n'); // new line
	    }
	  else
	    {
	      usartSend('0');
	      usartSend('\n'); // new line
	    }
	  break;
	case('w'): // write data to device
	  SET_STATUS(S_WRITE_NET);
	  netWrite(address, data);
	  usartSend('0');
	  usartSend('\n'); // new line
	  break;
	case('l'): // list devices on net
	  for (c=1; c<128; c++) // test all addresses for reply
	    {
	      if (netTest(c))
		{
		  usartByte(c);  // print device address
		  usartSend(' '); // separate with space
		}
	    }
	  usartSend('\n'); // new line
	  break;
	default:
	  usartSend('0');
	  usartSend('\n'); // new line
	  break;
	}
    } 
}


/* strToInt:
   convert a string to int. 
   will be padded with zeroes. */
uint8_t strToInt(char *str)
{
  uint8_t c = 0, sum = 0;

  while (str[c] != 0)
    {

      if (isdigit(str[c]))
	{
	  sum = sum * 10; // move old value with one decimal if more to come	
	  sum += str[c] - 48; // character from string to int
	}
      c++;
    }
  
  return sum; 
}

void strToCmd(uint8_t *str, uint8_t *cmd)
{
  char * tok;
  uint8_t cmd_counter = 0; 
  
  if ((tok = strtok(str," ")))
    {
      cmd[cmd_counter++] = tok[0]; // first token 

      while ((tok = strtok(NULL," ")))
	{
	  cmd[cmd_counter++] = atoi(tok); // more tokens 
	}
    }

}

/* strtoCmd:
   Parse command from string,
   first argument is a letter,
   followed by two bytes. */
/*void strToCmd(uint8_t *str, uint8_t *cmd)
{
  uint8_t string_count = 0;
  char temp[10]; 

  uint8_t i = 0;

  uint8_t cmd_count = 0;

  cmd[cmd_count++] = str[string_count++];
  
  while(str[string_count] != 0 && cmd_count < 3)
    {

      while(isdigit(str[string_count]))
	{
	  temp[i++] = str[string_count++]; // read numbers from string
	}
      
      if (i > 0) // digit was read in
	{
	  temp[i] = 0;  // null terminate
	  //cmd[cmd_count++] = strToInt(temp);  // store command
	  cmd[cmd_count++] = atoi(temp);  // store command
	  i = 0; // prepare for next command
	}
      string_count++;
    }
}
*/
