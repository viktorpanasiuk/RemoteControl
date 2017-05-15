#ifndef TIM1_H_
#define TIM1_H_

#include "main.h"
#include <avr/io.h>

typedef enum {
    TIM1WGM_FAST_PWM_OCR1A = ((uint8_t) 15)
} Tim1Wgm;

typedef enum {
    TIM1CH_A,
    TIM1CH_B,
} Tim1Ch;

typedef enum {
    TIM1COM_NORMAL,
    TIM1COM_TOGGLE,
    TIM1COM_NONINVERTING,
    TIM1COM_INVERTING,
} Tim1Com;

typedef enum {
    TIM1CSM_STOPPED,
    TIM1CSM_PRESCALER_1,
    TIM1CSM_PRESCALER_8,
    TIM1CSM_PRESCALER_64,
    TIM1CSM_PRESCALER_256,
    TIM1CSM_PRESCALER_1024,
    TIM1CSM_EXTERNAL_FALLING,
    TIM1CSM_EXTERNAL_RISING,
} Tim1Csm;

typedef enum {
    TIM1IMR_INPUT_CAPTURE = _BV(TICIE1),
    TIM1IMR_OUTPUT_COMPARE_MATCH_A = _BV(OCIE1A),
    TIM1IMR_OUTPUT_COMPARE_MATCH_B = _BV(OCIE1B),
    TIM1IMR_OVERFLOW = _BV(TOIE1),
} Tim1Imr;

void setTim1Wgm(Tim1Wgm mode);
void setTim1Com(Tim1Ch channel, Tim1Com mode);
void setTim1Ocr(Tim1Ch channel, uint16_t ocr);
void setTim1Csm(Tim1Csm mode);
void setTim1Imr(Tim1Imr mask);

#endif /* TIM1_H_ */