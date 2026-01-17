#include "stats.h"

volatile int high_score;
volatile int score;
volatile int tot_cleared_lines;

volatile struct tetromino curr_tetromino;
volatile unsigned int curr_tetromino_speed;
volatile unsigned int prev_tetromino_speed;

volatile enum game_state game_state;
volatile unsigned int buffered_move;
volatile bool slow_down_active;
