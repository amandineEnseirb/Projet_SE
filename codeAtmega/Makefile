all: install

loutre: main.c
	avr-g++ -Os -mmcu=atmega328p main.c -o loutre

loutre.bin:loutre
	avr-objcopy loutre loutre.bin

install:loutre.bin
	avrdude -c arduino -P /dev/ttyACM0 -p m328p -U flash:w:loutre.bin

compile:loutre

clean:
	rm -f a.out a.bin

