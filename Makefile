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

help:
	@fgrep -h "##" $(MAKEFILE_LIST) | fgrep -v fgrep | sed -e 's/\\$$//' | sed -e 's/##//'

install: ## install daemon on this system.
	cp daemon/geiger /usr/local/bin && chmod 755 /usr/local/bin/geiger
ifneq ($(wildcard /usr/share/geiger),)
	$(info "/usr/share/geiger" exists)
else
	mkdir -p /usr/share/geiger && chmod 755 /usr/share/geiger
endif
ifneq ($(wildcard /usr/share/geiger/geiger.conf),)
	$(info "/usr/share/geiger/geiger.conf" exists)
else
	cp daemon/geiger.conf /usr/share/geiger && chmod 640 /usr/share/geiger/geiger.conf
endif
ifneq ($(wildcard /usr/share/geiger/geiger.rrd),)
	$(info "/usr/share/geiger/geiger.rrd" exists)
else
	rrdtool create "/usr/share/geiger/geiger.rrd" --no-overwrite --step=1 \
	DS:cpm:GAUGE:1m:0:U RRA:AVERAGE:0.5:1m:1w RRA:AVERAGE:0.5:1h:1y \
	&& chmod 640 /usr/share/geiger/geiger.conf
endif
ifneq ($(wildcard /etc/systemd/system),)
	cp daemon/geiger.service /etc/systemd/system && chmod 644 /etc/systemd/system/geiger.service
	@echo Enable daemon with \'systemctl enable geiger.service\' as root!
else
	$(warning Systemd not found!)
endif

uninstall: ## remove all files from system including database and configs.
uninstall: clean
	rm -rf /usr/share/geiger
	rm -f /usr/lib/systemd/user/geiger.service

clean: ## remove binary files for reinstall (keep database and config).
	rm -f $(BINDIR)/$(NAME).hex $(BINDIR)/$(NAME).elf $(addprefix $(OBJDIR)/, $(OBJECTS))
	rm -f /usr/local/bin/geiger

flash: ## flash microcontroller with an AVR programmer.
	$(AVRDUDE)

dump: ## dump AVR binary for debug.
	avr-objdump -d $(BINDIR)/$(NAME).elf

$(OBJDIR)/%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

$(BINDIR)/$(NAME).elf: $(addprefix $(OBJDIR)/, $(OBJECTS))
	$(CC) -o $@ $^

$(BINDIR)/$(NAME).hex: $(BINDIR)/$(NAME).elf
	avr-objcopy -O ihex -R .eeprom $(BINDIR)/$(NAME).elf $(BINDIR)/$(NAME).hex
