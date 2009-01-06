#include <avr/interrupt.h>
#include <string.h>
#include <util/delay.h>
#include "bl0rg.h"

volatile uint32_t timerticks;
volatile uint16_t encoder_value = 0;
volatile uint16_t old_encoder_value = 0x03;

volatile uint8_t errflag = 0;

void InitTimer(void)
{
	TCCR0 = _BV(CS00);   /* CPU-Takt */
	TIMSK = _BV(TOIE0);
}

#define led_delay(t) { \
	for ( uint8_t i = 0; i < t; i++) asm volatile("nop\n\t"::);	\
	} while(0) 
	
#define HandleLED(ledbit) \
	PORTA = leds[0+ ledbit]; \
	PORTC = leds[11+ledbit]; \
	PORTB = leds[22+ledbit]; \
	

ISR (SIG_OVERFLOW0) 
{
	/* static uint8_t  encstat = 0, oencstat = 0; */
    static uint16_t ledbit;
    static uint16_t ledtimer;
								 
    timerticks++;   // Für andere Timeranwendungen

	/* check for encoder on pind 4 and 5 */


	/* here we check for dirty bits in the leds array */

/*
	DDRD = 0x00;
	encstat = (PIND >> 4) & 0x03;
	switch (oencstat) {
	case 0x00:
		if (encstat == 0x01) encoder_value++;
		if (encstat == 0x02) encoder_value--;
		break;
	case 0x01:
		if (encstat == 0x03) encoder_value++;
		if (encstat == 0x00) encoder_value--;
		break;
	case 0x02:
		if (encstat == 0x00) encoder_value++;
		if (encstat == 0x03) encoder_value--;
		break;
	case 0x03:
		if (encstat == 0x02) encoder_value++;
		if (encstat == 0x01) encoder_value--;
		break;
	}
	oencstat = encstat;
	
	DDRD = 0xff;

*/

	/* end of check for encoder */

	if (ledtimer == 0)
	{
		ledbit++;
		if (ledbit >= 11) ledbit = 0;
	
		switch(ledbit)
		{
			case 0: 
				HandleLED(0);
				led_delay(1);
			case 1: 
				HandleLED(1);
				led_delay(4);
			case 2: 
				HandleLED(2);
				led_delay(12);
			case 3: 
				HandleLED(3);
				ledtimer = 1;
				ledbit   = 3;				
				break;
			case 4: 
				HandleLED(4);
				ledtimer = 2;
				break;
			case 5: 
				HandleLED(5);			
				ledtimer = 4;
				break;
			case 6: 
				HandleLED(6);
				ledtimer = 8;
				break;
			case 7: 
				HandleLED(7);
				ledtimer = 16;
				break;
			case 8: 
				HandleLED(8);
				ledtimer = 32;
				break;
			case 9: 
				HandleLED(9);
				ledtimer = 64;
				break;
			case 10: 
				HandleLED(10);
				ledtimer = 128;
				break;
			case 11: 
				HandleLED(11);
				ledtimer = 256;
				break;
			case 12: 
				HandleLED(12);
				ledtimer = 512;
				break;
		}
	}
    ledtimer--;      
	
	if (leds_changed != 0) memcpy(leds,leds_copy,33);
	leds_changed = 0;
}
