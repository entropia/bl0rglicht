#include <inttypes.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>
#include <util/delay.h>

#include <stdlib.h>
#include <string.h>

#include "pt.h" /* protothreads */

#include "buffer.h"
#include "uart.h"

#include "bl0rg.h"



/* protothreads */
static struct pt serloop_pt, encloop_pt;

int main (void)
{
	for (uint8_t i = 0; i < 24; i++)
	{
		anim_leds_dir[i] = (187*i+45) % 128 + 10;
	}


	idle_anim = 1;				// Animation abspielen bis Daten vom PC kommen
	/* UCSRA &= ~ _BV(U2X); */		// no doublespeed for serial port
	uartInit();					// initialize UART (serial port)
	uartSetBaudRate(BAUDRATE);	// set UART speed
	
	sendstring("\n\r");
	sendstring(VERSION_STRING);
	
	
    DDRA = 0xff;
	DDRB = 0xff;
	DDRC = 0xff;
	DDRD = 0xff;
	
	InitTimer();
	sei();
	
	PORTA = 0x00;
	PORTB = 0x00;
	PORTC = 0x00;
	PORTD = 0x00;
	
	eeprom_ok();
	eeprom_busy_wait();
	eeprom_read_block(&leds,laststate,24);
	
	sendstring("\n\r");
	/* play_eeprom(); */
	sendstring("ok\n\r");
	
	for(uint8_t i = 0; i<24; ++i)
		SetLed(i,0x00);
	
	
	/* here goes protothreads */
	PT_INIT(&serloop_pt);
	PT_INIT(&encloop_pt);
	
	while(1) {
		serloop(&serloop_pt);
		encloop(&encloop_pt);
	}
}
