#ifndef ADDONTINY_H
#define ADDONTINY_H

#define DEVICE_ADDRESS 11
#define DEVICE_TYPE TYPE_LIGHT

#define LED1 PB1
#define SCL  PB2
#define SDA  PB0

enum deviceType {
  TYPE_UNKNOWN, 
  TYPE_BUTTON,
  TYPE_LIGHT
};

#define LED_ON() { PORTB |= (1<<LED1); }
#define LED_OFF() { PORTB &= ~(1<<LED1); }

#define SDA_LOW() { DDRB |= (1<<SDA); PORTB &= ~(1<<SDA); }
#define SDA_IN() { DDRB &= ~(1<<SDA); PORTB &= ~(1<<SDA); }
#define SCL_LOW() { DDRB |= (1<<SCL); PORTB &= ~(1<<SCL); }
#define SCL_IN() { DDRB &= ~(1<<SCL); PORTB &= ~(1<<SCL); }

#define SDA_R (PINB & (1<<SDA))  // sda read
#define SCL_R (PINB & (1<<SCL))  // scl read

#endif // ADDONTINY_H
