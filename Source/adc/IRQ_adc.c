#include <math.h>

#include "LPC17xx.h"
#include "adc.h"
#include "tetris/core/stats.h"

unsigned short ad_curr;
unsigned short ad_prev = 0xFF;

void ADC_IRQHandler(void)
{
	ad_curr = ((LPC_ADC->ADGDR >> 4) & 0xFFF);
	if (ad_curr != ad_prev) {
		current_tetromino_speed =
			round((double)ad_curr / 0xFFF * 4) + 1;

		ad_prev = ad_curr;
	}
}
