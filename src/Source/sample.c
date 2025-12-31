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
#define ONE_SECOND (2500000)
#define FIFTY_MSEC (700000)
#else
// #define ONE_SECOND (25000000)
// #define FIFTY_MSEC (ONE_SECOND / 20)
#endif

int high_score;
int score;
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
	SystemInit();

	LCD_Initialization();

	BUTTON_init();

	init_RIT(FIFTY_MSEC);
	enable_RIT();

	init_timer(TIMER0, 0, 0, CONTROL_RESET | CONTROL_INTERRUPT, ONE_SECOND);
	init_timer(TIMER1, 0, 0, CONTROL_NULL, 0);

menu:
	enable_timer(TIMER1);
	LCD_Clear(Blue);
	while (game_state == PAUSED)
		enter_power_down();

	/***************
	 * GAME BEGINS *
	 ***************/

	srand(get_timer_value(TIMER1));
	disable_timer(TIMER1);

	high_score = 0;
	score = 0;
	cleared_lines = 0;

	playing_field_reset();
	playing_field_draw();

	enable_timer(TIMER0);

	game_state = PLAYING;
	do {
		current_tetromino = tetromino_create(rand() % NOF_TETROMINOS,
						     (struct block){ 3, 0 });
		tetromino_draw(current_tetromino);

		for (int i = 0; game_state != GAME_OVER && i < 4; i++)
			if (playing_field_is_occupied(
				    current_tetromino.blocks[i].x,
				    current_tetromino.blocks[i].y))
				game_state = GAME_OVER;

		while (!current_tetromino.placed)
			enter_power_down();

	} while (game_state != GAME_OVER);

	if (score > high_score)
		high_score = score;
	disable_timer(TIMER0);
	goto menu;
}
