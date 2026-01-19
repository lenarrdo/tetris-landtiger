#include <stdlib.h>

#include "GLCD/GLCD.h"
#include "LPC17xx.h"
#include "RIT/RIT.h"
#include "adc/adc.h"
#include "button_EXINT/button.h"
#include "joystick/joystick.h"
#include "tetris/core/block.h"
#include "tetris/core/effects.h"
#include "tetris/core/playing_field.h"
#include "tetris/core/stats.h"
#include "tetris/core/tetromino.h"
#include "tetris/gui/gui.h"
#include "tetris/music/music.h"
#include "timer/timer.h"

#define MAX_CLEARABLE_LINES 10

// ScaleFlag needs to visible in order for the emulator to find the symbol (can
// be placed also inside system_LPC17xx.h but since it is RO, it needs more
// work)
#ifdef SIMULATOR
extern uint8_t ScaleFlag;
#endif

static void handle_user_input(void);
static inline bool lose_conditions(void);
static inline void enter_power_down(void);
static inline int max(int *arr, int size);
static int compute_line_clear_points(int nof_cleared_lines);

int main(void)
{
	SystemInit();

	LCD_Initialization();

	BUTTON_init();

	init_RIT(ONE_SECOND / 5);
	enable_RIT();

	joystick_init();

	ADC_init();

	music_init();

	init_timer(TIMER0, 0, 0, CONTROL_RESET | CONTROL_INTERRUPT, ONE_SECOND);
	init_timer(TIMER0, 0, 1, CONTROL_INTERRUPT, ONE_SECOND / 2);
	init_timer(TIMER1, 0, 0, CONTROL_NULL, 0);
	init_timer(TIMER2, 0, 0, CONTROL_RESET | CONTROL_INTERRUPT,
		   SLOW_DOWN_DURATION * ONE_SECOND);

	high_score = 0;

menu:
	game_state = PAUSED;
	score = 0;
	tot_cleared_lines = 0;
	buffered_move = NONE;
	int lines_since_last_powerup = 0;
	int lines_since_last_malus = 0;

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

	// reuse TIMER1 to play music theme
	curr_audio_track = A_TYPE_THEME;
	init_timer(TIMER1, 0, 0, CONTROL_INTERRUPT | CONTROL_RESET, ONE_SECOND);
	enable_timer(TIMER1);

	enable_timer(TIMER0);

	prev_tetromino_speed = 1;
	enum game_state prev_game_state = PAUSED;
	game_state = PLAYING;
	while (game_state != GAME_OVER) {
		/******************************************************
		 * PART 1: TETROMINO CREATION & LOSE CONDITIONS CHECK *
		 ******************************************************/

		curr_tetromino = tetromino_create(rand() % NOF_TETROMINOS,
						  (struct block){ 3, 0 });
		tetromino_draw(curr_tetromino);

		if (lose_conditions()) {
			game_state = GAME_OVER;
			continue;
		}

		/*************************************
		 * PART 2: HANDLE TETROMINO MOVEMENT *
		 *************************************/

		while (!curr_tetromino.placed) {
			enter_power_down();
			if (prev_game_state != game_state) {
				if (game_state == PAUSED)
					show_paused_popup();
				else if (game_state == PLAYING)
					hide_popup();
				prev_game_state = game_state;
			}
			if (!slow_down_active &&
			    prev_tetromino_speed != curr_tetromino_speed) {
				prev_tetromino_speed = curr_tetromino_speed;
				edit_timer_interval(
					TIMER0, 0,
					ONE_SECOND / curr_tetromino_speed);
				edit_timer_interval(
					TIMER0, 1,
					ONE_SECOND / curr_tetromino_speed / 2);
			}
			if (game_state == PLAYING)
				handle_user_input();
		}

		/***************************************************************
		 * PART 3: POST TETROMINO PLACEMENT STUFF (SCORE AWARDING... ) *
		 ***************************************************************/

		score += 10;

		int cleared_lines[MAX_CLEARABLE_LINES] = { 0 };
		int nof_cleared_lines = 0;
		for (int i = 0; i < 4; i++) {
			int curr_line = curr_tetromino.blocks[i].y;
			bool curr_line_is_complete = true;
			bool clear_lines_found = false;
			bool slow_down_found = false;
			for (int x = 0; curr_line_is_complete &&
					x < PLAYING_FIELD_HSQUARES;
			     x++)
				switch (playing_field[x][curr_line]) {
				case HIDDEN:
					curr_line_is_complete = false;
					break;
				case CLEAR_LINES_COLOR:
					clear_lines_found = true;
					break;
				case SLOW_DOWN_COLOR:
					slow_down_found = true;
					break;
				default:
					break;
				}

			if (curr_line_is_complete) {
				playing_field_clear_row(curr_line);
				cleared_lines[nof_cleared_lines++] = curr_line;

				if (clear_lines_found)
					effects_powerup_clear_lines_activate(
						cleared_lines,
						&nof_cleared_lines);

				if (slow_down_found) {
					prev_tetromino_speed =
						curr_tetromino_speed;
					effects_powerup_slow_down_activate();
					enable_timer(TIMER2);
				}
			}
		}

		if (nof_cleared_lines > 0) {
			// play sound effect
			curr_audio_track = SFX_CLEAR_LINES;

			// update stats
			score += compute_line_clear_points(nof_cleared_lines);
			tot_cleared_lines += nof_cleared_lines;

			// redraw playing field
			int lowest_cleared_line =
				max(cleared_lines, MAX_CLEARABLE_LINES);
			playing_field_compress_empty_rows(lowest_cleared_line);
			playing_field_redraw(lowest_cleared_line,
					     peak - nof_cleared_lines);

			// optionally spawn a new powerup
			lines_since_last_powerup += nof_cleared_lines;
			if (lines_since_last_powerup >= NOF_LINES_FOR_POWERUP) {
				if (!playing_field_is_empty()) {
					struct block powerup_pos =
						effects_powerup_spawn();
					playing_field_redraw_block(
						powerup_pos.x, powerup_pos.y);
				}
				lines_since_last_powerup -=
					NOF_LINES_FOR_POWERUP;
			}
			visualize_lines();

			lines_since_last_malus += nof_cleared_lines;
			if (lines_since_last_malus >= NOF_LINES_RANDOM_MALUS) {
				if (!effects_random_malus_activate())
					game_state = GAME_OVER;
				playing_field_redraw(PLAYING_FIELD_BOTTOM,
						     peak);

				lines_since_last_malus -=
					NOF_LINES_RANDOM_MALUS;
			}
		}
		visualize_score();
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

/********************
 * HELPER FUNCTIONS *
 ********************/

static void handle_user_input(void)
{
	if (buffered_move == NONE)
		return;

	tetromino_hide(curr_tetromino);
	if (buffered_move & MOVE_LEFT) {
		tetromino_move_left(&curr_tetromino);
		buffered_move &= ~MOVE_LEFT;
	}
	if (buffered_move & MOVE_RIGHT) {
		tetromino_move_right(&curr_tetromino);
		buffered_move &= ~MOVE_RIGHT;
	}
	if (buffered_move & MOVE_DOWN) {
		tetromino_move_down(&curr_tetromino);
		buffered_move &= ~MOVE_DOWN;
	}
	if (buffered_move & ROTATE) {
		tetromino_rotate_clockwise(&curr_tetromino);
		buffered_move &= ~ROTATE;
	}
	if (buffered_move & HARD_DROP) {
		tetromino_hard_drop(&curr_tetromino);
		buffered_move &= ~HARD_DROP;
	}
	tetromino_draw(curr_tetromino);
}

static inline bool lose_conditions(void)
{
	for (int i = 0; i < 4; i++)
		if (playing_field_is_occupied(curr_tetromino.blocks[i].x,
					      curr_tetromino.blocks[i].y) ||
		    is_block_out_of_bounds(curr_tetromino.blocks[i]))
			return true;
	return false;
}

static inline void enter_power_down(void)
{
	LPC_SC->PCON |= (1U << 0);
	LPC_SC->PCON &= ~(1U << 1);

	__asm volatile("wfi");
}

static inline int max(int *arr, int size)
{
	int max = arr[0];
	for (int i = 1; i < size; i++)
		if (arr[i] > max)
			max = arr[i];
	return max;
}

static int compute_line_clear_points(int nof_cleared_lines)
{
	int points = 0;
	while (nof_cleared_lines >= 4) {
		points += 600;
		nof_cleared_lines -= 4;
	}

	return points + 100 * nof_cleared_lines;
}
