#include "tetris/core/tetromino.h"
#include "tetris/gui/gui.h"

void tetromino_draw_block(struct block block, enum tetromino_color color)
{
	struct block upper_left = { block.x * BLOCK_LEN + VERTICAL_BORDER_WIDTH,
				    block.y * BLOCK_LEN +
					    HORIZONTAL_BORDER_WIDTH };

	for (int i = upper_left.x; i < upper_left.x + BLOCK_LEN; i++)
		for (int j = upper_left.y; j < upper_left.y + BLOCK_LEN; j++)
			LCD_SetPoint(i, j, color);
}

void tetromino_draw(struct tetromino tetromino)
{
	if (tetromino.color == HIDDEN)
		return;

	for (int i = 0; i < 4; i++)
		tetromino_draw_block(tetromino.blocks[i], tetromino.color);
}

void tetromino_hide(struct tetromino tetromino)
{
	for (int i = 0; i < 4; i++)
		tetromino_draw_block(tetromino.blocks[i], HIDDEN);
}
