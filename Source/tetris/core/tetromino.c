#include "tetromino.h"
#include "playing_field.h"
#include "stats.h"

#define MAX_CLEARABLE_LINES 4

static void tetromino_shift(volatile struct tetromino *tetromino,
			    struct block vector);
static void check_cleared_lines(volatile struct tetromino *tetromino);
static inline int max(int *arr, int size);

struct tetromino tetromino_create(enum tetromino_type type,
				  struct block position)
{
	struct tetromino new_tetromino = { 0 };
	new_tetromino.type = type;

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

	for (int i = 0; i < 4; i++)
		new_tetromino.placed |= playing_field_is_occupied(
			new_tetromino.blocks[i].x,
			new_tetromino.blocks[i].y + 1);

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
			score += 10;

			check_cleared_lines(tetromino);
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

bool is_block_out_of_bounds(struct block block)
{
	return block.x < PLAYING_FIELD_LEFT_BORDER ||
	       block.x > PLAYING_FIELD_RIGHT_BORDER ||
	       block.y < PLAYING_FIELD_TOP || block.y > PLAYING_FIELD_BOTTOM;
}

/********************
 * HELPER FUNCTIONS *
 ********************/

static void check_cleared_lines(volatile struct tetromino *tetromino)
{
	int cleared_lines[MAX_CLEARABLE_LINES] = { -1, -1, -1, -1 };
	int nof_cleared_lines = 0;
	for (int i = 0; i < 4; i++) {
		int curr_line = tetromino->blocks[i].y;
		if (playing_field_is_complete_row(curr_line)) {
			playing_field_clear_row(curr_line);
			cleared_lines[nof_cleared_lines++] = curr_line;
		}
	}

	if (nof_cleared_lines > 0) {
		score += nof_cleared_lines == MAX_CLEARABLE_LINES ?
				 600 : // tetris!
				 100 * nof_cleared_lines;
		tot_cleared_lines += nof_cleared_lines;

		lowest_cleared_line = max(cleared_lines, MAX_CLEARABLE_LINES);
		playing_field_compress_empty_rows(lowest_cleared_line);
	}
}

static void tetromino_shift(volatile struct tetromino *tetromino,
			    struct block offset)
{
	for (int i = 0; i < 4; i++) {
		tetromino->blocks[i].x += offset.x;
		tetromino->blocks[i].y += offset.y;
	}
}

static inline int max(int *arr, int size)
{
	int max = arr[0];
	for (int i = 1; i < size; i++)
		if (arr[i] > max)
			max = arr[i];
	return max;
}
