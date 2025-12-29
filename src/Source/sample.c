#include "LPC17xx.h"
#include "GLCD/GLCD.h"
#include "tetromino/tetromino.h"
#include "playing_field/playing_field.h"

// ScaleFlag needs to visible in order for the emulator to find the symbol (can
// be placed also inside system_LPC17xx.h but since it is RO, it needs more
// work)
#ifdef SIMULATOR
extern uint8_t ScaleFlag;
#endif

int main(void)
{
	SystemInit();
	LCD_Initialization();

	draw_playing_field();

	struct tetromino i_piece =
		tetromino_create(I_TYPE, (struct square){ 0, 0 });

	for (int i = 0; i < 15; i++) {
		tetromino_draw(i_piece);
		tetromino_move_right(&i_piece);
	}

	// power-down mode
	LPC_SC->PCON |= 0x1;
	LPC_SC->PCON &= ~(0x2);

	for (;;)
		__ASM("wfi");
}
