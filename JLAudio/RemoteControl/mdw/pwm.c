#include "mdw/pwm.h"
#include "hal/tim1.h"

#define MAX_PWM_LVL 100

#define PWM_DDRx    DDRB
#define PWM_DDxn    DDB2

void pwmInit(void)
{
    PWM_DDRx |= _BV(PWM_DDxn);
    setTim1Wgm(TIM1WGM_FAST_PWM_OCR1A);
    setTim1Ocr(TIM1CH_A, MAX_PWM_LVL);
    setTim1Ocr(TIM1CH_B, MAX_PWM_LVL);
    setTim1Com(TIM1CH_B, TIM1COM_NORMAL);
    setTim1Com(TIM1CH_B, TIM1COM_NONINVERTING);
    setTim1Csm(TIM1CSM_PRESCALER_1);
}

void setPwmLvl(uint8_t percent)
{
    setTim1Ocr(TIM1CH_B, MAX_PWM_LVL - percent);
}