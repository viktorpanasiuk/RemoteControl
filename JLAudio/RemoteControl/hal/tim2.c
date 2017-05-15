#include "hal/tim2.h"
#include <avr/interrupt.h>
#include <stdbool.h>

extern volatile bool isTick;

void tim2Init(void)
{
    setTim2Wgm(TIM2WGM_CTC);
    setTim2Imr(TIM2IMR_OUTPUT_COMPARE_MATCH);
    setTim2Ocr(250);
    setTim2Csm(TIM2CSM_PRESCALER_32);
}

void setTim2Wgm(Tim2Wgm mode)
{
    TCCR2 = (mode & _BV(0)) ? TCCR2 | _BV(WGM20) : TCCR2 & ~_BV(WGM20);
    TCCR2 = (mode & _BV(1)) ? TCCR2 | _BV(WGM21) : TCCR2 & ~_BV(WGM21);
}

void setTim2Ocr(uint16_t ocr)
{
    OCR2 = ocr;
}

void setTim2Csm(Tim2Csm mode)
{
    TCCR2 &= ~(_BV(CS22) | _BV(CS21) | _BV(CS20));
    TCNT2 = 0x00;
    TCCR2 |= mode;
}

void setTim2Imr(Tim2Imr mask)
{
    TIMSK &= ~(_BV(OCIE2) | _BV(TOIE2));
    TIMSK |= mask;
}

ISR(TIMER2_COMP_vect)
{
    isTick = true;
}