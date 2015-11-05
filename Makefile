# AVR-Geiger
# https://github.com/gcavallo/AVR-Geiger

# Copyright (c) 2015, Gabriel Cavallo
# GPLv3 License https://gnu.org/licenses/gpl.txt

DEVICE     = atmega328p
PROGRAMMER = usbasp
PORT       = /dev/ttyACM0
NAME       = main
OBJECTS    = $(NAME).o uart.o time.o
SRCDIR     = avr
OBJDIR     = avr/obj
BINDIR     = avr/bin

VPATH      = $(SRCDIR)
CC         = avr-gcc -mmcu=$(DEVICE)
CFLAGS     = -Wall -Wextra -Werror -Os
AVRDUDE    = avrdude -v -c $(PROGRAMMER) -p $(DEVICE) -P $(PORT) -b 57600 -D -U flash:w:$(BINDIR)/$(NAME).hex:i

all: $(BINDIR)/$(NAME).hex

$(OBJDIR)/%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

install: flash

clean:
	rm -f $(BINDIR)/$(NAME).hex $(BINDIR)/$(NAME).elf $(addprefix $(OBJDIR)/, $(OBJECTS))

flash:
	# Leonardo/atmega32u4: hard-reset into bootloader before flashing!
	$(AVRDUDE)

$(BINDIR)/$(NAME).elf: $(addprefix $(OBJDIR)/, $(OBJECTS))
	$(CC) -o $@ $^

$(BINDIR)/$(NAME).hex: $(BINDIR)/$(NAME).elf
	rm -f $(BINDIR)/$(NAME).hex
	avr-objcopy -O ihex -R .eeprom $(BINDIR)/$(NAME).elf $(BINDIR)/$(NAME).hex

debug: $(BINDIR)/$(NAME).elf
	avr-objdump -d $(BINDIR)/$(NAME).elf
