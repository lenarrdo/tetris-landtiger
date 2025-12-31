#include <stdlib.h>
#include <time.h>

#include "LPC17xx.h"
#include "GLCD/GLCD.h"
#include "RIT/RIT.h"
#include "tetromino/tetromino.h"
#include "playing_field/playing_field.h"
#include "game_stats.h"
#include "button_EXINT/button.h"
#include "timer/timer.h"

// ScaleFlag needs to visible in order for the emulator to find the symbol (can
// be placed also inside system_LPC17xx.h but since it is RO, it needs more
// work)
#ifdef SIMULATOR
extern uint8_t ScaleFlag;
#endif

// #define ONE_SECOND (25000000)
#define ONE_SECOND (2500000)

int high_score;
int cleared_lines;
enum game_state game_state;
struct tetromino current_tetromino;

static inline void enter_power_down(void)
{
	LPC_SC->PCON |= (1U << 0);
	LPC_SC->PCON &= ~(1U << 1);

	__asm volatile("wfi");
}

int main(void)
{
	srand(time(NULL));
	SystemInit();
	LCD_Initialization();
	BUTTON_init();
	init_RIT(0x004C4B40); // RIT Initialization 50 msec
	enable_RIT();

	// game_state = PAUSED;
	// LCD_Clear(Blue);
	// while (game_state == PAUSED)
	// 	enter_power_down();

	/***************
	 * GAME BEGINS *
	 ***************/

	high_score = 0;
	cleared_lines = 0;

	playing_field_reset();
	playing_field_draw();

	init_timer(TIMER0, 0, 0, CONTROL_RESET | CONTROL_INTERRUPT, ONE_SECOND);
	enable_timer(TIMER0);

	game_state = PLAYING;
	do {
		current_tetromino = tetromino_create(rand() % NOF_TETROMINOS,
						     (struct square){ 3, 0 });
		tetromino_draw(current_tetromino);

		while (!current_tetromino.placed)
			enter_power_down();

	} while (game_state != GAME_OVER);

	for (;;)
		enter_power_down();
}
