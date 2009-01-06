#include <avr/interrupt.h>
#include <inttypes.h>
#include <avr/io.h>

#include "bl0rg.h"

volatile uint8_t leds[33];
volatile uint8_t leds_copy[33];
volatile uint8_t leds_changed;

void SetLed(uint8_t index, uint16_t value)
{
	while (leds_changed == 1) ;
	//value *= 8;
	if (value > 32) value = (value*value)/32;
	if (value > 0x07ff) value = 0x07ff;
	uint8_t sub_index = index / 8;
	for (uint8_t i = 0; i < 11; i++)
	{
		if (value  & _BV(i))
		{
			leds_copy[sub_index*11+i] |=  _BV(index % 8);
		} else {
			leds_copy[sub_index*11+i] &= ~_BV(index % 8);
		}
	}
	leds_changed = 1;
}


/*
inline void HandleLED(uint8_t ledbit)
{
	PORTA = leds[0+ ledbit];
	PORTC = leds[11+ledbit];
	PORTB = leds[22+ledbit];
}
*/

