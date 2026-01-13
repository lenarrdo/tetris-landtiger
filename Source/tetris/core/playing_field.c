#include <stdbool.h>
#include <stdlib.h>

#include "playing_field.h"
#include "tetromino.h"

enum tetromino_color playing_field[PLAYING_FIELD_HSQUARES]
				  [PLAYING_FIELD_VSQUARES];
int peak;

static void playing_field_copy_row(int dest_line, int src_line);

void playing_field_reset(void)
{
	peak = PLAYING_FIELD_BOTTOM;
	for (int x = 0; x < PLAYING_FIELD_HSQUARES; x++)
		for (int y = 0; y < PLAYING_FIELD_VSQUARES; y++)
			playing_field[x][y] = HIDDEN;
}

bool playing_field_is_occupied(int pos_x, int pos_y)
{
	return playing_field[pos_x][pos_y] != HIDDEN;
}

void playing_field_add_tetromino(volatile struct tetromino *tetromino)
{
	for (int i = 0; i < 4; i++) {
		playing_field[tetromino->blocks[i].x][tetromino->blocks[i].y] =
			tetromino->color;
		if (tetromino->blocks[i].y < peak)
			peak = tetromino->blocks[i].y;
	}
}

void playing_field_clear_row(int row)
{
	for (int x = 0; x < PLAYING_FIELD_HSQUARES; x++)
		playing_field[x][row] = HIDDEN;
}

bool playing_field_is_complete_row(int row)
{
	for (int x = 0; x < PLAYING_FIELD_HSQUARES; x++)
		if (playing_field[x][row] == HIDDEN)
			return false;
	return true;
}

bool playing_field_is_empty_row(int row)
{
	for (int x = 0; x < PLAYING_FIELD_HSQUARES; x++)
		if (playing_field[x][row] != HIDDEN)
			return false;
	return true;
}

void playing_field_compress_empty_rows(int from_row)
{
	int nof_empty_rows = 0;
	for (int curr_line = from_row; curr_line >= peak; curr_line--)
		if (playing_field_is_empty_row(curr_line))
			nof_empty_rows++;
		else
			playing_field_copy_row(from_row--, curr_line);

	for (int i = 0; i < nof_empty_rows; i++) {
		playing_field_clear_row(peak);
		peak++;
	}
}

int playing_field_get_peak(void)
{
	return peak;
}

int playing_field_get_color(int x, int y)
{
	return playing_field[x][y];
}

struct block playing_field_spawn_random_powerup(void)
{
	int random_y = rand() % (PLAYING_FIELD_VSQUARES - peak) + peak;

	int nof_filled_x_blocks = 0;
	int filled_x_positions[PLAYING_FIELD_HSQUARES];
	for (int i = 0; i < PLAYING_FIELD_HSQUARES; i++)
		filled_x_positions[i] = -1;
	for (int i = 0; i < PLAYING_FIELD_HSQUARES; i++)
		if (playing_field[i][random_y] != HIDDEN &&
		    playing_field[i][random_y] != Grey &&
		    playing_field[i][random_y] != Blue2)
			filled_x_positions[nof_filled_x_blocks++] = i;
	int random_x = filled_x_positions[rand() % nof_filled_x_blocks];

	enum powerup random_powerup = rand() % 2;
	switch (random_powerup) {
	case CLEAR_LINES:
		playing_field[random_x][random_y] = Grey;
		break;
	case SLOW_DOWN:
		playing_field[random_x][random_y] = Blue2;
		break;
	}

	return (struct block){ random_x, random_y };
}

/********************
 * HELPER FUNCTIONS *
 ********************/

static void playing_field_copy_row(int dest_row, int src_row)
{
	for (int x = 0; x < PLAYING_FIELD_HSQUARES; x++)
		playing_field[x][dest_row] = playing_field[x][src_row];
}
