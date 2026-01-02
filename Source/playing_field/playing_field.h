#ifndef PLAYING_FIELD_H_
#define PLAYING_FIELD_H_

#include <stdbool.h>

struct tetromino;

#define BORDER_COLOR White
#define BACKGROUND_COLOR Black
#define TEXT_COLOR White

#define HORIZONTAL_BORDER_WIDTH 10
#define VERTICAL_BORDER_WIDTH 5

#define PLAYING_FIELD_HSQUARES 10
#define PLAYING_FIELD_VSQUARES 20

#define PLAYING_FIELD_WIDTH_PX 150
#define PLAYING_FIELD_HEIGHT_PX 300

#define PLAYING_FIELD_TOP 0
#define PLAYING_FIELD_BOTTOM (PLAYING_FIELD_VSQUARES - 1)
#define PLAYING_FIELD_LEFT_BORDER 0
#define PLAYING_FIELD_RIGHT_BORDER (PLAYING_FIELD_HSQUARES - 1)

void playing_field_draw(void);
void playing_field_redraw(int from_row, int nof_cleared_lines);
void playing_field_reset(void);
void playing_field_clear_row(int line);
void playing_field_compress_empty_rows(int from_line);

bool playing_field_is_empty_row(int line);
bool playing_field_is_complete_row(int line);

bool playing_field_is_occupied(int pos_x, int pos_y);

void playing_field_add_tetromino(struct tetromino *tetromino);

#endif // PLAYING_FIELD_H_
