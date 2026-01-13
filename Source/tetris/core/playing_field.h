#ifndef PLAYING_FIELD_H_
#define PLAYING_FIELD_H_

#include <stdbool.h>

#define PLAYING_FIELD_HSQUARES 10
#define PLAYING_FIELD_VSQUARES 20

#define PLAYING_FIELD_TOP 0
#define PLAYING_FIELD_BOTTOM (PLAYING_FIELD_VSQUARES - 1)
#define PLAYING_FIELD_LEFT_BORDER 0
#define PLAYING_FIELD_RIGHT_BORDER (PLAYING_FIELD_HSQUARES - 1)

struct tetromino;
enum tetromino_color;

void playing_field_reset(void);
void playing_field_clear_row(int line);
void playing_field_compress_empty_rows(int from_line);

bool playing_field_is_empty_row(int line);
bool playing_field_is_complete_row(int line);

bool playing_field_is_occupied(int pos_x, int pos_y);

void playing_field_add_tetromino(volatile struct tetromino *tetromino);

int playing_field_get_peak(void);
int playing_field_get_color(int x, int y);
void playing_field_set_color(int x, int y, enum tetromino_color color);

#endif // PLAYING_FIELD_H_
