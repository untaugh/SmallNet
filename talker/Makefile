PRG            = talker
OBJ            = $(PRG).o net.c usart.c
MCU_TARGET     = atmega328p
OPTIMIZE       = -O2

DEFS           =
LIBS           = 
CC             = avr-gcc

override CFLAGS        = -g -Wall $(OPTIMIZE) -mmcu=$(MCU_TARGET) $(DEFS)
override LDFLAGS       = -Wl,-Map,$(PRG).map

OBJCOPY        = avr-objcopy
OBJDUMP        = avr-objdump

all: $(PRG).elf lst hex

$(PRG).elf: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)
	avr-size -C --mcu=$(MCU_TARGET) $(PRG).elf
main.o: main.c 

clean:
	rm -rf *.o $(PRG).elf *.eps *.png *.pdf *.bak 
	rm -rf *.lst *.map $(EXTRA_CLEAN_FILES)

lst:  $(PRG).lst

%.lst: %.elf
	$(OBJDUMP) -h -S $< > $@

text: hex bin srec

hex:  $(PRG).hex

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@


program:
	avrdude -v -p$(MCU_TARGET) -carduino -P/dev/ttyUSB0 -b57600 -D -Uflash:w:$(PRG).hex:i

#	avrdude -p $(MCU_TARGET) -c jtag2isp -P usb -U flash:w:$(PRG).hex
