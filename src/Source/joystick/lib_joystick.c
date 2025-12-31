#include "LPC17xx.h"
#include "joystick.h"

void joystick_init(void)
{
	/* joystick up functionality */
	LPC_PINCON->PINSEL3 &= ~(3 << 26);
	LPC_GPIO1->FIODIR &= ~(1 << 29);

	/* joystick dx functionality */
	LPC_PINCON->PINSEL3 &= ~(3 << 24);
	LPC_GPIO1->FIODIR &= ~(1 << 28);

	/* joystick sx functionality */
	LPC_PINCON->PINSEL3 &= ~(3 << 22);
	LPC_GPIO1->FIODIR &= ~(1 << 27);

	/* joystick down functionality */
	LPC_PINCON->PINSEL3 &= ~(3 << 20);
	LPC_GPIO1->FIODIR &= ~(1 << 26);

	/* joystick clicked functionality */
	LPC_PINCON->PINSEL3 &= ~(3 << 18);
	LPC_GPIO1->FIODIR &= ~(1 << 25);
}
