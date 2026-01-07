#include "stats.h"
#include "tetromino.h"

volatile int high_score;
volatile int score;
volatile int tot_cleared_lines;

volatile enum game_state game_state;
volatile struct tetromino current_tetromino;
volatile unsigned int buffered_move;

volatile int lowest_cleared_line;
