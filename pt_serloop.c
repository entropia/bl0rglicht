#include <inttypes.h>
#include <avr/eeprom.h>
#include <stdlib.h>
#include <string.h>
#include "pt.h"
#include "bl0rg.h"


/* the interactive serial i/o loop */
int serloop(struct pt *pt)
{
	PT_BEGIN(pt);

	static uint8_t flag = 0;
	static uint8_t *randbuffer;
	static uint8_t c,led,i;

    while(1)
	{
		c = 0;
		led = 0;
		i = 0;
		
		/*
		sendint(encoder_value);
		sendstring("\n\r");
		*/
		
		memset((void *)buffer,0x00,sizeof(char)*128);
	
		sendchar('!');
		/* we are not allowed to sleep in this function */
		PT_WAIT_UNTIL(pt,lready==1);
		recline();
		
		idle_anim = 10000;			// Idle Animation STOP
		
		switch(buffer[c++]) {
		case '\r':
			continue;
			break;
		case 'a':
		case 'A':
			eeprom_ok();
			eeprom_write_block(&leds,laststate,24);
		case 's':
		case 'S':
			while(buffer[c] && buffer[c+1]) {
				SetLed(led++,hextoint(buffer[c])*16 + hextoint(buffer[c+1]));
				c+=2;
			}
			break;
		case 'g':
			sendstring("Error flag: ");
			sendint(errflag);
			sendstring("\r\n");
			break;
		case 'G':
			errflag = 0;
			break;
		case 'c':
		case 'C':
			if (buffer[c] == 'e') 
				clear_eeprom();
			else {
				for(uint8_t i = 0; i<24; ++i)
					SetLed(i,0);
				memset((void *)buffer,0x00,sizeof(char)*128);
			}
			break;
		case 'f':
		case 'F':
			for(uint8_t i = 0; i<24; ++i)
				SetLed(i,0xff);
			memset((void *)buffer,0xff,sizeof(char)*128);
			break;
		case 'i':
		case 'I':
			sendstring(VERSION_STRING);
			break;
		case 'r':
		case 'R':
			if (!flag) {
				for(uint8_t i=0,f=0; i<24; ++i) {
					if (f) {
						SetLed(i,0);
						++f;
					} else {
						SetLed(i,255);
						--f;
					}
				}
				flag++;
			} else {
				for(uint8_t i=0,f=0; i<24; ++i) {
					if (!f) {
						SetLed(i,0);
						--f;
					} else {
						SetLed(i,255);
						++f;
					}
				}
				flag--;
			}
			break;
		case 'e':
		case 'E':
			if (buffer[c] == 'r' || buffer[c] == 'R')
				record_eeprom();
			else if (buffer[c] == 'p' || buffer[c] == 'P')
				play_eeprom();
			else
				sendstring("r or p\n\r");
			break;
		case 'y':
		case 'Y':
			random_eeprom();
			break;
		case 'w':
		case 'W':
			randbuffer = malloc(24);
			if (randbuffer != NULL)
			{
				srand(128);
				for(;;) {
					for(i=0;i<24;i++) {
						randbuffer[i] = rand();
					}
					memcpy(&leds,randbuffer,24);
					delay_ms(100);
				}
				free(randbuffer);
			} else 
				sendstring("memory low\n\r");
			break;
		case 'z':
		case 'Z':
			for(;;)
				play_eeprom();
			break;
		case 'd':
		case 'D':
			for (uint8_t i = 0; i < 33; i++)
			{
				sendint(leds[i]);
				sendstring("!\n\r");
			}
			break;
		default:
			sendstring("Unknown command\n\r");
		}
	}
	PT_END(pt);
}

