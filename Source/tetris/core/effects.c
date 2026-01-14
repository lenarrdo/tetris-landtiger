#include <stdlib.h>

#include "effects.h"
#include "block.h"
#include "playing_field.h"

struct block playing_field_spawn_random_powerup(void)
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

	enum powerup random_powerup = rand() % 2;
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
