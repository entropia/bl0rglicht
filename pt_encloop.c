#include <inttypes.h>
#include "bl0rg.h"



int encloop(struct pt *pt)
{
	PT_BEGIN(pt);
	while(1) {
		
		
		if (idle_anim > 0) idle_anim--;
		else
		{
			for(uint8_t i=0; i < 24; i++)
			{
				SetLed(i,anim_leds[i] / 64);
				anim_leds[i] += anim_leds_dir[i];
				if (anim_leds[i] > 64*255) anim_leds_dir[i] = -anim_leds_dir[i];
				if (anim_leds[i] < 0)      
				{
					anim_leds_dir[i] = -(187*anim_leds_dir[i]) % 128 + 10;
					anim_leds[i] = 0;
				}
			}
		}
	
		timerticks = 0;
		PT_WAIT_UNTIL(pt,timerticks >= 150);
		//PT_YIELD(pt);
		
	}
	PT_END(pt);
}
