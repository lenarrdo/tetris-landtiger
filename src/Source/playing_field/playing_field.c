#include <stdbool.h>
#include <stdint.h>

#include "playing_field.h"
#include "tetromino/tetromino.h"
#include "GLCD/GLCD.h"

struct pixel {
	int x;
	int y;
};

static void draw_horizontal_wall(struct pixel from, int length);
static void draw_vertical_wall(struct pixel from, int length);

// playing_field[i][j] == true <==> playing_field[i][j] is occupied by another tetromino
bool playing_field[PLAYING_FIELD_HSQUARES][PLAYING_FIELD_VSQUARES];

void playing_field_draw(void)
{
	LCD_Clear(Black);

	draw_horizontal_wall((struct pixel){ 0, 0 }, MAX_X);
	draw_horizontal_wall(
		(struct pixel){ 0, MAX_Y - HORIZONTAL_BORDER_WIDTH }, MAX_X);

	draw_vertical_wall((struct pixel){ 0, 0 }, MAX_Y);
	draw_vertical_wall(
		(struct pixel){ VERTICAL_BORDER_WIDTH + PLAYING_FIELD_WIDTH_PX,
				0 },
		MAX_Y);
	draw_vertical_wall((struct pixel){ MAX_X - VERTICAL_BORDER_WIDTH, 0 },
			   MAX_Y);

	GUI_Text(170, 90, (uint8_t *)"TOP", White, Black);
	GUI_Text(170, 110, (uint8_t *)"000000", White, Black);

	GUI_Text(170, 140, (uint8_t *)"SCORE", White, Black);
	GUI_Text(170, 160, (uint8_t *)"000000", White, Black);

	GUI_Text(170, 190, (uint8_t *)"LINES", White, Black);
	GUI_Text(170, 210, (uint8_t *)"000000", White, Black);
}

void playing_field_reset(void)
{
	for (int i = 0; i < PLAYING_FIELD_HSQUARES; i++)
		for (int j = 0; j < PLAYING_FIELD_VSQUARES; j++)
			playing_field[i][j] = false;
}

bool playing_field_is_occupied_or_bottom(int pos_x, int pos_y)
{
	return playing_field[pos_x][pos_y] || pos_y == PLAYING_FIELD_VSQUARES;
}

void place_tetromino(struct tetromino *tetromino)
{
	for (int i = 0; i < 4; i++)
		playing_field[tetromino->squares[i].x][tetromino->squares[i].y] =
			true;
}

void remove_tetromino(struct tetromino *tetromino)
{
	for (int i = 0; i < 4; i++)
		playing_field[tetromino->squares[i].x][tetromino->squares[i].y] =
			false;
}

/********************
 * HELPER FUNCTIONS *
 ********************/

static void draw_horizontal_wall(struct pixel from, int length)
{
	for (int i = 0; i < HORIZONTAL_BORDER_WIDTH; i++)
		LCD_DrawLine(from.x, from.y + i, from.x + length, from.y + i,
			     BORDER_COLOR);
}

static void draw_vertical_wall(struct pixel from, int length)
{
	for (int i = 0; i < VERTICAL_BORDER_WIDTH; i++)
		LCD_DrawLine(from.x + i, from.y, from.x + i, from.y + length,
			     BORDER_COLOR);
}
