#include <stdlib.h>

#include "GLCD/GLCD.h"
#include "LPC17xx.h"
#include "RIT/RIT.h"
#include "button_EXINT/button.h"
#include "joystick/joystick.h"
#include "tetris/core/stats.h"
#include "tetris/core/playing_field.h"
#include "tetris/core/tetromino.h"
#include "tetris/gui/gui.h"
#include "timer/timer.h"

#define ONE_SECOND (25000000)

// ScaleFlag needs to visible in order for the emulator to find the symbol (can
// be placed also inside system_LPC17xx.h but since it is RO, it needs more
// work)
#ifdef SIMULATOR
extern uint8_t ScaleFlag;
#endif

static void handle_user_input(void);
static inline bool lose_conditions(void);
static inline void enter_power_down(void);

int main(void)
{
	SystemInit();

	LCD_Initialization();

	BUTTON_init();

	init_RIT(ONE_SECOND / 5);
	enable_RIT();

	joystick_init();

	init_timer(TIMER0, 0, 0, CONTROL_RESET | CONTROL_INTERRUPT, ONE_SECOND);
	init_timer(TIMER0, 0, 1, CONTROL_INTERRUPT, ONE_SECOND / 2);
	init_timer(TIMER1, 0, 0, CONTROL_NULL, 0);

	high_score = 0;

menu:
	game_state = PAUSED;
	score = 0;
	tot_cleared_lines = 0;
	buffered_move = NONE;

	init_playing_borders();
	show_default_stats();
	playing_field_reset();
	show_paused_popup();

	enable_timer(TIMER1);

	while (game_state == PAUSED)
		enter_power_down();

	/***************
	 * GAME BEGINS *
	 ***************/

	srand(get_timer_value(TIMER1));
	disable_timer(TIMER1);
	reset_timer(TIMER1);

	enable_timer(TIMER0);

	enum game_state prev_game_state = PAUSED;
	game_state = PLAYING;
	while (game_state != GAME_OVER) {
		current_tetromino = tetromino_create(rand() % NOF_TETROMINOS,
						     (struct block){ 3, 0 });
		tetromino_draw(current_tetromino);

		if (lose_conditions()) {
			game_state = GAME_OVER;
			continue;
		}

		int prev_tot_cleared_lines = tot_cleared_lines;
		while (!current_tetromino.placed) {
			enter_power_down();
			if (prev_game_state != game_state) {
				if (game_state == PAUSED)
					show_paused_popup();
				else if (game_state == PLAYING)
					hide_popup();
				prev_game_state = game_state;
			}
			if (game_state == PLAYING)
				handle_user_input();
		}
		visualize_score();
		int cleared_lines = tot_cleared_lines - prev_tot_cleared_lines;
		if (cleared_lines > 0) {
			visualize_lines();
			playing_field_redraw(lowest_cleared_line,
					     cleared_lines);
		}
	}
	if (score > high_score)
		high_score = score;

	disable_timer(TIMER0);
	reset_timer(TIMER0);

	show_game_over_popup();
	while (game_state == GAME_OVER)
		enter_power_down();

	goto menu;
}

static void handle_user_input(void)
{
	if (buffered_move == NONE)
		return;

	tetromino_hide(current_tetromino);
	if (buffered_move & MOVE_LEFT) {
		tetromino_move_left(&current_tetromino);
		buffered_move &= ~MOVE_LEFT;
	}
	if (buffered_move & MOVE_RIGHT) {
		tetromino_move_right(&current_tetromino);
		buffered_move &= ~MOVE_RIGHT;
	}
	if (buffered_move & MOVE_DOWN) {
		tetromino_move_down(&current_tetromino);
		buffered_move &= ~MOVE_DOWN;
	}
	if (buffered_move & ROTATE) {
		tetromino_rotate_clockwise(&current_tetromino);
		buffered_move &= ~ROTATE;
	}
	if (buffered_move & HARD_DROP) {
		tetromino_hard_drop(&current_tetromino);
		buffered_move &= ~HARD_DROP;
	}
	tetromino_draw(current_tetromino);
}

static inline bool lose_conditions(void)
{
	for (int i = 0; i < 4; i++)
		if (playing_field_is_occupied(current_tetromino.blocks[i].x,
					      current_tetromino.blocks[i].y) ||
		    is_block_out_of_bounds(current_tetromino.blocks[i]))
			return true;
	return false;
}

static inline void enter_power_down(void)
{
	LPC_SC->PCON |= (1U << 0);
	LPC_SC->PCON &= ~(1U << 1);

	__asm volatile("wfi");
}
