#include "LPC17xx.h"
#include "GLCD/GLCD.h"
#include "TouchPanel/TouchPanel.h"
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
	// TP_Init();
	// TouchPanel_Calibrate();

	draw_playing_field();

	// tetromino_draw(tetromino_create(I_TYPE, (struct square){ 0, 0 }));
	// tetromino_draw(tetromino_create(O_TYPE, (struct square){ 0, 17 }));
	// tetromino_draw(tetromino_create(T_TYPE, (struct square){ 3, 18 }));
	// tetromino_draw(tetromino_create(S_TYPE, (struct square){ 5, 18 }));
	// tetromino_draw(tetromino_create(Z_TYPE, (struct square){ 5, 16 }));
	// tetromino_draw(tetromino_create(J_TYPE, (struct square){ 7, 17 }));
	// tetromino_draw(tetromino_create(L_TYPE, (struct square){ 0, 14 }));

	// LCD_DrawLine(0, 0, 200, 0, White);
	// LCD_DrawLine(0, 1, 200, 1, White);
	// LCD_DrawLine(0, 2, 200, 2, White);
	// LCD_DrawLine(0, 3, 200, 3, White);

	// LCD_DrawLine(160, 0, 160, 320, White);
	// LCD_DrawLine(161, 0, 161, 320, White);
	// LCD_DrawLine(162, 0, 162, 320, White);
	// LCD_DrawLine(163, 0, 163, 320, White);

	// power-down mode
	LPC_SC->PCON |= 0x1;
	LPC_SC->PCON &= ~(0x2);

	for (;;)
		__ASM("wfi");
}
