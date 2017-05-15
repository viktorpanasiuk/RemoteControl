#ifndef PWM_H_
#define PWM_H_

#include <avr/io.h>

void pwmInit(void);
void setPwmLvl(uint8_t percent);

#endif /* PWM_H_ */