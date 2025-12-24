/******************************* Copyright (c) *******************************
**                                                                          **
**                          http://www.powermcu.com                         **
**                                                                          **
**--------------File Info---------------------------------------------------**
** File name:               main.c                                          **
** Descriptions:            The GLCD application function                   **
**                                                                          **
**--------------------------------------------------------------------------**
** Created by:              AVRman                                          **
** Created date:            2010-11-7                                       **
** Version:                 v1.0                                            **
** Descriptions:            The original version                            **
**                                                                          **
**--------------------------------------------------------------------------**
** Modified by:             Paolo Bernardi                                  **
** Modified date:           03/01/2020                                      **
** Version:                 v2.0                                            **
** Descriptions:            basic program for LCD and Touch Panel teaching  **
**                                                                          **
******************************************************************************/

#include "LPC17xx.h"
#include "GLCD/GLCD.h"
#include "TouchPanel/TouchPanel.h"

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
	TP_Init();
	TouchPanel_Calibrate();

	LCD_Clear(Black);
	GUI_Text(0, 280, "Touch here: 1 sec to clear.", Red, White);
	// LCD_DrawLine(0, 0, 200, 200, White);

	// power-down mode
	LPC_SC->PCON |= 0x1;
	LPC_SC->PCON &= ~(0x2);

	for (;;)
		__ASM("wfi");
}
