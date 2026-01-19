#include "LPC17xx.h"
#include "music.h"

#include "../timer/timer.h"

volatile int sine_table[45] = { 410, 467, 523, 576, 627, 673, 714, 749, 778,
				799, 813, 819, 817, 807, 789, 764, 732, 694,
				650, 602, 550, 495, 438, 381, 324, 270, 217,
				169, 125, 87,  55,  30,	 12,  2,   0,	6,
				20,  41,  70,  105, 146, 193, 243, 297, 353 };
volatile int music_tick = 0;
volatile int curr_music_note = 0;
volatile int curr_sfx_note = 0;
volatile enum audio_track curr_audio_track = NO_AUDIO;

void set_note(struct note note)
{
	disable_timer(TIMER3);
	reset_timer(TIMER3);
	init_timer(TIMER3, 0, 0, CONTROL_RESET | CONTROL_INTERRUPT, note.freq);
	enable_timer(TIMER3);
}

void play_note(void)
{
	LPC_DAC->DACR = (sine_table[music_tick++] << 6);
	if (music_tick == 45)
		music_tick = 0;
}

void music_init(void)
{
	LPC_PINCON->PINSEL1 |= (1 << 21);
	LPC_PINCON->PINSEL1 &= ~(1 << 20);
	LPC_GPIO0->FIODIR |= (1 << 26);
}

void music_pause(void)
{
	disable_timer(TIMER1);
	disable_timer(TIMER3);
	curr_audio_track = NO_AUDIO;
}

void music_play(void)
{
	enable_timer(TIMER1);
	enable_timer(TIMER3);
	curr_audio_track = A_TYPE_THEME;
}
