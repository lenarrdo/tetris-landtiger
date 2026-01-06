#include "button.h"
#include "LPC17xx.h"
#include "RIT/RIT.h"
#include "tetris/core/stats.h"

#define CLEAR_PENDING_INTERRUPT(button) (LPC_SC->EXTINT &= (1 << (button)))

void EINT0_IRQHandler(void)
{
	CLEAR_PENDING_INTERRUPT(0);
}

void EINT1_IRQHandler(void)
{
	down_1 = 1;
	NVIC_DisableIRQ(EINT1_IRQn);
	LPC_PINCON->PINSEL4 &= ~(1 << 22);

	CLEAR_PENDING_INTERRUPT(1);
}

void EINT2_IRQHandler(void)
{
	if (game_state == PLAYING) {
		down_2 = 1;
		NVIC_DisableIRQ(EINT2_IRQn);
		LPC_PINCON->PINSEL4 &= ~(1 << 24);
	}
	CLEAR_PENDING_INTERRUPT(2);
}
