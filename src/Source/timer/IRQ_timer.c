#include "LPC17xx.h"
#include "timer.h"
#include "tetromino/tetromino.h"
#include "game_stats.h"

void TIMER0_IRQHandler(void)
{
	// MR0
	if (LPC_TIM0->IR & 1) {
		tetromino_hide(&current_tetromino);
		tetromino_move_down(&current_tetromino);
		tetromino_draw(current_tetromino);
		LPC_TIM0->IR = 1; // clear interrupt flag
	} else if (LPC_TIM0->IR & 2) { // MR1
		LPC_TIM0->IR = 2; // clear interrupt flag
	} else if (LPC_TIM0->IR & 4) { // MR2
		LPC_TIM0->IR = 4; // clear interrupt flag
	} else if (LPC_TIM0->IR & 8) { // MR3
		LPC_TIM0->IR = 8; // clear interrupt flag
	}
}

void TIMER1_IRQHandler(void)
{
	// MR0
	if (LPC_TIM1->IR & 1) {
		LPC_TIM1->IR = 1; // clear interrupt flag
	} else if (LPC_TIM1->IR & 2) { // MR1
		LPC_TIM1->IR = 2; // clear interrupt flag
	} else if (LPC_TIM1->IR & 4) { // MR2
		LPC_TIM1->IR = 4; // clear interrupt flag
	} else if (LPC_TIM1->IR & 8) { // MR3
		LPC_TIM1->IR = 8; // clear interrupt flag
	}
}

void TIMER2_IRQHandler(void)
{
	// MR0
	if (LPC_TIM2->IR & 1) {
		LPC_TIM2->IR = 1; // clear interrupt flag
	} else if (LPC_TIM2->IR & 2) { // MR1
		LPC_TIM2->IR = 2; // clear interrupt flag
	} else if (LPC_TIM2->IR & 4) { // MR2
		LPC_TIM2->IR = 4; // clear interrupt flag
	} else if (LPC_TIM2->IR & 8) { // MR3
		LPC_TIM2->IR = 8; // clear interrupt flag
	}
}

void TIMER3_IRQHandler(void)
{
	// MR0
	if (LPC_TIM3->IR & 1) {
		LPC_TIM3->IR = 1; // clear interrupt flag
	} else if (LPC_TIM3->IR & 2) { // MR1
		LPC_TIM3->IR = 2; // clear interrupt flag
	} else if (LPC_TIM3->IR & 4) { // MR2
		LPC_TIM3->IR = 4; // clear interrupt flag
	} else if (LPC_TIM3->IR & 8) { // MR3
		LPC_TIM3->IR = 8; // clear interrupt flag
	}
}
