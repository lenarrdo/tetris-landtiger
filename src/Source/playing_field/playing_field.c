#include "playing_field.h"
#include "../GLCD/GLCD.h"

struct pixel {
	int x;
	int y;
};

static void draw_horizontal_wall(struct pixel from, int length);
static void draw_vertical_wall(struct pixel from, int length);

void draw_playing_field(void)
{
	LCD_Clear(Black);

	draw_horizontal_wall((struct pixel){ 0, 0 }, MAX_X);
	draw_horizontal_wall(
		(struct pixel){ 0, MAX_Y - HORIZONTAL_BORDER_WIDTH }, MAX_X);

	draw_vertical_wall((struct pixel){ 0, 0 }, MAX_Y);
	draw_vertical_wall((struct pixel){ PLAYING_FIELD_WIDTH + 1, 0 }, MAX_Y);
	draw_vertical_wall((struct pixel){ MAX_X - VERTICAL_BORDER_WIDTH, 0 },
			   MAX_Y);

	GUI_Text(170, 90, (uint8_t *)"TOP", White, Black);
	GUI_Text(170, 110, (uint8_t *)"000000", White, Black);

	GUI_Text(170, 140, (uint8_t *)"SCORE", White, Black);
	GUI_Text(170, 160, (uint8_t *)"000000", White, Black);

	GUI_Text(170, 190, (uint8_t *)"LINES", White, Black);
	GUI_Text(170, 210, (uint8_t *)"000000", White, Black);
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
