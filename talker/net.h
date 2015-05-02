#ifndef NET_H
#define NET_H

#include <avr/io.h>
#include "usart.h"

#define TWI_SEND_START() { TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN)|(1<<TWEA); }

#define TWI_SEND_STOP() { TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN)|(1<<TWEA); }

#define TWI_WAIT() { while( !(TWCR & (1<<TWINT)) ); }

#define TWI_SEND_ACK() { TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA); }

#define TWI_SEND_NACK() { TWCR = (1<<TWINT)|(1<<TWEN); }

void netScan();
uint8_t netTest(uint8_t address);
uint8_t netRead(uint8_t address, uint8_t *data, uint8_t count);
uint8_t netWrite(uint8_t address, uint8_t data);
//uint8_t netWriteString(uint8_t address, uint8_t *data);

#endif // NET_H
