#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define DELAY 1000
#define BAUD 9600
#define MY_UBRR ((F_CPU/16/BAUD)-1)

void turnON(void)
{
  PORTB |= _BV(PB5);
}

void turnOFF(void)
{
  PORTB &= ~_BV(PB5);
}

unsigned char displayString(unsigned char * string)
{
  int ind = 0;
  if(string != 0)
    {
      while(string[ind] != '\0')
      {
        if(UCSR0A & _BV(UDRE0))
          {
            UDR0 = string[ind];
            ind++;
          }
      }

      return 1;
    }
  else
    return 0;
}

unsigned char displayNumber(unsigned char number)
{
  unsigned char buf[6], cent, diz, uni;

  cent = number/100;
  diz = (number - cent*100)/10;
  uni = (number - cent*100 - diz*10);
 
  if(cent)
    {
      buf[0] = cent + '0';
      buf[1] = diz + '0';
      buf[2] = uni + '0';
      buf[3] = '\0';
    }
  else
    if(diz)
      {
	buf[0] = diz + '0';
	buf[1] = uni + '0';
	buf[2] = '\0';
      }
    else
      {
	buf[0] = uni + '0';
	buf[1] = '\0';
      }
  

  return displayString(buf);
}

void launchADC()
{
  ADCSRA |= _BV(ADSC);
}

unsigned char getADC()
{
  while(ADCSRA & _BV(ADIF))
    ;

  ADCSRA |= _BV(ADIF);
  return ADCH;
}

void init(void)
{
  DDRB |= _BV(PB5);
  turnOFF(); //init led at 0
  
  /* Set Baud rate */
  UBRR0H = (unsigned char)(MY_UBRR >> 8);
  UBRR0L = (unsigned char)(MY_UBRR);
  UCSR0A = (0 << U2X0);
  UCSR0B = _BV(RXEN0) | _BV(TXEN0);
  UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); // asynchronous mode + no parity bit + 1 stop bit + 8 bit of data

  /* set ADC */
  ADMUX = _BV(ADLAR);
  ADCSRA = _BV(ADEN) | _BV(ADIE) | 0x07;
  
}

ISR(ADC_vect)
{
  SREG &= ~0x80;
  displayNumber(ADCH);
  displayString("\n\r");
  launchADC();
  SREG |= 0x80;
}

int main() {
  unsigned char car, new_data = 0;
  init();

  displayString("Coucou???\n\r");
  while(!(UCSR0A & _BV(RXC0)))
    ;
  displayString("Salut!!!\n\r");
  turnON();
  launchADC();
  SREG = 0x80;
  
  while(1)
    {
     
      volatile unsigned int var = 50000;
      while(var>0)
        var--;
    
      PORTB = PORTB ^ _BV(PB5);
     
      //launchADC();
      //displayNumber(getADC());
      //displayString("\n\r");

      /* return 0;

      
      if(car == '1')
	turnON();
      else
	if(car == '0')
	  turnOFF();

      // data = getData();

      if((new_data == 1) && (UCSR0A & _BV(UDRE0)))
	{
	  if(car == '\n' || car == '\r')
	    {
	      UDR0 = '\n';
	      while(!(UCSR0A & _BV(UDRE0)))
		{
		  ;
		}
	      UDR0 = '\r';
	    }
	  else
	    UDR0 = car;

	  new_data = 0;
	  }*/
    }
}
