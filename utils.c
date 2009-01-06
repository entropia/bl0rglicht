#include <inttypes.h>
#include "bl0rg.h"

uint8_t hextoint(char c)
{
	if (c >= 'A' && c <= 'F')
		return(c-'A'+10);
	else if (c >= 'a' && c <= 'f')
		return(c-'a'+10);
	else if (c >= '0' && c <= '9')
		return(c-'0');
	else
		return(0);
}

