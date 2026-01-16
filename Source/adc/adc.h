#ifndef ADC_H_
#define ADC_H_

// lib_adc.c
void ADC_init(void);
void ADC_start_conversion(void);

// IRQ_adc.c
void ADC_IRQHandler(void);

#endif // ADC_H_
