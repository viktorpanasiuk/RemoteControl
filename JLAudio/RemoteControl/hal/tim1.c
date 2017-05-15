#include "hal/tim1.h"
#include "hal/gpio.h"

#include "main.h"


void setTim1Wgm(Tim1Wgm mode)
{
    TCCR1A = (mode & _BV(0)) ? TCCR1A | _BV(WGM10) : TCCR1A & ~_BV(WGM10);
    TCCR1A = (mode & _BV(1)) ? TCCR1A | _BV(WGM11) : TCCR1A & ~_BV(WGM11);
    TCCR1B = (mode & _BV(2)) ? TCCR1B | _BV(WGM12) : TCCR1B & ~_BV(WGM12);
    TCCR1B = (mode & _BV(3)) ? TCCR1B | _BV(WGM13) : TCCR1B & ~_BV(WGM13);
}

void setTim1Com(Tim1Ch channel, Tim1Com mode)
{
    switch (channel) {

        case TIM1CH_A:
        TCCR1A = (mode & _BV(0)) ? TCCR1A | _BV(COM1A0) : TCCR1A & ~_BV(COM1A0);
        TCCR1A = (mode & _BV(1)) ? TCCR1A | _BV(COM1A1) : TCCR1A & ~_BV(COM1A1);
        break;

        case TIM1CH_B:
        TCCR1A = (mode & _BV(0)) ? TCCR1A | _BV(COM1B0) : TCCR1A & ~_BV(COM1B0);
        TCCR1A = (mode & _BV(1)) ? TCCR1A | _BV(COM1B1) : TCCR1A & ~_BV(COM1B1);
        break;

        default: break;
    }
}

void setTim1Ocr(Tim1Ch channel, uint16_t ocr)
{
    switch (channel) {

        case TIM1CH_A:
        OCR1A = ocr;
        break;

        case TIM1CH_B:
        OCR1B = ocr;
        break;

        default: break;
    }
}

void setTim1Csm(Tim1Csm mode)
{
    TCCR1B &= ~(_BV(CS12) | _BV(CS11) | _BV(CS10));
    TCNT1 = 0x0000;
    TCCR1B |= mode;
}

void setTim1Imr(Tim1Imr mask)
{
    TIMSK &= ~(_BV(TICIE1) | _BV(OCIE1A) | _BV(OCIE1B) | _BV(TOIE1));
    TIMSK |= mask;
}