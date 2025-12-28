#include "button.h"
#include "lpc17xx.h"

extern int down;

// INT0
void EINT0_IRQHandler(void)
{
	LPC_SC->EXTINT &= (1 << 0); // clear pending interrupt
}

// KEY1
void EINT1_IRQHandler(void)
{
	NVIC_DisableIRQ(EINT1_IRQn); // disable Button interrupts
	LPC_PINCON->PINSEL4 &= ~(1 << 22); // GPIO pin selection
	down = 1;
	LPC_SC->EXTINT &= (1 << 1); // clear pending interrupt
}

// KEY2
void EINT2_IRQHandler(void)
{
	LPC_SC->EXTINT &= (1 << 2); // clear pending interrupt
}
