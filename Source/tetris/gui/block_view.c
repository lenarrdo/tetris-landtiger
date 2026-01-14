#include "gui.h"
#include "tetris/core/block.h"

void block_draw(struct block block, enum block_color color)
{
	struct block upper_left = { block.x * BLOCK_LEN + VERTICAL_BORDER_WIDTH,
				    block.y * BLOCK_LEN +
					    HORIZONTAL_BORDER_WIDTH };

	for (int i = upper_left.x; i < upper_left.x + BLOCK_LEN; i++)
		for (int j = upper_left.y; j < upper_left.y + BLOCK_LEN; j++)
			LCD_SetPoint(i, j, color);
}
