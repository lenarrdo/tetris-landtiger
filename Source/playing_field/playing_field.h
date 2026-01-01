#ifndef PLAYING_FIELD_H_
#define PLAYING_FIELD_H_

#include <stdbool.h>

struct tetromino;

#define BORDER_COLOR White

#define HORIZONTAL_BORDER_WIDTH 10
#define VERTICAL_BORDER_WIDTH 5

#define PLAYING_FIELD_HSQUARES 10
#define PLAYING_FIELD_VSQUARES 20

#define PLAYING_FIELD_WIDTH_PX 150
#define PLAYING_FIELD_HEIGHT_PX 300

#define PLAYING_FIELD_BOTTOM (PLAYING_FIELD_VSQUARES - 1)

void playing_field_draw(void);
void playing_field_reset(void);

bool playing_field_is_occupied(int pos_x, int pos_y);

void playing_field_add_tetromino(struct tetromino *tetromino);
void playing_field_remove_tetromino(struct tetromino *tetromino);

#endif // PLAYING_FIELD_H_
