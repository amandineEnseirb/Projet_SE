#include <avr/io.h>
#include <avr/interrupt.h>
#define FOSC 16000000			// Clock Speed
#define BAUD 1000000
#define MYUBRR (FOSC/16/BAUD-1)
#define FIFO_SIZE 1024			//2K bytes de RAM
unsigned char fifo_to_send[FIFO_SIZE];
unsigned int head_fifo = 0, back_fifo = 0;
volatile bool is_capture_on = false;

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
	UCSR0B = _BV(TXEN0);
		
	UCSR0A &= ~(1 << U2X0); //Si mise à 0 du reste, erreur dans les baud rate

	/*
	 * Set frame format: 8data, 2stop bit 
	 */
	UCSR0C = (1 << USBS0) | (3 << UCSZ00);
}

bool USART_Transmit(unsigned char data)
{
	unsigned int tmp = (back_fifo + 1) & 0x3FF;//Remplace le modulo
	if(tmp == head_fifo)
		return false;
	fifo_to_send[back_fifo] = data;
	back_fifo = tmp;
	UCSR0B |= _BV(UDRIE0);		//Réactive les interruptions pour l'envois
	return true;
}

ISR(USART_UDRE_vect)
{
	UDR0 = fifo_to_send[head_fifo];
	head_fifo = (head_fifo + 1) & 0x3FF;//Remplace le modulo
	if(head_fifo == back_fifo)	//Disable interupt when fifo is empty
		UCSR0B &= ~(1 << UDRIE0);
}
ISR(PCINT0_vect)
{
	USART_Transmit(PINB);
}

int main()
{
	USART_Init(MYUBRR);
	PCICR |= _BV(PCIE0);		//Enable interrupt on PORTB
	PCMSK0 = (_BV(PCINT0) | _BV(PCINT1));
	/*PCMSK0 = ~(_BV(PCINT6) & _BV(PCINT7));*/
	
	//Enable intterrupt
	SREG = 0x80;
	while(1);
}
