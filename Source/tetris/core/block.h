#ifndef BLOCK_H_
#define BLOCK_H_

#include <stdbool.h>

#include "GLCD/GLCD.h"

struct block {
	int x;
	int y;
};

enum block_color {
	I_COLOR = Cyan,
	O_COLOR = Yellow,
	T_COLOR = Magenta,
	J_COLOR = Blue,
	L_COLOR = Orange,
	S_COLOR = Green,
	Z_COLOR = Red,

	CLEAR_LINES_COLOR = Grey,
	SLOW_DOWN_COLOR = Blue2,

	HIDDEN = Black,
};

bool is_block_out_of_bounds(struct block block);

#endif // BLOCK_H_
