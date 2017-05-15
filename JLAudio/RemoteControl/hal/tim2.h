#ifndef TIM2_H_
#define TIM2_H_

#include <avr/io.h>

typedef enum {
    TIM2WGM_NORMAL,
    TIM2WGM_PHASE_CORRECT,
    TIM2WGM_CTC,
    TIM2WGM_FAST_PWM,
} Tim2Wgm;

typedef enum {
    TIM2CSM_STOPPED,
    TIM2CSM_PRESCALER_1,
    TIM2CSM_PRESCALER_8,
    TIM2CSM_PRESCALER_32,
    TIM2CSM_PRESCALER_64,
    TIM2CSM_PRESCALER_128,
    TIM2CSM_PRESCALER_256,
    TIM2CSM_PRESCALER_1024,
} Tim2Csm;

typedef enum {
    TIM2IMR_OUTPUT_COMPARE_MATCH = _BV(OCIE2),
    TIM2IMR_OVERFLOW = _BV(TOIE2),
} Tim2Imr;

void tim2Init(void);

void setTim2Wgm(Tim2Wgm mode);
void setTim2Ocr(uint16_t ocr);
void setTim2Csm(Tim2Csm mode);
void setTim2Imr(Tim2Imr mask);

#endif /* TIM2_H_ */