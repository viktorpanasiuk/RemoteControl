#include "hal/timer1.h"

typedef enum {
    FAST_PWM_OCR1A_TOP = ((uint8_t) 15)
} WaveformGenerationMode;

typedef enum {
    A,
    B,
} Channel;

typedef enum {
    NORMAL,
    TOGGLE,
    NONINVERTING,
    INVERTING,
} CompareOutputMode;

typedef enum {
    STOPPED,
    PRESCALER_1,
    PRESCALER_8,
    PRESCALER_64,
    PRESCALER_256,
    PRESCALER_1024,
    EXTERNAL_FALLING,
    EXTERNAL_RISING,
} ClockSelect;

static void setWaveformGenerationMode(WaveformGenerationMode mode);
static void setCompareOutputMode(Channel channel, CompareOutputMode mode);
static void setClockSelectMode(ClockSelect mode);

void timer1Init(void)
{
    setWaveformGenerationMode(FAST_PWM_OCR1A_TOP);
    setCompareOutputMode(A, NORMAL);
    setCompareOutputMode(B, NONINVERTING);
}

void timer1Start(void)
{
    setClockSelectMode(PRESCALER_1);
}

static void setWaveformGenerationMode(WaveformGenerationMode mode)
{
    TCCR1A = (mode & _BV(0)) ? TCCR1A | _BV(WGM10) : TCCR1A & ~_BV(WGM10);
    TCCR1A = (mode & _BV(1)) ? TCCR1A | _BV(WGM11) : TCCR1A & ~_BV(WGM11);
    TCCR1B = (mode & _BV(2)) ? TCCR1B | _BV(WGM12) : TCCR1B & ~_BV(WGM12);
    TCCR1B = (mode & _BV(3)) ? TCCR1B | _BV(WGM13) : TCCR1B & ~_BV(WGM13);
}

static void setCompareOutputMode(Channel channel, CompareOutputMode mode)
{
    switch (channel) {

    case A:
        TCCR1A = (mode & _BV(0)) ? TCCR1A | _BV(COM1A0) : TCCR1A & ~_BV(COM1A0);
        TCCR1A = (mode & _BV(1)) ? TCCR1A | _BV(COM1A1) : TCCR1A & ~_BV(COM1A1);
        break;

    case B:
        TCCR1A = (mode & _BV(0)) ? TCCR1A | _BV(COM1B0) : TCCR1A & ~_BV(COM1B0);
        TCCR1A = (mode & _BV(1)) ? TCCR1A | _BV(COM1B1) : TCCR1A & ~_BV(COM1B1);
        break;
    }
}

static void setClockSelectMode(ClockSelect mode)
{
    TCCR1B &= ~(_BV(CS12) | _BV(CS11) | _BV(CS10));
    TCNT1 = 0x0000;
    TCCR1B |= mode;
}

void setOCR1A(uint8_t ocr1a)
{
    OCR1A = ocr1a;
}

void setOCR1B(uint8_t ocr1b)
{
    OCR1B = ocr1b;
}