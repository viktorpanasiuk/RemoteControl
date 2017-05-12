#include "drv/pwm.h"
#include "hal/gpio.h"
#include "hal/timer1.h"

#define PWM_MAX_LVL 100

#define PWM_DDRx    DDRB
#define PWM_DDxn    DDB2

void pwmInit(void)
{
    GPIO_DIR_O(PWM_DDRx, PWM_DDxn);
    timer1Init();
    setOCR1A(PWM_MAX_LVL);
    setOCR1B(PWM_MAX_LVL);
    timer1Start();
}

void setPwmLvl(uint8_t percent)
{
    uint16_t ocr1b = 100 - percent * OCR1A / 100;

    setOCR1B(ocr1b);
}