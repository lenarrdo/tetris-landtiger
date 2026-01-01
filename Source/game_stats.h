#ifndef GAME_STATS_H_
#define GAME_STATS_H_

enum game_state { PAUSED, PLAYING, GAME_OVER };

extern int high_score;
extern int score;
extern int tot_cleared_lines;
extern enum game_state game_state;
extern struct tetromino current_tetromino;

#endif // GAME_STATS_H_
