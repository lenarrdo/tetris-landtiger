#include "button.h"
#include "LPC17xx.h"
#include "tetromino/tetromino.h"
#include "game_stats.h"

#define CLEAR_PENDING_INTERRUPT(button) (LPC_SC->EXTINT &= (1 << (button)))

void EINT0_IRQHandler(void)
{
	CLEAR_PENDING_INTERRUPT(0);
}

void EINT1_IRQHandler(void)
{
	CLEAR_PENDING_INTERRUPT(1);
}

void EINT2_IRQHandler(void)
{
	tetromino_hide(&current_tetromino);
	tetromino_hard_drop(&current_tetromino);
	tetromino_draw(current_tetromino);

	CLEAR_PENDING_INTERRUPT(2);
}
