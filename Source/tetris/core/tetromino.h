#ifndef TETROMINO_H_
#define TETROMINO_H_

#include <stdbool.h>

#include "block.h"

#define NOF_TETROMINOS 7

enum tetromino_type {
	I_TYPE,
	O_TYPE,
	T_TYPE,
	J_TYPE,
	L_TYPE,
	S_TYPE,
	Z_TYPE,
};

enum tetromino_moves {
	NONE = 0,
	MOVE_LEFT = 1,
	MOVE_RIGHT = 1 << 1,
	MOVE_DOWN = 1 << 2,
	ROTATE = 1 << 3,
	HARD_DROP = 1 << 4
};

struct tetromino {
	struct block blocks[4];
	enum tetromino_type type;
	enum block_color color;
	bool placed;
};

struct tetromino tetromino_create(enum tetromino_type type,
				  struct block position);

void tetromino_move_left(volatile struct tetromino *tetromino);
void tetromino_move_right(volatile struct tetromino *tetromino);
void tetromino_move_down(volatile struct tetromino *tetromino);
void tetromino_hard_drop(volatile struct tetromino *tetromino);
void tetromino_rotate_clockwise(volatile struct tetromino *tetromino);

#endif // TETROMINO_H_
