#include <avr/eeprom.h>
#include <stdlib.h>
#include <string.h>
#include "bl0rg.h"


/* EEPROM data */
uint8_t laststate[24] EEMEM;
static uint8_t eepromdata[20][24] EEMEM;

uint8_t eeprom_ok(void)
{
	if (eeprom_is_ready()) {
		return(1);
	} else {
		sendstring("eeprom not ready\n\r");
		return(0);
	}
}

void play_eeprom(void)
{
	uint8_t i = 0;
	uint8_t *s = eepromdata[0];
	
	eeprom_ok(); /* generate a warning if eeprom is not ready */
	eeprom_busy_wait();
		
	for(i = 0; i<20;i++) {
		eeprom_read_block(&leds,s,24);
		s+=24;
		delay_ms(100.0);
	}
}

void record_eeprom(void)
{
	uint8_t c = 0, led = 0;
	uint8_t counter = 1;
	uint8_t *ee = eepromdata[0];
	
	eeprom_ok();
	eeprom_busy_wait();
	/* check if eeprom is ready */
/*
	if (!eeprom_ok())
		return;
*/	
	
	sendstring("prog. eeprom(exit with x)\n\r");
	sendint(counter);
	sendchar(' ');
	sendchar('!');
	
	// recline();
	
	while(counter <= 20) {
		c = 0;
		
		recline();
		
		if (buffer[c] == 'x' || buffer[c] == 'X')
			return;
	
		/* should the prelude be printed for next loop-run */
		counter++;
		if (counter <= 20) {
			sendint(counter);
			if (counter<10)
				sendchar(' ');
			sendchar('!');
		}
	
		led = 0;
		while(buffer[c] && buffer[c+1]) {
			SetLed(led++,hextoint(buffer[c])*16 + hextoint(buffer[c+1]));
			c+=2;
		}
		eeprom_write_block(&leds,ee,24);
		ee += 24;
	}
}

void clear_eeprom(void)
{
	int i = 0;
	uint8_t *ee = eepromdata[0];
	
	sendstring("clearing eeprom\n\r");
	eeprom_ok();
	eeprom_busy_wait();
	
	for (i=0;i<24*20;++i) {
		eeprom_write_byte(ee,0);
		ee++;
	}
	
	ee = laststate;
	for (i=0;i<24;++i) {
		eeprom_write_byte(ee,0);
		ee++;
	}
}

void random_eeprom(void)
{
	int c = 0, i = 0;
	uint8_t lleds[24];
	uint8_t *ee = eepromdata[0];
	
	srand(128);
	for(c=0;c<20;++c) {
		for(i=0;i<24;++i)
			SetLed(i,rand());
		memcpy(&leds,lleds,24);
		eeprom_write_block(&leds,ee,24);
		ee+=24;
		delay_ms(10);
	}
}
