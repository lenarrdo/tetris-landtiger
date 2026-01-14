#include "gui.h"
#include "tetris/core/tetromino.h"

void tetromino_draw(struct tetromino tetromino)
{
	if (tetromino.color == HIDDEN)
		return;

	for (int i = 0; i < 4; i++)
		block_draw(tetromino.blocks[i], tetromino.color);
}

void tetromino_hide(struct tetromino tetromino)
{
	for (int i = 0; i < 4; i++)
		block_draw(tetromino.blocks[i], HIDDEN);
}
