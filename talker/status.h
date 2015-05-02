#ifndef STATUS_H
#define STATUS_H

/* status:
   Show status on leds, 
   values 0 to 15. */
enum status {
  S_OFF,
  S_START,
  S_USART_CMD,
  S_USART_READ,
  S_READ_NET,
  S_WRITE_NET,
  S_TWI_START,
  S_TWI_ADDR,
  S_TWI_READ,
};

#define INIT_STATUS() DDRB |= 0x0F
#define SET_STATUS(status) PORTB = status & 0x0F

#endif // STATUS_H
