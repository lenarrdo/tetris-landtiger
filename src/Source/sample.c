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

	tetromino_draw(tetromino_create(I_TYPE, (struct square){ 0, 19 }));
	tetromino_draw(tetromino_create(O_TYPE, (struct square){ 0, 17 }));
	tetromino_draw(tetromino_create(T_TYPE, (struct square){ 3, 18 }));
	tetromino_draw(tetromino_create(S_TYPE, (struct square){ 5, 18 }));
	tetromino_draw(tetromino_create(Z_TYPE, (struct square){ 5, 16 }));
	tetromino_draw(tetromino_create(J_TYPE, (struct square){ 7, 17 }));
	tetromino_draw(tetromino_create(L_TYPE, (struct square){ 0, 14 }));

	// power-down mode
	LPC_SC->PCON |= 0x1;
	LPC_SC->PCON &= ~(0x2);

	for (;;)
		__ASM("wfi");
}
