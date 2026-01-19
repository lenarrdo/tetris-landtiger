#ifndef MUSIC_H_
#define MUSIC_H_

#include <stdbool.h>
#include <stdint.h>

// #define FREQUENCY_SCALER 0.3
#define SLOWDOWN 2
#define SECOND 25000000
#define NOF_NOTES_A_TYPE 41

enum note_duration {
	semibiscroma = (unsigned int)(SECOND * SLOWDOWN / 64.0f),
	biscroma = (unsigned int)(SECOND * SLOWDOWN / 32.0f),
	semicroma = (unsigned int)(SECOND * SLOWDOWN / 16.0f),
	croma = (unsigned int)(SECOND * SLOWDOWN / 8.0f),
	semiminima = (unsigned int)(SECOND * SLOWDOWN / 4.0f),
	minima = (unsigned int)(SECOND * SLOWDOWN / 2.0f),
	semibreve = (unsigned int)(SECOND * SLOWDOWN),
};

enum note_frequency {
	A0b = 18969,
	B0 = 17747,
	C1 = 16671,
	D1b = 15889,
	D1 = 14997,
	E1 = 13420,
	F1 = 12503,
	G1b = 11960,
	G1 = 11227,
	A1b = 10579,
	A1 = 10003,
	B1 = 8873,
	C2 = 8414,
	D2b = 7944,
	D2 = 7498,
	E2 = 6711,
	F2 = 6321,
	G2b = 5980,
	G2 = 5614,
	A2b = 5351,
	A2 = 5001,
	B2 = 4500,
	C3 = 4240,
	D3b = 3972,
	D3 = 3779,
	E3 = 3367,
	F3 = 3175,
	G3b = 2974,
	G3 = 2834,
	A3b = 2670,
	A3 = 2525,
	B3 = 2249,
	C4 = 2120,
	D4b = 1986,
	D4 = 1890,
	E4 = 1684,
	F4 = 1592,
	G4b = 1487,
	G4 = 1417,
	A4 = 1263,
	B4 = 1125,
	C5 = 1062,
	PAUSE = 0
};

struct note {
	enum note_frequency freq;
	enum note_duration duration;
};

extern uint16_t sine_table[45];
extern struct note a_type_theme[NOF_NOTES_A_TYPE];
extern int music_tick;
extern int current_note;

void set_note(struct note note);
void play_note(void);

#endif // MUSIC_H_
