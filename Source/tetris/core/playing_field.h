#ifndef PLAYING_FIELD_H_
#define PLAYING_FIELD_H_

#include <stdbool.h>

#include "block.h"
#include "tetromino.h"

#define PLAYING_FIELD_HSQUARES 10
#define PLAYING_FIELD_VSQUARES 20

#define PLAYING_FIELD_TOP 0
#define PLAYING_FIELD_BOTTOM (PLAYING_FIELD_VSQUARES - 1)
#define PLAYING_FIELD_LEFT_BORDER 0
#define PLAYING_FIELD_RIGHT_BORDER (PLAYING_FIELD_HSQUARES - 1)

extern enum block_color playing_field[PLAYING_FIELD_HSQUARES]
				     [PLAYING_FIELD_VSQUARES];
extern int peak;

void playing_field_reset(void);
void playing_field_clear_row(int line);
void playing_field_compress_empty_rows(int from_line);
bool playing_field_shift_up(int nof_blocks);

bool playing_field_is_empty_row(int line);
bool playing_field_is_complete_row(int line);
bool playing_field_is_empty(void);
bool playing_field_is_occupied(int pos_x, int pos_y);

void playing_field_add_tetromino(volatile struct tetromino *tetromino);

#endif // PLAYING_FIELD_H_
