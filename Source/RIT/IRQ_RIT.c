#include "LPC17xx.h"
#include "RIT.h"
#include "tetris/game_stats.h"
#include "tetris/tetromino.h"
#include "timer.h"

volatile int down_1 = 0;
volatile int down_2 = 0;

static volatile int joystick_up = 0;
volatile int joystick_down = 0;
static volatile int joystick_right = 0;
static volatile int joystick_left = 0;

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
		joystick_up = 0;

	if ((LPC_GPIO1->FIOPIN & (1 << 26)) == 0)
		handle_joystick_down();
	else
		joystick_down = 0;

	if ((LPC_GPIO1->FIOPIN & (1 << 28)) == 0)
		handle_joystick_right();
	else
		joystick_right = 0;

	if ((LPC_GPIO1->FIOPIN & (1 << 27)) == 0)
		handle_joystick_left();
	else
		joystick_left = 0;

	// reset_RIT();
	LPC_RIT->RICTRL |= 0x1;
}

static inline void handle_key1_press(void)
{
	down_1++;
	if ((LPC_GPIO2->FIOPIN & (1 << 11)) == 0) {
		switch (down_1) {
		case 2:
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
			break;
		default:
			break;
		}
	} else {
		down_1 = 0;
		NVIC_EnableIRQ(EINT1_IRQn);
		LPC_PINCON->PINSEL4 |= (1 << 22);
	}
}

static inline void handle_key2_press(void)
{
	down_2++;
	if ((LPC_GPIO2->FIOPIN & (1 << 12)) == 0) {
		switch (down_2) {
		case 2:
			buffered_move |= HARD_DROP;
			break;
		default:
			break;
		}
	} else {
		down_2 = 0;
		NVIC_EnableIRQ(EINT2_IRQn);
		LPC_PINCON->PINSEL4 |= (1 << 24);
	}
}

static inline void handle_joystick_left(void)
{
	joystick_left++;
	switch (joystick_left) {
	case 1:
		buffered_move |= MOVE_LEFT;
		break;
	default:
		break;
	}
}

static inline void handle_joystick_down(void)
{
	joystick_down++;
	switch (joystick_down) {
	case 1:
		break;
	default:
		break;
	}
}

static inline void handle_joystick_up(void)
{
	joystick_up++;
	switch (joystick_up) {
	case 1:
		buffered_move |= ROTATE;
		break;
	default:
		break;
	}
}

static inline void handle_joystick_right(void)
{
	joystick_right++;
	switch (joystick_right) {
	case 1:
		buffered_move |= MOVE_RIGHT;
		break;
	default:
		break;
	}
}
