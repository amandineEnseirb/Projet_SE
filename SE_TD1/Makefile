all : install

test.elf : main.c 
	avr-gcc main.c -mmcu=atmega328p -Os -o test.elf

test.bin : test.elf
	avr-objcopy -O binary test.elf test.bin

install: test.bin
	sudo avrdude -p ATmega328p -P /dev/ttyACM0 -c arduino -U flash:w:test.bin

show: test.elf
	avr-objdump -d test.elf

