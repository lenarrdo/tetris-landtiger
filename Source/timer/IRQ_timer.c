#include "LPC17xx.h"
#include "RIT/RIT.h"
#include "tetris/core/stats.h"
#include "tetris/core/tetromino.h"
#include "timer.h"

void TIMER0_IRQHandler(void)
{
	// MR0
	if (LPC_TIM0->IR & 1) {
		buffered_move |= MOVE_DOWN;
		LPC_TIM0->IR = 1;
	} else if (LPC_TIM0->IR & 2) { // MR1
		if (joystick_down)
			buffered_move |= MOVE_DOWN;
		LPC_TIM0->IR = 2;
	} else if (LPC_TIM0->IR & 4) { // MR2
		LPC_TIM0->IR = 4;
	} else if (LPC_TIM0->IR & 8) { // MR3
		LPC_TIM0->IR = 8;
	}
}

void TIMER1_IRQHandler(void)
{
	// MR0
	if (LPC_TIM1->IR & 1) {
		LPC_TIM1->IR = 1;
	} else if (LPC_TIM1->IR & 2) { // MR1
		LPC_TIM1->IR = 2;
	} else if (LPC_TIM1->IR & 4) { // MR2
		LPC_TIM1->IR = 4;
	} else if (LPC_TIM1->IR & 8) { // MR3
		LPC_TIM1->IR = 8;
	}
}

void TIMER2_IRQHandler(void)
{
	// MR0
	if (LPC_TIM2->IR & 1) {
		LPC_TIM2->IR = 1;
	} else if (LPC_TIM2->IR & 2) { // MR1
		LPC_TIM2->IR = 2;
	} else if (LPC_TIM2->IR & 4) { // MR2
		LPC_TIM2->IR = 4;
	} else if (LPC_TIM2->IR & 8) { // MR3
		LPC_TIM2->IR = 8;
	}
}

void TIMER3_IRQHandler(void)
{
	// MR0
	if (LPC_TIM3->IR & 1) {
		LPC_TIM3->IR = 1;
	} else if (LPC_TIM3->IR & 2) { // MR1
		LPC_TIM3->IR = 2;
	} else if (LPC_TIM3->IR & 4) { // MR2
		LPC_TIM3->IR = 4;
	} else if (LPC_TIM3->IR & 8) { // MR3
		LPC_TIM3->IR = 8;
	}
}
