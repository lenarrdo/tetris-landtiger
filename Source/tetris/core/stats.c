#include "stats.h"

volatile int high_score;
volatile int score;
volatile int tot_cleared_lines;

volatile struct tetromino current_tetromino;
volatile unsigned int current_tetromino_speed;
volatile unsigned int previous_tetromino_speed;

volatile enum game_state game_state;
volatile unsigned int buffered_move;
volatile bool slow_down_active;
