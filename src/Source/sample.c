#include <stdlib.h>
#include <time.h>

#include "LPC17xx.h"
#include "GLCD/GLCD.h"
#include "tetromino/tetromino.h"
#include "playing_field/playing_field.h"
#include "game_stats.h"
#include "timer/timer.h"

// ScaleFlag needs to visible in order for the emulator to find the symbol (can
// be placed also inside system_LPC17xx.h but since it is RO, it needs more
// work)
#ifdef SIMULATOR
extern uint8_t ScaleFlag;
#endif

// #define ONE_SECOND (25000000)
#define ONE_SECOND (250000)

int high_score = 0;
int cleared_lines = 0;
enum game_state game_state = MENU;
struct tetromino current_tetromino;

static inline void enter_power_down(void)
{
	LPC_SC->PCON |= (1U << 0);
	LPC_SC->PCON &= ~(1U << 1);

	__ASM("wfi");
}

int main(void)
{
	srand(time(NULL));
	SystemInit();
	LCD_Initialization();

	playing_field_draw();
	playing_field_reset();

	init_timer(TIMER0, 0, 0, CONTROL_RESET | CONTROL_INTERRUPT, ONE_SECOND);
	enable_timer(TIMER0);

	do {
		current_tetromino = tetromino_create(rand() % NOF_TETROMINOS,
						     (struct square){ 3, 0 });

		tetromino_draw(current_tetromino);

		while (!current_tetromino.placed) {
			enter_power_down();
			tetromino_move_down(&current_tetromino);
		}
	} while (game_state != GAME_OVER);

	for (;;)
		enter_power_down();
}
