#include "LPC17xx.h"
#include "music.h"

#include "../timer/timer.h"

uint16_t sine_table[45] = { 410, 467, 523, 576, 627, 673, 714, 749, 778,
			    799, 813, 819, 817, 807, 789, 764, 732, 694,
			    650, 602, 550, 495, 438, 381, 324, 270, 217,
			    169, 125, 87,  55,	30,  12,  2,   0,   6,
			    20,	 41,  70,  105, 146, 193, 243, 297, 353 };
int music_tick = 0;
int current_note = 0;

void set_note(struct note note)
{
	disable_timer(TIMER0);
	reset_timer(TIMER0);
	init_timer(TIMER0, 0, 0, CONTROL_RESET | CONTROL_INTERRUPT, note.freq);
	enable_timer(TIMER0);
}

void play_note(void)
{
	LPC_DAC->DACR = (sine_table[music_tick++] << 6);
	if (music_tick == 45)
		music_tick = 0;
}
