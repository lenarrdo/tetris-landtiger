#include "playing_field.h"

enum block_color playing_field[PLAYING_FIELD_HSQUARES][PLAYING_FIELD_VSQUARES];
int peak;

static void playing_field_copy_row(int dest_line, int src_line);

void playing_field_reset(void)
{
	peak = PLAYING_FIELD_VSQUARES;
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

bool playing_field_is_empty(void)
{
	return peak == PLAYING_FIELD_VSQUARES;
}

bool playing_field_shift_up(int nof_blocks)
{
	if (!playing_field_is_empty_row(PLAYING_FIELD_TOP))
		return false;

	for (int row = 1; row <= PLAYING_FIELD_VSQUARES; row++)
		playing_field_copy_row(row - nof_blocks, row);
	playing_field_clear_row(PLAYING_FIELD_BOTTOM);

	peak--;

	return true;
}

/********************
 * HELPER FUNCTIONS *
 ********************/

static void playing_field_copy_row(int dest_row, int src_row)
{
	for (int x = 0; x < PLAYING_FIELD_HSQUARES; x++)
		playing_field[x][dest_row] = playing_field[x][src_row];
}
