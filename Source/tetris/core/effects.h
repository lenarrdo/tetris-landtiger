#ifndef EFFECTS_H_
#define EFFECTS_H_

#define NOF_POWERUPS 2
#define NOF_LINES_FOR_POWERUP 5

enum powerup {
	CLEAR_LINES,
	SLOW_DOWN,
};

struct block effects_powerup_spawn(void);
int effects_powerup_clear_lines_activate(int row_trigger);

#endif // EFFECTS_H_
