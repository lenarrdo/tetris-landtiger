#include "button.h"
#include "LPC17xx.h"
#include "tetromino/tetromino.h"
#include "game_stats.h"
#include "timer/timer.h"

#define CLEAR_PENDING_INTERRUPT(button) (LPC_SC->EXTINT &= (1 << (button)))
#define ONE_SECOND (2500000)

void EINT0_IRQHandler(void)
{
	CLEAR_PENDING_INTERRUPT(0);
}

void EINT1_IRQHandler(void)
{
	switch (game_state) {
	case PAUSED:
		enable_timer(TIMER0);
		game_state = PLAYING;
		break;
	case PLAYING:
		disable_timer(TIMER0);
		game_state = PAUSED;
		break;
	case GAME_OVER:
		// TODO: reset playing field and goto menu.
		break;
	}
	CLEAR_PENDING_INTERRUPT(1);
}

void EINT2_IRQHandler(void)
{
	if (game_state == PLAYING) {
		tetromino_hide(&current_tetromino);
		tetromino_hard_drop(&current_tetromino);
		tetromino_draw(current_tetromino);
	}

	CLEAR_PENDING_INTERRUPT(2);
}
