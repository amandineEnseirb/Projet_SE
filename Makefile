all: install

a.out: main.c
	avr-g++ -Os -mmcu=atmega328p main.c

a.bin:a.out
	avr-objcopy a.out a.bin

install:a.bin
	avrdude -c arduino -P /dev/ttyACM0 -p m328p -U flash:w:a.bin

clean:
	rm -f a.out a.bin

