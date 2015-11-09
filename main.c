#include <avr/io.h>
#include <avr/interrupt.h>
#define FOSC 16000000// Clock Speed
#define BAUD 300 
#define MYUBRR FOSC/16/BAUD-1
#define FIFO_SIZE 200
#define LED_BOARD PB5
#define LED_1 PB3
#define LED_2 PB2
#define LED_3 PB1
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

ISR(USART_RX_vect) //Reception de donnée depuis le port serie
{
	/*//Disable interrupt*/
	SREG &= ~0x80;
	/*UDR0 = 'a';*/
	unsigned char tmp = UDR0;
	if(tmp == 'a')
	{
		PORTB ^= _BV(LED_BOARD);
		is_capture_on = !is_capture_on;
	}
		
	/*//Enable interrupt*/
	SREG |= 0x80;
}
ISR(USART_UDRE_vect)
{
	/*Disable interrupt*/
	SREG &= ~0x80;
	UDR0 = fifo_to_send[head_fifo];
	head_fifo = (head_fifo+1)%FIFO_SIZE;
	if(head_fifo == back_fifo) //Disable interupt when fifo is empty
		UCSR0B &= ~(1 << UDRIE0);
	PORTB ^= _BV(LED_1);
	/*//Enable interrupt*/
	SREG |= 0x80;
}
ISR(INT0_vect)
{
	/*Disable interrupt*/
	SREG &= ~0x80;
	/*UDR0 = fifo_to_send[head_fifo];*/
	/*head_fifo = (head_fifo+1)%FIFO_SIZE;*/
	/*if(head_fifo == back_fifo) //Disable interupt when fifo is empty*/
		/*UCSR0B &= ~(1 << UDRIE0);*/
	PORTB ^= _BV(LED_2);
	/*USART_Transmit('a');*/
	/*//Enable interrupt*/
	SREG |= 0x80;
}
/*bool volatile wait_otter;*/
/*ISR(TIMER1_COMPA_vect)*/
/*{*/
	/*SREG &= ~0x80;*/
	/*wait_otter = false;*/
	/*PRR |= _BV(PRTIM1); //To disable counter*/
	/*SREG |= 0x80;*/
/*}*/
/*void wait(unsigned short cycles)*/
/*{*/
	/*wait_otter = true;*/
	/*OCR1A = cycles;*/
	/*TCNT1 = 0;*/
	/*//Selectionner source (internal clock)*/
	/*TIMSK1 //Enable interrupt*/
	/*OCIE1A //enable -> to activate interrupt*/
	/*PRR &= ~_BV(PRTIM1); //To enable counter*/
	/*while(wait_otter);*/
/*}*/

int main()
{
	// Active et allume la broche PB5 (led)
	//Précise que l'on prend la main sur la broche de la led
	DDRB |= _BV(LED_BOARD);
	DDRB |= _BV(LED_3);
	DDRB |= _BV(LED_2);
	DDRB |= _BV(LED_1);
	DDRB |= _BV(DATA);
	DDRD |= _BV(CLOCK);//Digital pin 2 (INT0)

	USART_Init(MYUBRR);

	//Eteint les led
	PORTB &= ~_BV(LED_BOARD);//On board
	PORTB &= ~_BV(LED_1);
	PORTB &= ~_BV(LED_2);
	PORTB &= ~_BV(LED_3);

	//Enable external interrupt on INT0
	EIMSK |= _BV(INT0);
	//Interrupt on INT0 with raising level
	EICRA |= (_BV(ISC00) | _BV(ISC01));

	PORTD &= ~_BV(CLOCK); //Mettre l'horloge basse pendant au moins 10 ms
	for(int volatile j = 0; j < 10000; ++j);
	PORTB &= ~_BV(DATA); //Met les donnée à 0
	PORTD |= _BV(CLOCK); //Met la clock à 0
	DDRD &= ~_BV(CLOCK); //Relache la clock

	int i = 0;
	SREG = 0x80;
	while(1)
	{
		if(i == 0)
		{
			//Set la led
			/*PORTB ^= _BV(PB5);*/
			/*USART_Transmit_int(platypus);*/
			/*USART_Transmit(' ');*/
			/*if(is_capture_on)*/
				/*USART_Transmit_string("abcde ");*/
			/*USART_Transmit_int(MYUBRR);*/
			/*USART_Transmit(' ');*/

		}
		i = (i + 1) % FOSC;
	}
}

