#ifndef GAME_STATS_H_
#define GAME_STATS_H_

#include <stdbool.h>

enum game_state { PAUSED, PLAYING, GAME_OVER };

extern int high_score;
extern int score;
extern int tot_cleared_lines;
extern enum game_state game_state;
extern struct tetromino current_tetromino;
extern unsigned int buffered_move;

#endif // GAME_STATS_H_
