#include <avr/io.h>
#include "addontiny.h"

typedef void *(*Function)();  // function pointer

uint8_t data;
uint8_t bit_counter; 

#define ADDRESS_READ ((DEVICE_ADDRESS << 1) | 1)
#define ADDRESS_WRITE (DEVICE_ADDRESS << 1)

// states
void *idle_high();
void *idle_sda();
void *idle_scl();
void *idle_low();

void *read_high();
void *read_scl();
void *read_sda();
void *read_low();

void *ack_mw_high();
void *ack_mw_scl();
void *ack_mw_sda();
void *ack_mw_low();
void *ack_mw_done();

void *ack_high();
void *ack_scl();
//void *ack_sda(); // should not happen
void *ack_low();
void *ack_done_scl();

void *read_mw_high();
void *read_mw_scl();
void *read_mw_sda();
void *read_mw_low();

void *write_high();
//void *write_scl();  // not used, no start/stop checking
//void *write_sda(); // not used, no start/stop checking
void *write_low();
void *write_last();

//void *ack_read_high(); // should not happen
//void *ack_read_scl();  // should not happen
void *ack_read_sda();
void *ack_read_low();


void *idle_high()
{
  while(1)
    {
      if (!SDA_R) // start condition
	{
	  data = 0;
	  bit_counter = 7; 
	  return read_scl;
	}
      if (!SCL_R) // negative clock
	{
	  return idle_sda;
	}
    }
}

void *idle_scl()
{
  while(1)
    {
      if (SDA_R) // stop condition
	{
	  return idle_high;
	}
      if (!SCL_R) // negative clock
	{
	  return idle_low;
	}
    }
}

void *idle_sda()
{
  while(1)
    {
      if (!SDA_R)
	{
	  return idle_low;
	}
      if (SCL_R) // positive clock
	{
	  return idle_high;
	}
    }
}

void *idle_low()
{
  while(1)
    {
      if (SDA_R)
	{
	  return idle_sda;
	}
      if (SCL_R) // positive clock
	{
	  return idle_scl;
	}
    }
}

void *read_low()
{
  while(1)
    {
      if (SCL_R) // positive clock
	{
	  if (!bit_counter--)
	    {
	      if (data == ADDRESS_READ)
		{
		  bit_counter = 7;  
		  data = TYPE_LIGHT; // first data to be read 
				  
		  return ack_scl; 
		}
	      else if (data == ADDRESS_WRITE)
		{
		  bit_counter = 7;
		  data = 0; 
		  return ack_mw_scl;
		}
	      return idle_scl;
	    }
	  return read_scl;
	}
      if (SDA_R)
	{
	  return read_sda; 
	}
    }

}

void *read_high()
{
  while(1)
    {
      if (!SCL_R) // negative clock
	{
	  return read_sda;
	}
      if (!SDA_R) // start condition, but not used as one
	{
	  return read_scl; 
	}
    }
}

void *read_scl()
{
  while(1)
    {
      if (!SCL_R) // negative clock
	{
	  return read_low;
	}
      if (SDA_R) // stop condition
	{
	  return read_high; 
	}
    }
}

void *read_sda()
{
  while(1)
    {
      if (SCL_R) // positive clock
	{
	  data |= (1<<bit_counter);
	  if (!bit_counter--)
	    {
	      if (data == ADDRESS_READ)
		{
		  bit_counter = 7;
		  data = TYPE_LIGHT; // first data to be read 
		  return ack_high; 
		}
	      else if (data == ADDRESS_WRITE)
		{
		  bit_counter = 7;
		  data = 0; 
		  return ack_mw_high;
		}
	      return idle_high;
	    }
	  return read_high;
	}
      if (!SDA_R) // data changed
	{
	  return read_low; 
	}
    }
}

void *ack_mw_high()
{
  while(1)
    {
      if (!SCL_R) // negative clock
	{
	  SDA_LOW(); // send ack
	  return ack_mw_low; 
	}
      if (!SDA_R) // start condition
	{
	  return idle_scl; 
	}
    }
}

void *ack_mw_scl()
{
  while(1)
    {
      if (!SCL_R) // negative clock
	{
	  SDA_LOW(); // send ack
	  return ack_mw_low; 
	}
      if (SDA_R) // stop condition
	{
	  return idle_high; 
	}
    }
}

void *ack_mw_sda()
{
  while(1)
    {
      if (!SCL_R) // positive clock
	{
	  return idle_high; // should not happen
	}
      if (!SDA_R) // start condition
	{
	  return idle_low; 
	}
    }
}

