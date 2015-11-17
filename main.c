#include <avr/io.h>
#include <avr/interrupt.h>
#define FOSC 16000000// Clock Speed
#define BAUD 300 
#define MYUBRR FOSC/16/BAUD-1
#define FIFO_SIZE 1024 //2K bytes de RAM
#define LED_BOARD PB5
#define LED_1 PB1
#define LED_2 PB2
#define LED_3 PB3
#define LED_4 PB4
#define DATA PB0
#define CLOCK PD2
int platypus = 0;
unsigned char fifo_to_send[FIFO_SIZE]; 
unsigned int head_fifo = 0, back_fifo = 0;
bool is_capture_on = false;

//screen /dev/ttyACM0 300,8N1
void USART_Init(unsigned int ubrr)
{
	/*
	 * Set baud rate 
	 */
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char)ubrr;
	/*
	 * Enable receiver and transmitter 
	 */
	UCSR0B = (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0);
	/*
	 * Set frame format: 8data, 2stop bit 
	 */
	UCSR0C = (1 << USBS0) | (3 << UCSZ00);
}

bool USART_Transmit(unsigned char data)
{
	unsigned int tmp = (back_fifo+1)%FIFO_SIZE;
	if(tmp == head_fifo)
		return false;
	fifo_to_send[back_fifo] = data;
	back_fifo = tmp;
	UCSR0B |= _BV(UDRIE0); //Réactive les interruptions pour l'envois
	return true;
}

void USART_Transmit_string(char *s)	// A tester
{
	while(*s > 15)
		USART_Transmit(*(s++));
}

void USART_Transmit_int(int n)	// testee
{
	int tmp = n;
	int pow_10 = 1;
	while(tmp > 10)
	{
		tmp /= 10;
		pow_10 *= 10;
	}
	while(pow_10)
	{
		USART_Transmit('0' + (n / pow_10) % 10);
		pow_10 /= 10;
	}
}
ISR(USART_UDRE_vect)
{
	UDR0 = fifo_to_send[head_fifo];
	head_fifo = (head_fifo+1)%FIFO_SIZE;
	if(head_fifo == back_fifo) //Disable interupt when fifo is empty
		UCSR0B &= ~(1 << UDRIE0);
}
unsigned char accumulateur = 0, counter = 0;
bool is_accumulating = false;
bool oddity;
ISR(INT0_vect)
{
	if(!is_accumulating && !(PINB & _BV(DATA))) //Start bit qui est toujours à 0
	{
		is_accumulating = true;
		PORTB ^= _BV(LED_BOARD);
	}
	else if(is_accumulating && counter < 8)
	{
		if(PINB & _BV(DATA)) //On a 1 dans les datas
			accumulateur |= _BV(counter);
		else
			accumulateur &= ~_BV(counter);
		++counter;
	}
	else if(is_accumulating && counter == 8)
	{
		oddity = (PINB & _BV(DATA));
		++counter;
		USART_Transmit(' ');//Ship notre data
		USART_Transmit_int(accumulateur);
		USART_Transmit(' ');//Ship notre data
			
		PORTB ^= _BV(LED_1);
	}
	else if(is_accumulating && counter == 9)
	{
		is_accumulating = false;
		counter = 0;
		if(!(PINB & _BV(DATA))) //Stop bit, doit être égale à 1
			PORTB ^= _BV(LED_3);
	}
}
int main()
{
	USART_Init(MYUBRR);

	//Enable external interrupt on INT0
	EIMSK |= _BV(INT0);

	//Interrupt on INT0 with falling level
	EICRA |= _BV(ISC01);
	EICRA &= ~_BV(ISC00);

	//Enable intterrupt
	SREG = 0x80;
	while(1);
}

