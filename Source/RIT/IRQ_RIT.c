#include "LPC17xx.h"
#include "RIT.h"
#include "game_stats.h"
#include "tetromino/tetromino.h"
#include "timer.h"

volatile int down_1 = 0;
volatile int down_2 = 0;
static volatile int toRelease_down_1 = 0;
static volatile int toRelease_down_2 = 0;

static volatile int J_up = 0;
static volatile int J_down = 0;
static volatile int J_right = 0;
static volatile int J_left = 0;

static inline void handle_key1_press(void);
static inline void handle_key2_press(void);

static inline void handle_joystick_left(void);
static inline void handle_joystick_down(void);
static inline void handle_joystick_up(void);
static inline void handle_joystick_right(void);

void RIT_IRQHandler(void)
{
	if (down_1 != 0)
		handle_key1_press();

	if (down_2 != 0)
		handle_key2_press();

	if ((LPC_GPIO1->FIOPIN & (1 << 29)) == 0)
		handle_joystick_up();
	else
		J_up = 0;

	if ((LPC_GPIO1->FIOPIN & (1 << 26)) == 0)
		handle_joystick_down();
	else
		J_down = 0;

	if ((LPC_GPIO1->FIOPIN & (1 << 28)) == 0)
		handle_joystick_right();
	else
		J_right = 0;

	if ((LPC_GPIO1->FIOPIN & (1 << 27)) == 0)
		handle_joystick_left();
	else
		J_left = 0;

	// reset_RIT(); // se ci sono cose strane come il rit che si ferma
	LPC_RIT->RICTRL |= 0x1; /* clear interrupt flag */
}

static inline void handle_key1_press(void)
{
	down_1++;
	if ((LPC_GPIO2->FIOPIN & (1 << 11)) == 0) {
		switch (down_1) {
		case 2: // short press
			switch (game_state) {
			case PAUSED:
				enable_timer(TIMER0);
				game_state = PLAYING;
				break;
			case PLAYING:
				disable_timer(TIMER0);
				game_state = PAUSED;
				break;
			case GAME_OVER:
				// TODO: reset playing field and goto menu.
				break;
			}
			toRelease_down_1 = 1;
			break;
		default:
			break;
		}
	} else { /* button released */
		if (toRelease_down_1) {
			//add code to manage release.
			toRelease_down_1 = 0;
		}
		down_1 = 0;
		NVIC_EnableIRQ(EINT1_IRQn);
		LPC_PINCON->PINSEL4 |= (1 << 22);
	}
}

static inline void handle_key2_press(void)
{
	down_2++;
	if ((LPC_GPIO2->FIOPIN & (1 << 12)) == 0) { /* button premuto */
		switch (down_2) {
		case 2: // short press
			if (game_state == PLAYING) {
				tetromino_hide(&current_tetromino);
				tetromino_hard_drop(&current_tetromino);
				tetromino_draw(current_tetromino);
			}
			toRelease_down_2 = 1;
			break;
		default:
			break;
		}
	} else { /* button released */
		if (toRelease_down_2) {
			//add code to manage release.
			toRelease_down_2 = 0;
		}
		down_2 = 0;
		NVIC_EnableIRQ(EINT2_IRQn);
		LPC_PINCON->PINSEL4 |= (1 << 24);
	}
}

static inline void handle_joystick_left(void)
{
	J_left++;
	switch (J_left) {
	case 1:
		if (game_state == PLAYING) {
			tetromino_hide(&current_tetromino);
			tetromino_move_left(&current_tetromino);
			tetromino_draw(current_tetromino);
		}
		break;
	default:
		break;
	}
}

static inline void handle_joystick_down(void)
{
	J_down++;
	switch (J_down) {
	case 1:
		// TODO: add soft drop
		break;
	default:
		break;
	}
}

static inline void handle_joystick_up(void)
{
	J_up++;
	switch (J_up) {
	case 1:
		if (game_state == PLAYING) {
			tetromino_hide(&current_tetromino);
			tetromino_rotate_clockwise(&current_tetromino);
			tetromino_draw(current_tetromino);
		}
		break;
	default:
		break;
	}
}

static inline void handle_joystick_right(void)
{
	J_right++;
	switch (J_right) {
	case 1:
		if (game_state == PLAYING) {
			tetromino_hide(&current_tetromino);
			tetromino_move_right(&current_tetromino);
			tetromino_draw(current_tetromino);
		}
		break;
	default:
		break;
	}
}
