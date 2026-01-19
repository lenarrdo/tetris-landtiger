#include "LPC17xx.h"
#include "RIT/RIT.h"
#include "tetris/core/stats.h"
#include "tetris/core/tetromino.h"
#include "tetris/music/music.h"
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
		switch (curr_audio_track) {
		case A_TYPE_THEME:
			set_note(a_type_theme[curr_music_note]);

			disable_timer(TIMER1);
			reset_timer(TIMER1);
			init_timer(TIMER1, 0, 0,
				   CONTROL_RESET | CONTROL_INTERRUPT,
				   a_type_theme[curr_music_note].duration);
			enable_timer(TIMER1);
			curr_music_note++;

			if (curr_music_note == NOF_NOTES_A_TYPE)
				curr_music_note = 0;
			break;
		case SFX_CLEAR_LINES:
			set_note(sfx_clear_lines[curr_sfx_note]);

			disable_timer(TIMER1);
			reset_timer(TIMER1);
			init_timer(TIMER1, 0, 0,
				   CONTROL_RESET | CONTROL_INTERRUPT,
				   sfx_clear_lines[curr_sfx_note].duration);
			enable_timer(TIMER1);
			curr_sfx_note++;

			// sound effect has finished playing, resume to music theme
			if (curr_sfx_note == NOF_NOTES_SFX_CLEAR_LINES) {
				curr_sfx_note = 0;
				curr_audio_track = A_TYPE_THEME;
			}
			break;
		case NO_AUDIO:
			break;
		}

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
		disable_timer(TIMER2);

		edit_timer_interval(TIMER0, 0,
				    ONE_SECOND / prev_tetromino_speed);
		edit_timer_interval(TIMER0, 1,
				    ONE_SECOND / prev_tetromino_speed / 2);

		unsigned int temp = curr_tetromino_speed;
		curr_tetromino_speed = prev_tetromino_speed;
		prev_tetromino_speed = temp;

		slow_down_active = false;

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
		play_note();
		LPC_TIM3->IR = 1;
	} else if (LPC_TIM3->IR & 2) { // MR1
		LPC_TIM3->IR = 2;
	} else if (LPC_TIM3->IR & 4) { // MR2
		LPC_TIM3->IR = 4;
	} else if (LPC_TIM3->IR & 8) { // MR3
		LPC_TIM3->IR = 8;
	}
}
