#ifndef TETROMINO_H_
#define TETROMINO_H_

#include <stdbool.h>

#include "GLCD/GLCD.h"
#include "playing_field/playing_field.h"

#define NOF_TETROMINOS 7
#define BLOCK_LEN (PLAYING_FIELD_WIDTH_PX / 10) // pixels

enum tetromino_type {
	I_TYPE,
	O_TYPE,
	T_TYPE,
	J_TYPE,
	L_TYPE,
	S_TYPE,
	Z_TYPE,
};

enum tetromino_color {
	I_COLOR = Cyan,
	O_COLOR = Yellow,
	T_COLOR = Magenta,
	J_COLOR = Blue,
	L_COLOR = Orange,
	S_COLOR = Green,
	Z_COLOR = Red,
};

struct block {
	int x;
	int y;
};

struct tetromino {
	struct block blocks[4];
	enum tetromino_type type;
	enum tetromino_color color;
	bool placed;
};

// creation
struct tetromino tetromino_create(enum tetromino_type type,
				  struct block position);

// visualization
void tetromino_draw(struct tetromino tetromino);
void tetromino_hide(struct tetromino tetromino);

// movement
void tetromino_move_left(struct tetromino *tetromino);
void tetromino_move_right(struct tetromino *tetromino);
void tetromino_move_down(struct tetromino *tetromino);
void tetromino_hard_drop(struct tetromino *tetromino);
void tetromino_rotate_clockwise(struct tetromino *tetromino);

#endif // TETROMINO_H_
