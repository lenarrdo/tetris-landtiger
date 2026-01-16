#include "tetromino.h"
#include "playing_field.h"

static void tetromino_shift(volatile struct tetromino *tetromino,
			    struct block vector);

struct tetromino tetromino_create(enum tetromino_type type,
				  struct block position)
{
	struct tetromino new_tetromino = { 0 };
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

void tetromino_move_left(volatile struct tetromino *tetromino)
{
	for (int i = 0; i < 4; i++)
		if (tetromino->blocks[i].x == PLAYING_FIELD_LEFT_BORDER ||
		    playing_field_is_occupied(tetromino->blocks[i].x - 1,
					      tetromino->blocks[i].y))
			return;

	tetromino_shift(tetromino, (struct block){ -1, 0 });
}

void tetromino_move_right(volatile struct tetromino *tetromino)
{
	for (int i = 0; i < 4; i++)
		if (tetromino->blocks[i].x == PLAYING_FIELD_RIGHT_BORDER ||
		    playing_field_is_occupied(tetromino->blocks[i].x + 1,
					      tetromino->blocks[i].y))
			return;

	tetromino_shift(tetromino, (struct block){ 1, 0 });
}

void tetromino_move_down(volatile struct tetromino *tetromino)
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

void tetromino_hard_drop(volatile struct tetromino *tetromino)
{
	while (!tetromino->placed)
		tetromino_move_down(tetromino);
}

void tetromino_rotate_clockwise(volatile struct tetromino *tetromino)
{
	if (tetromino->type == O_TYPE)
		return;

	struct tetromino tmp_tetromino = *tetromino;
	struct block pivot = { tmp_tetromino.blocks[2].x,
			       tmp_tetromino.blocks[2].y };

	for (int i = 0; i < 4; i++) {
		int dx = tmp_tetromino.blocks[i].x - pivot.x;
		int dy = tmp_tetromino.blocks[i].y - pivot.y;

		tmp_tetromino.blocks[i].x = -dy + pivot.x;
		tmp_tetromino.blocks[i].y = +dx + pivot.y;

		if (is_block_out_of_bounds(tmp_tetromino.blocks[i]) ||
		    playing_field_is_occupied(tmp_tetromino.blocks[i].x,
					      tmp_tetromino.blocks[i].y))
			return;
	}

	*tetromino = tmp_tetromino;
}

/********************
 * HELPER FUNCTIONS *
 ********************/

static void tetromino_shift(volatile struct tetromino *tetromino,
			    struct block offset)
{
	for (int i = 0; i < 4; i++) {
		tetromino->blocks[i].x += offset.x;
		tetromino->blocks[i].y += offset.y;
	}
}
