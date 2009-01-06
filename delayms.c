#include <util/delay.h>
#include "bl0rg.h"

void delay_ms(uint16_t ms)
{
        for(uint16_t i = 0; i <= ms; i++)
        {
            _delay_ms(1);
        }
}


void WaitTimerTicks(uint32_t ticks)
{
	timerticks = 0;
	while (timerticks <= ticks);
}
