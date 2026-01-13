#include "gui.h"
#include "tetris/core/playing_field.h"

static void playing_field_draw_row(int row_num);

void playing_field_redraw(int from_row, int nof_cleared_lines)
{
	for (int i = from_row;
	     i >= playing_field_get_peak() - nof_cleared_lines; i--)
		playing_field_draw_row(i);
}

void playing_field_redraw_block(int x, int y)
{
	tetromino_draw_block((struct block){ x, y },
			     playing_field_get_color(x, y));
}

/********************
 * HELPER FUNCTIONS *
 ********************/

static void playing_field_draw_row(int row_num)
{
	for (int x = 0; x < PLAYING_FIELD_HSQUARES; x++)
		tetromino_draw_block((struct block){ x, row_num },
				     playing_field_get_color(x, row_num));
}
