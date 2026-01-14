#ifndef EFFECTS_H_
#define EFFECTS_H_

#define NOF_POWERUPS 2
#define NOF_LINES_FOR_POWERUP 5
#define NOF_LINES_RANDOM_MALUS 10

enum powerup {
	CLEAR_LINES,
	SLOW_DOWN,
};

struct block effects_powerup_spawn(void);
void effects_powerup_clear_lines_activate(int *cleared_lines,
					  int *nof_cleared_lines);

void effects_random_malus_activate(void);

#endif // EFFECTS_H_
