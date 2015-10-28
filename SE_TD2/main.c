#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "SPI.h"

#define DELAY   1000
#define BAUD    9600
#define MY_UBRR ((F_CPU/16/BAUD)-1)

#define PIN_SPI_CS_DAC    PB0
#define PIN_SPI_LDAC_DAC  PB1

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

void init(void)
{
  DDRB |= _BV(PIN_SPI_CS_DAC)|_BV(PIN_SPI_LDAC_DAC);
  PORTB |= _BV(PIN_SPI_CS_DAC)|_BV(PIN_SPI_LDAC_DAC);
  
  SPI_MasterInit();
}

int main() {
  unsigned short data;
  init();
  
  data = ((_BV(4) & 0xF)<<8) & (0xFF);
  
  while(1)
    {
      data = ((_BV(4) & 0x8)<<8) & (0x00);
      PORTB &= ~_BV(PIN_SPI_CS_DAC); //chip select
      SPI_MasterTransmit(data>>8);
      SPI_MasterTransmit((char)data);
      PORTB |= _BV(PIN_SPI_CS_DAC); // chip deselect
      PORTB &= ~_BV(PIN_SPI_LDAC_DAC);
      PORTB |= _BV(PIN_SPI_LDAC_DAC);
      
      _delay_ms(5000);
      return;
    }
}