void *ack_mw_low()
{
  while(1)
    {
      if (SCL_R) // positive clock
	{
	  // ack is sampled here by master
	  return ack_mw_done; 
	}
    }
}
void *ack_mw_done()
{
  while(1)
    {
      if (!SCL_R) // negative clock
	{
	  SDA_IN();
	  return read_mw_sda;
	}
    }
}

void *ack_high()
{
  while(1)
    {
      if (!SCL_R) // negative clock
	{
	  SDA_LOW(); // send ack
	  return ack_low; 
	}
      if (!SDA_R) // start condition
	{
	  return idle_scl; 
	}
    }
}

void *ack_scl()
{
  while(1)
    {
      if (!SCL_R) // negative clock
	{
	  SDA_LOW(); // send ack
	  return ack_low; 
	}
      if (SDA_R) // stop condition
	{
	  return idle_high; 
	}
    }
}

void *ack_low()
{
  while(1)
    {
      if (SCL_R) // positive clock
	{
	  // ack is sampled here by master
	  return ack_done_scl; 
	}
    }
}

void *ack_done_scl()
{
  while(1)
    {
      if (!SCL_R) // negative clock
	{
	  if (data & (1<<7)) // first bit send
	    {
	      SDA_IN();
	      return write_low;
	    }
	  else
	    {
	      SDA_LOW();
	      return write_low; 
	    }
	}
    }
}

void *read_mw_low()
{
  while(1)
    {
      if (SCL_R) // positive clock
	{
	  if (!bit_counter--)
	    {
	      OCR0B = data; 
	      return idle_scl;
	    }
	  return read_mw_scl;
	}
      if (SDA_R)
	{
	  return read_mw_sda; 
	}
    }

}

void *read_mw_high()
{
  while(1)
    {
      if (!SCL_R) // negative clock
	{
	  return read_mw_sda;
	}
      if (!SDA_R) // start condition
	{
	  return read_mw_scl; 
	}
    }
}

void *read_mw_scl()
{
  while(1)
    {
      if (!SCL_R) // negative clock
	{
	  return read_mw_low;
	}
      if (SDA_R) // stop condition
	{
	  return read_mw_high; 
	}
    }
}

void *read_mw_sda()
{
  while(1)
    {
      if (SCL_R) // positive clock
	{
	  data |= (1<<bit_counter);
	  if (!bit_counter--)
	    {
	      OCR0B = data; 
	      return idle_high;
	    }
	  return read_mw_high;
	}
      if (!SDA_R) // data changed
	{
	  return read_mw_low; 
	}
    }
}

void *write_high()
{
  while(1)
    {
      if (!SCL_R) // negative clock
	{
	  if (data & (1<<bit_counter)) //  bit send
	    {
	      SDA_IN(); // send high bit
	      return write_low;
	    }
	  else
	    {
	      SDA_LOW();  // send low bit
	      return write_low;
	    }
	}
    }
}

void *write_low()
{
  while(1)
    {
      if (SCL_R) // positive clock, master sample
	{
	  if (!bit_counter--)
	    {
	      return write_last;
	    }
	  else
	    {
	      return write_high; 
	    }
	}
    }
}

void *write_last()
{
  while(1)
    {
      if (!SCL_R) // negative clock
	{
	  SDA_IN(); // release sda
	  return ack_read_sda;
	}
    }
}


void *ack_read_sda()
{
  while(1)
    {
      if (SCL_R) // positive clock, nack from master
	{
	  return idle_high;
	}
      if (!SDA_R)
	{
	  return ack_read_low;
	}
    }
}

void *ack_read_low()
{
  while(1)
    {
      if (SCL_R) // positive clock, ack from master
	{
	  data = OCR0B;  // this is the led value
	  bit_counter = 7;
	  return ack_done_scl;
	}
      if (SDA_R)
	{
	  return ack_read_sda;
	}
    }
}

int main()
{
  // init pwm
  TCCR0A |= (1<<COM0B1) | (1<<WGM01) | (1<<WGM00);  // fast pwm mode
  TCCR0B |= (1<<CS02); // prescaler 256
  OCR0B = 30; // initial value
  DDRB |= (1<<LED1); // port out
  
  Function func = idle_high; // first function
  
  while(1)
    {
      // run function that returns pointer to next function
      func = (Function)(*func)(); 
    }
}
