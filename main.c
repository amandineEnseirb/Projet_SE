#include <avr/io.h>
#include <avr/interrupt.h>
#define FOSC 16000000// Clock Speed
#define BAUD 300 
#define MYUBRR FOSC/16/BAUD-1
#define FIFO_SIZE 1024 //2K bytes de RAM
#define DATA PB0
#define CLOCK PD2
unsigned char fifo_to_send[FIFO_SIZE]; 
unsigned int head_fifo = 0, back_fifo = 0;
bool is_capture_on = false;
unsigned char accumulateur = 0, counter = 0;

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
ISR(USART_UDRE_vect)
{
	UDR0 = fifo_to_send[head_fifo];
	head_fifo = (head_fifo+1)%FIFO_SIZE;
	if(head_fifo == back_fifo) //Disable interupt when fifo is empty
		UCSR0B &= ~(1 << UDRIE0);
}
ISR(INT0_vect)
{
	if(PINB & _BV(DATA)) //On a 1 dans les datas
		accumulateur |= _BV(counter);
	else //On a un 0
		accumulateur &= ~_BV(counter);
	++counter;
	if(counter == 8)
	{
		counter = 0;
		USART_Transmit(accumulateur);//On ship le block de huit
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

