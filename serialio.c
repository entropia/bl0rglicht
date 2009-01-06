#include <inttypes.h>
#include <stdlib.h>
#include "uart.h"
#include "bl0rg.h"

/* is a complete line(with \n) ready for reading? */
volatile uint8_t lready = 0;

/* buffer for general purpose */
volatile char buffer[128];

void sendchar(char i)
{
	uartSendByte(i);
}

void sendstring(char* text)
{
	uint8_t i = 0;
	
	while (text[i] != 0) 
	{
		sendchar(text[i]);
		i++;
	}
}

void sendint(uint8_t i)
{
	char acount[2];
	itoa(i,acount,10);
	sendstring(acount);
}

/* only for use with recline()!
   this function doesn't block if no character is avaible! */
char recchar(void)
{
	// static int i = 0;
	// while((i = uartGetByte()) != -1);
	return((char)uartGetByte());
}

char *recline(void)
{
	int c = 0;

	while(!lready);
	// sendchar('!');
	while((buffer[c] = recchar()) != '\n') {
		sendchar(buffer[c]);
		c++;
	}
	buffer[c] = '\0';
	sendstring("\n\r");
	lready = 0; // we got the newline
	return((char *)buffer);
}
