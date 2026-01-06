#include <stdint.h>

#include "GLCD.h"
#include "gui.h"

static void draw_horizontal_wall(int x_pixel, int y_pixel, int length);
static void draw_vertical_wall(int x_pixel, int y_pixel, int length);

void init_playing_borders(void)
{
	LCD_Clear(BACKGROUND_COLOR);

	draw_horizontal_wall(0, 0, MAX_X);
	draw_horizontal_wall(0, MAX_Y - HORIZONTAL_BORDER_WIDTH, MAX_X);

	draw_vertical_wall(0, 0, MAX_Y);
	draw_vertical_wall(VERTICAL_BORDER_WIDTH + PLAYING_FIELD_WIDTH_PX, 0,
			   MAX_Y);
	draw_vertical_wall(MAX_X - VERTICAL_BORDER_WIDTH, 0, MAX_Y);
}

void show_paused_popup(void)
{
	uint8_t msg[4][9] = {
		"--------",
		"  GAME  ",
		" PAUSED ",
		"--------",
	};
	for (int i = 0; i < 4; i++)
		GUI_Text(165, 240 + 15 * i, msg[i], Red, BACKGROUND_COLOR);
}

void show_game_over_popup(void)
{
	uint8_t msg[4][9] = {
		"--------",
		"  GAME  ",
		"  OVER  ",
		"--------",
	};
	for (int i = 0; i < 4; i++)
		GUI_Text(165, 240 + 15 * i, msg[i], Red, BACKGROUND_COLOR);
}

void hide_popup(void)
{
	uint8_t msg[4][9] = {
		"        ",
		"        ",
		"        ",
		"        ",
	};
	for (int i = 0; i < 4; i++)
		GUI_Text(165, 240 + 15 * i, msg[i], BACKGROUND_COLOR,
			 BACKGROUND_COLOR);
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
