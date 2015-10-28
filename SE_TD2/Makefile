all : install

%.o : %.c
	avr-gcc $^ -mmcu=atmega328p -Os -c -o $@

test.elf : main.c SPI.o
	avr-gcc $^ -mmcu=atmega328p -Os -o $@

test.bin : test.elf
	avr-objcopy -O binary $^ $@

install: test.bin
	sudo avrdude -p ATmega328p -P /dev/ttyACM0 -c arduino -U flash:w:test.bin

show: test.elf
	avr-objdump -d test.elf

