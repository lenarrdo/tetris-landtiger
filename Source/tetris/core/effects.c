#include <stdlib.h>

#include "effects.h"
#include "tetromino.h"
#include "playing_field.h"

struct block playing_field_spawn_random_powerup(void)
{
	int random_y =
		rand() % (PLAYING_FIELD_VSQUARES - playing_field_get_peak()) +
		playing_field_get_peak();

	int nof_filled_x_blocks = 0;
	int filled_x_positions[PLAYING_FIELD_HSQUARES];
	for (int i = 0; i < PLAYING_FIELD_HSQUARES; i++)
		filled_x_positions[i] = -1;
	for (int i = 0; i < PLAYING_FIELD_HSQUARES; i++)
		if (playing_field_get_color(i, random_y) != HIDDEN &&
		    playing_field_get_color(i, random_y) != Grey &&
		    playing_field_get_color(i, random_y) != Blue2)
			filled_x_positions[nof_filled_x_blocks++] = i;
	int random_x = filled_x_positions[rand() % nof_filled_x_blocks];

	enum powerup random_powerup = rand() % 2;
	switch (random_powerup) {
	case CLEAR_LINES:
		playing_field_set_color(random_x, random_y, Grey);
		break;
	case SLOW_DOWN:
		playing_field_set_color(random_x, random_y, Blue2);
		break;
	}

	return (struct block){ random_x, random_y };
}
