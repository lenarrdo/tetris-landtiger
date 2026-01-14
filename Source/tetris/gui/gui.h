#ifndef GUI_H_
#define GUI_H_

#include "tetris/core/block.h"
#include "tetris/core/tetromino.h"

#define BORDER_COLOR White
#define BACKGROUND_COLOR Black
#define TEXT_COLOR White

#define HORIZONTAL_BORDER_WIDTH 10
#define VERTICAL_BORDER_WIDTH 5

#define PLAYING_FIELD_WIDTH_PX 150
#define PLAYING_FIELD_HEIGHT_PX 300

#define BLOCK_LEN (PLAYING_FIELD_WIDTH_PX / 10)

// stats
void show_default_stats(void);
void visualize_top(void);
void visualize_score(void);
void visualize_lines(void);

// interfaces
void init_playing_borders(void);
void show_paused_popup(void);
void show_game_over_popup(void);
void hide_popup(void);

// tetromino
void tetromino_draw(struct tetromino tetromino);
void tetromino_hide(struct tetromino tetromino);

// playing field
void playing_field_redraw(int from_row, int nof_cleared_lines);
void playing_field_redraw_block(int x, int y);

// blocks
void block_draw(struct block block, enum block_color color);

#endif // GUI_H_
