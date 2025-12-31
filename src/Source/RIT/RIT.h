#ifndef __RIT_H
#define __RIT_H

#include <stdint.h>

// init_RIT.c
extern uint32_t init_RIT(uint32_t RITInterval);
extern void enable_RIT(void);
extern void disable_RIT(void);
extern void reset_RIT(void);
unsigned int get_RIT_value();

// IRQ_RIT.c
extern void RIT_IRQHandler(void);

#endif /* end __RIT_H */
