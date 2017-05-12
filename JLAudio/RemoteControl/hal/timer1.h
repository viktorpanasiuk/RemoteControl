#ifndef TIMER1_H_
#define TIMER1_H_

#include <avr/io.h>

void timer1Init(void);
void timer1Start(void);
void setOCR1A(uint8_t ocr1a);
void setOCR1B(uint8_t ocr1b);

#endif /* TIMER1_H_ */