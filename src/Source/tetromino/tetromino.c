#include "tetromino.h"
#include "playing_field/playing_field.h"
#include "GLCD/GLCD.h"

static void draw_block(struct block square, enum tetromino_color color);
static void tetromino_shift(struct tetromino *tetromino, struct block vector);

struct tetromino tetromino_create(enum tetromino_type type,
				  struct block position)
{
	struct tetromino new_tetromino;
	new_tetromino.type = type;
	new_tetromino.placed = false;

	switch (type) {
	case I_TYPE:
		new_tetromino.blocks[0] =
			(struct block){ position.x, position.y };
		new_tetromino.blocks[1] =
			(struct block){ position.x + 1, position.y };
		new_tetromino.blocks[2] =
			(struct block){ position.x + 2, position.y };
		new_tetromino.blocks[3] =
			(struct block){ position.x + 3, position.y };

		new_tetromino.color = I_COLOR;
		break;
	case O_TYPE:
		new_tetromino.blocks[0] =
			(struct block){ position.x, position.y };
		new_tetromino.blocks[1] =
			(struct block){ position.x, position.y + 1 };
		new_tetromino.blocks[2] =
			(struct block){ position.x + 1, position.y };
		new_tetromino.blocks[3] =
			(struct block){ position.x + 1, position.y + 1 };

		new_tetromino.color = O_COLOR;
		break;
	case T_TYPE:
		new_tetromino.blocks[0] =
			(struct block){ position.x + 1, position.y };
		new_tetromino.blocks[1] =
			(struct block){ position.x, position.y + 1 };
		new_tetromino.blocks[2] =
			(struct block){ position.x + 1, position.y + 1 };
		new_tetromino.blocks[3] =
			(struct block){ position.x + 2, position.y + 1 };

		new_tetromino.color = T_COLOR;
		break;
	case J_TYPE:
		new_tetromino.blocks[0] =
			(struct block){ position.x, position.y };
		new_tetromino.blocks[1] =
			(struct block){ position.x, position.y + 1 };
		new_tetromino.blocks[2] =
			(struct block){ position.x + 1, position.y + 1 };
		new_tetromino.blocks[3] =
			(struct block){ position.x + 2, position.y + 1 };

		new_tetromino.color = J_COLOR;
		break;
	case L_TYPE:
		new_tetromino.blocks[0] =
			(struct block){ position.x + 2, position.y };
		new_tetromino.blocks[1] =
			(struct block){ position.x, position.y + 1 };
		new_tetromino.blocks[2] =
			(struct block){ position.x + 1, position.y + 1 };
		new_tetromino.blocks[3] =
			(struct block){ position.x + 2, position.y + 1 };

		new_tetromino.color = L_COLOR;
		break;
	case S_TYPE:
		new_tetromino.blocks[0] =
			(struct block){ position.x + 1, position.y };
		new_tetromino.blocks[1] =
			(struct block){ position.x + 2, position.y };
		new_tetromino.blocks[2] =
			(struct block){ position.x + 1, position.y + 1 };
		new_tetromino.blocks[3] =
			(struct block){ position.x, position.y + 1 };

		new_tetromino.color = S_COLOR;
		break;
	case Z_TYPE:
		new_tetromino.blocks[0] =
			(struct block){ position.x, position.y };
		new_tetromino.blocks[1] =
			(struct block){ position.x + 1, position.y };
		new_tetromino.blocks[2] =
			(struct block){ position.x + 1, position.y + 1 };
		new_tetromino.blocks[3] =
			(struct block){ position.x + 2, position.y + 1 };

		new_tetromino.color = Z_COLOR;
		break;
	}

	return new_tetromino;
}

void tetromino_draw(struct tetromino tetromino)
{
	for (int i = 0; i < 4; i++)
		draw_block(tetromino.blocks[i], tetromino.color);
}

void tetromino_hide(struct tetromino *tetromino)
{
	for (int i = 0; i < 4; i++)
		draw_block(tetromino->blocks[i], Black);
}

void tetromino_move_left(struct tetromino *tetromino)
{
	for (int i = 0; i < 4; i++)
		if (tetromino->blocks[i].x == 0)
			return;

	tetromino_shift(tetromino, (struct block){ -1, 0 });
}

void tetromino_move_right(struct tetromino *tetromino)
{
	for (int i = 0; i < 4; i++)
		if (tetromino->blocks[i].x == PLAYING_FIELD_HSQUARES - 1)
			return;

	tetromino_shift(tetromino, (struct block){ 1, 0 });
}

void tetromino_move_down(struct tetromino *tetromino)
{
	for (int i = 0; i < 4; i++)
		if (playing_field_is_occupied(tetromino->blocks[i].x,
					      tetromino->blocks[i].y + 1) ||
		    tetromino->blocks[i].y == PLAYING_FIELD_BOTTOM) {
			playing_field_add_tetromino(tetromino);
			tetromino->placed = true;
			return;
		}

	tetromino_shift(tetromino, (struct block){ 0, 1 });
}

void tetromino_hard_drop(struct tetromino *tetromino)
{
	while (!tetromino->placed)
		tetromino_move_down(tetromino);
}

void tetromino_rotate_clockwise(struct tetromino *tetromino)
{
	if (tetromino->type == O_TYPE)
		return;

	struct block pivot = { tetromino->blocks[2].x, tetromino->blocks[2].y };

	for (int i = 0; i < 4; i++) {
		int dx = tetromino->blocks[i].x - pivot.x;
		int dy = tetromino->blocks[i].y - pivot.y;

		tetromino->blocks[i].x = -dy + pivot.x;
		tetromino->blocks[i].y = +dx + pivot.y;
	}
}

/********************
 * HELPER FUNCTIONS *
 ********************/

static void draw_block(struct block block, enum tetromino_color color)
{
	struct block upper_left = { block.x * BLOCK_LEN + VERTICAL_BORDER_WIDTH,
				    block.y * BLOCK_LEN +
					    HORIZONTAL_BORDER_WIDTH };

	for (int i = upper_left.x; i < upper_left.x + BLOCK_LEN; i++)
		for (int j = upper_left.y; j < upper_left.y + BLOCK_LEN; j++)
			LCD_SetPoint(i, j, color);
}

static void tetromino_shift(struct tetromino *tetromino, struct block offset)
{
	for (int i = 0; i < 4; i++) {
		tetromino->blocks[i].x += offset.x;
		tetromino->blocks[i].y += offset.y;
	}
}
