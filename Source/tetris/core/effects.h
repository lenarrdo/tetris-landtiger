#ifndef EFFECTS_H_
#define EFFECTS_H_

#define NOF_POWERUPS 2
#define NOF_LINES_FOR_POWERUP 5

enum powerup {
	CLEAR_LINES,
	SLOW_DOWN,
};

struct block playing_field_spawn_random_powerup(void);

#endif // EFFECTS_H_
