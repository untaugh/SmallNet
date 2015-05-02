#ifndef USART_H
#define USART_H

#include <avr/io.h>

void usartSend(uint8_t d);
void usartByte(uint8_t b);
void usartReadLine(uint8_t *buffer, uint8_t length);
void usartPrintLn(uint8_t * line, uint8_t n);

#endif // USART_H
