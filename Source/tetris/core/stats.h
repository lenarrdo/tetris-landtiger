#ifndef STATS_H_
#define STATS_H_

#include <stdbool.h>

#include "tetromino.h"

#define ONE_SECOND 25000000

enum game_state { PAUSED, PLAYING, GAME_OVER };

extern volatile int high_score;
extern volatile int score;
extern volatile int tot_cleared_lines;

extern volatile struct tetromino curr_tetromino;
extern volatile unsigned int curr_tetromino_speed;
extern volatile unsigned int prev_tetromino_speed;

extern volatile enum game_state game_state;
extern volatile unsigned int buffered_move;
extern volatile bool slow_down_active;

#endif // STATS_H_
