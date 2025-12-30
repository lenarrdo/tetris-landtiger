#include "tetromino.h"
#include "../GLCD/GLCD.h"
#include "../playing_field/playing_field.h"

static void draw_block(struct square square, enum tetromino_color color);
static void tetromino_hide(struct tetromino *tetromino);
static void tetromino_shift(struct tetromino *tetromino, struct square vector);

struct tetromino tetromino_create(enum tetromino_type type,
				  struct square position)
{
	struct tetromino new_tetromino;
	new_tetromino.type = type;
	new_tetromino.placed = false;

	switch (type) {
	case I_TYPE:
		new_tetromino.squares[0] =
			(struct square){ position.x, position.y };
		new_tetromino.squares[1] =
			(struct square){ position.x + 1, position.y };
		new_tetromino.squares[2] =
			(struct square){ position.x + 2, position.y };
		new_tetromino.squares[3] =
			(struct square){ position.x + 3, position.y };

		new_tetromino.color = I_COLOR;
		break;
	case O_TYPE:
		new_tetromino.squares[0] =
			(struct square){ position.x, position.y };
		new_tetromino.squares[1] =
			(struct square){ position.x, position.y + 1 };
		new_tetromino.squares[2] =
			(struct square){ position.x + 1, position.y };
		new_tetromino.squares[3] =
			(struct square){ position.x + 1, position.y + 1 };

		new_tetromino.color = O_COLOR;
		break;
	case T_TYPE:
		new_tetromino.squares[0] =
			(struct square){ position.x + 1, position.y };
		new_tetromino.squares[1] =
			(struct square){ position.x, position.y + 1 };
		new_tetromino.squares[2] =
			(struct square){ position.x + 1, position.y + 1 };
		new_tetromino.squares[3] =
			(struct square){ position.x + 2, position.y + 1 };

		new_tetromino.color = T_COLOR;
		break;
	case J_TYPE:
		new_tetromino.squares[0] =
			(struct square){ position.x, position.y };
		new_tetromino.squares[1] =
			(struct square){ position.x, position.y + 1 };
		new_tetromino.squares[2] =
			(struct square){ position.x + 1, position.y + 1 };
		new_tetromino.squares[3] =
			(struct square){ position.x + 2, position.y + 1 };

		new_tetromino.color = J_COLOR;
		break;
	case L_TYPE:
		new_tetromino.squares[0] =
			(struct square){ position.x + 2, position.y };
		new_tetromino.squares[1] =
			(struct square){ position.x, position.y + 1 };
		new_tetromino.squares[2] =
			(struct square){ position.x + 1, position.y + 1 };
		new_tetromino.squares[3] =
			(struct square){ position.x + 2, position.y + 1 };

		new_tetromino.color = L_COLOR;
		break;
	case S_TYPE:
		new_tetromino.squares[0] =
			(struct square){ position.x + 1, position.y };
		new_tetromino.squares[1] =
			(struct square){ position.x + 2, position.y };
		new_tetromino.squares[2] =
			(struct square){ position.x + 1, position.y + 1 };
		new_tetromino.squares[3] =
			(struct square){ position.x, position.y + 1 };

		new_tetromino.color = S_COLOR;
		break;
	case Z_TYPE:
		new_tetromino.squares[0] =
			(struct square){ position.x, position.y };
		new_tetromino.squares[1] =
			(struct square){ position.x + 1, position.y };
		new_tetromino.squares[2] =
			(struct square){ position.x + 1, position.y + 1 };
		new_tetromino.squares[3] =
			(struct square){ position.x + 2, position.y + 1 };

		new_tetromino.color = Z_COLOR;
		break;
	}

	return new_tetromino;
}

void tetromino_draw(struct tetromino tetromino)
{
	for (int i = 0; i < 4; i++)
		draw_block(tetromino.squares[i], tetromino.color);
}

void tetromino_move_left(struct tetromino *tetromino)
{
	for (int i = 0; i < 4; i++)
		if (tetromino->squares[i].x == 0)
			return;

	tetromino_shift(tetromino, (struct square){ -1, 0 });
}

void tetromino_move_right(struct tetromino *tetromino)
{
	for (int i = 0; i < 4; i++)
		if (tetromino->squares[i].x == 9)
			return;

	tetromino_shift(tetromino, (struct square){ 1, 0 });
}

void tetromino_move_down(struct tetromino *tetromino)
{
	// TODO: check if tetromino is touching wall or another tetromino

	tetromino_shift(tetromino, (struct square){ 0, 1 });
}

void tetromino_rotate_clockwise(struct tetromino *tetromino)
{
	if (tetromino->type == O_TYPE)
		return;

	struct square pivot = { tetromino->squares[2].x,
				tetromino->squares[2].y };

	for (int i = 0; i < 4; i++) {
		int dx = tetromino->squares[i].x - pivot.x;
		int dy = tetromino->squares[i].y - pivot.y;

		tetromino->squares[i].x = -dy + pivot.x;
		tetromino->squares[i].y = +dx + pivot.y;
	}
}

/********************
 * HELPER FUNCTIONS *
 ********************/

static void draw_block(struct square square, enum tetromino_color color)
{
	struct square upper_left = {
		square.x * BLOCK_LEN + VERTICAL_BORDER_WIDTH,
		square.y * BLOCK_LEN + HORIZONTAL_BORDER_WIDTH
	};

	for (int i = upper_left.x; i < upper_left.x + BLOCK_LEN; i++)
		for (int j = upper_left.y; j < upper_left.y + BLOCK_LEN; j++)
			LCD_SetPoint(i, j, color);
}

static void tetromino_hide(struct tetromino *tetromino)
{
	for (int i = 0; i < 4; i++)
		draw_block(tetromino->squares[i], Black);
}

// TODO: change "struct square" name
static void tetromino_shift(struct tetromino *tetromino, struct square vector)
{
	tetromino_hide(tetromino);

	for (int i = 0; i < 4; i++) {
		tetromino->squares[i].x += vector.x;
		tetromino->squares[i].y += vector.y;
	}

	tetromino_draw(*tetromino);
}
