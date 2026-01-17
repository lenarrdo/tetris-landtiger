#include <stdlib.h>

#include "timer/timer.h"

#include "effects.h"
#include "block.h"
#include "playing_field.h"
#include "stats.h"

struct block effects_powerup_spawn(void)
{
	int random_y = rand() % (PLAYING_FIELD_VSQUARES - peak) + peak;

	int nof_filled_x_blocks = 0;
	int filled_x_positions[PLAYING_FIELD_HSQUARES];
	for (int i = 0; i < PLAYING_FIELD_HSQUARES; i++)
		filled_x_positions[i] = -1;
	for (int i = 0; i < PLAYING_FIELD_HSQUARES; i++)
		if (playing_field[i][random_y] != HIDDEN &&
		    playing_field[i][random_y] != CLEAR_LINES_COLOR &&
		    playing_field[i][random_y] != SLOW_DOWN_COLOR)
			filled_x_positions[nof_filled_x_blocks++] = i;
	int random_x = filled_x_positions[rand() % nof_filled_x_blocks];

	enum powerup random_powerup = rand() % NOF_POWERUPS;
	switch (random_powerup) {
	case CLEAR_LINES:
		playing_field[random_x][random_y] = CLEAR_LINES_COLOR;
		break;
	case SLOW_DOWN:
		playing_field[random_x][random_y] = SLOW_DOWN_COLOR;
		break;
	}

	return (struct block){ random_x, random_y };
}

void effects_powerup_clear_lines_activate(int *cleared_lines,
					  int *nof_cleared_lines)
{
	int ceiling = peak > PLAYING_FIELD_VSQUARES / 2 ?
			      peak :
			      PLAYING_FIELD_VSQUARES / 2;

	for (int row = PLAYING_FIELD_BOTTOM; row >= ceiling; row--)
		if (!playing_field_is_empty_row(row)) {
			playing_field_clear_row(row);
			cleared_lines[(*nof_cleared_lines)++] = row;
		}
}

void effects_powerup_slow_down_activate(void)
{
	curr_tetromino_speed = 1;
	edit_timer_interval(TIMER0, 0, ONE_SECOND);
	edit_timer_interval(TIMER0, 1, ONE_SECOND / 2);
	slow_down_active = true;
}

bool effects_random_malus_activate(void)
{
	if (!playing_field_shift_up(1))
		return false;

	for (int x = 0; x < PLAYING_FIELD_HSQUARES; x++)
		playing_field[x][PLAYING_FIELD_BOTTOM] = RANDOM_MALUS;

	int nof_empty_blocks = 0;
	do {
		int randx = rand() % PLAYING_FIELD_HSQUARES;
		if (playing_field[randx][PLAYING_FIELD_BOTTOM] ==
		    RANDOM_MALUS) {
			playing_field[randx][PLAYING_FIELD_BOTTOM] = HIDDEN;
			nof_empty_blocks++;
		}
	} while (nof_empty_blocks < 3);

	return true;
}
