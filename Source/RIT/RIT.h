#ifndef __RIT_H
#define __RIT_H

#include <stdint.h>

extern volatile int down_1;
extern volatile int down_2;

extern volatile int joystick_down;

// init_RIT.c
uint32_t init_RIT(uint32_t RITInterval);
void enable_RIT(void);
void disable_RIT(void);
void reset_RIT(void);
unsigned int get_RIT_value();

// IRQ_RIT.c
void RIT_IRQHandler(void);

#endif
