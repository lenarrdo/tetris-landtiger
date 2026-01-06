#ifndef GAME_STATS_H_
#define GAME_STATS_H_

#include <stdbool.h>

enum game_state { PAUSED, PLAYING, GAME_OVER };

extern volatile int high_score;
extern volatile int score;
extern volatile int tot_cleared_lines;
extern volatile enum game_state game_state;
extern volatile struct tetromino current_tetromino;
extern volatile unsigned int buffered_move;

#endif // GAME_STATS_H_
