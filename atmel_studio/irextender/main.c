/*
 * irextender.c
 *
 
 

                                           VCC
                                    10k     +                VCC
                                    ___     |   ||            +
                             /RES.-|___|----o---||----.       |
   TSOP             11,0592 MHz  |          |   ||    |       |
   36kHz                _        |   __     |        ===      V ->
    +--+               | |       '-o|  |o---'        GND      -
    |  o-------+ +--o-|| ||-o------o|  |o-----+               |  LD274
   (   o----+  | |  |  |_|  |  .---o|  |o---. |              .-.
    |  |    |  | | ---     --- | .-o|__|o---)-)-.        150R| |
    |  o--. |  | | ---     --- | |          | | |            | |
    +--+  | |  | |  |       |  | |          | | |            '-'
          | |  | |  |       |  | |  ATTINY45| | |             |
          | |  '-)--o-------o--)-|          | | |PB0  ___   |/
          | |    |             | |          | | '----|___|--|  BC547
          | |    '-------------' |     PB1  | |       1k    |>
          | '--------------------)----------' |               |
          '----------------------)------------'              ===
                                 |     PB2                   GND
                                ===
                                GND
 
 
SELFPRGEN = [ ]
RSTDISBL = [ ]
DWEN = [ ]
SPIEN = [X]
WDTON = [ ]
EESAVE = [ ]
BODLEVEL = DISABLED
CKDIV8 = [ ]
CKOUT = [ ]
SUT_CKSEL = EXTXOSC_8MHZ_XX_258CK_14CK_65MS

EXTENDED = 0xFF (valid)
HIGH = 0xDF (valid)
LOW = 0xDE (valid)

 
 * Created: 28.11.2015 13:33:54
 * Author : hilchenbachc
 */ 

#define F_CPU 11059200

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int main(void)
{
	
	DDRB = (1<<PB0) | (1<<PB1);
	
	// Timer 0 konfigurieren
	// Mode 2 - CTC
	// Prescaler 1
	// toggle OC0A
	
	PORTB |= (1<<PB1); // switch ir-receiver on 

	TCCR0A = /*(0 << COM0A1) | (1 << COM0A0) | */ (1 << WGM01) | (0 << WGM00) ;
	TCCR0B = (0 << WGM02 ) | (0 << CS02)| (0 << CS01)| (1 << CS00);
	OCR0A = 153;  // 72282 Hz ==> approx. 36 kHz at PB0
	
	while(1)
	{
		if( (PINB & (1<<PB2)) == 0 )  /* signal from ir remote control ? */
		{
			TCCR0A |= (0 << COM0A1) | (1 << COM0A0);
		}
		else
		{
			TCCR0A &= ~((0 << COM0A1) | (1 << COM0A0));  /* disconnect OC0A */
			PORTB &= ~(1<<PB0);
		}
	}
}
