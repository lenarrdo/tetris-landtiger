#include <stdbool.h>
#include <stdint.h>

#include "playing_field.h"
#include "tetris/tetromino.h"
#include "GLCD/GLCD.h"
#include "tetris/game_stats.h"

#define MAX_DIGITS 6

static void draw_horizontal_wall(int x_pixel, int y_pixel, int length);
static void draw_vertical_wall(int x_pixel, int y_pixel, int length);
static void playing_field_copy_row(int dest_line, int src_line);
static void playing_field_draw_row(int line);
static void visualize_stat(int x, int y, int num);

enum tetromino_color playing_field[PLAYING_FIELD_HSQUARES]
				  [PLAYING_FIELD_VSQUARES];
int peak;

void playing_field_draw(void)
{
	LCD_Clear(BACKGROUND_COLOR);

	draw_horizontal_wall(0, 0, MAX_X);
	draw_horizontal_wall(0, MAX_Y - HORIZONTAL_BORDER_WIDTH, MAX_X);

	draw_vertical_wall(0, 0, MAX_Y);
	draw_vertical_wall(VERTICAL_BORDER_WIDTH + PLAYING_FIELD_WIDTH_PX, 0,
			   MAX_Y);
	draw_vertical_wall(MAX_X - VERTICAL_BORDER_WIDTH, 0, MAX_Y);

	GUI_Text(170, 90, (uint8_t *)"TOP", TEXT_COLOR, BACKGROUND_COLOR);
	visualize_top();

	GUI_Text(170, 140, (uint8_t *)"SCORE", TEXT_COLOR, BACKGROUND_COLOR);
	visualize_score();

	GUI_Text(170, 190, (uint8_t *)"LINES", TEXT_COLOR, BACKGROUND_COLOR);
	visualize_lines();
}

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

// FIXME: nof_cleared_lines is too artificial, unclear.
void playing_field_redraw(int from_row, int nof_cleared_lines)
{
	for (int i = from_row; i >= peak - nof_cleared_lines; i--)
		playing_field_draw_row(i);
}

void visualize_top(void)
{
	visualize_stat(170, 110, high_score);
}

void visualize_score(void)
{
	visualize_stat(170, 160, score);
}

void visualize_lines(void)
{
	visualize_stat(170, 210, tot_cleared_lines);
}

/********************
 * HELPER FUNCTIONS *
 ********************/

static void draw_horizontal_wall(int x_pixel, int y_pixel, int length)
{
	for (int i = 0; i < HORIZONTAL_BORDER_WIDTH; i++)
		LCD_DrawLine(x_pixel, y_pixel + i, x_pixel + length,
			     y_pixel + i, BORDER_COLOR);
}

static void draw_vertical_wall(int x_pixel, int y_pixel, int length)
{
	for (int i = 0; i < VERTICAL_BORDER_WIDTH; i++)
		LCD_DrawLine(x_pixel + i, y_pixel, x_pixel + i,
			     y_pixel + length, BORDER_COLOR);
}

static void playing_field_copy_row(int dest_row, int src_row)
{
	for (int x = 0; x < PLAYING_FIELD_HSQUARES; x++)
		playing_field[x][dest_row] = playing_field[x][src_row];
}

// WARNING: duplicate function
static void playing_field_draw_cell(int pos_x, int pos_y)
{
	struct block upper_left = { pos_x * BLOCK_LEN + VERTICAL_BORDER_WIDTH,
				    pos_y * BLOCK_LEN +
					    HORIZONTAL_BORDER_WIDTH };

	for (int x = upper_left.x; x < upper_left.x + BLOCK_LEN; x++)
		for (int y = upper_left.y; y < upper_left.y + BLOCK_LEN; y++)
			LCD_SetPoint(x, y, playing_field[pos_x][pos_y]);
}

static void playing_field_draw_row(int row_num)
{
	for (int x = 0; x < PLAYING_FIELD_HSQUARES; x++)
		playing_field_draw_cell(x, row_num);
}

static void itoa(int n, char *s)
{
	int i = 0;
	s[MAX_DIGITS] = '\0';

	// maxout
	if (n >= 999999) {
		while (i < MAX_DIGITS)
			s[i++] = '9';
		return;
	}

	do {
		s[i++] = n % 10 + '0';
	} while ((n /= 10) > 0);

	while (i < MAX_DIGITS)
		s[i++] = '0';

	char temp;
	for (int j = 0, k = i - 1; j < k; j++, k--) {
		temp = s[j];
		s[j] = s[k];
		s[k] = temp;
	}
}

static void visualize_stat(int x, int y, int num)
{
	char strnum[10];
	itoa(num, strnum);
	GUI_Text(x, y, (uint8_t *)strnum, TEXT_COLOR, BACKGROUND_COLOR);
}
