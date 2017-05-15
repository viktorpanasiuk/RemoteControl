#ifndef MAIN_H_
#define MAIN_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#define F_OSC 8000000UL // 8 MHz
#define TICK_PERIOD 1000UL // 1000 us

#define NUM_OF(array) (sizeof(array) / sizeof(array[0]))

#define LED_ILLUMINATION_LEVEL_0    ((uint8_t) 0)
#define LED_ILLUMINATION_LEVEL_1    ((uint8_t) 2)
#define LED_ILLUMINATION_LEVEL_2    ((uint8_t) 4)
#define LED_ILLUMINATION_LEVEL_3    ((uint8_t) 6)
#define LED_ILLUMINATION_LEVEL_4    ((uint8_t) 8)
#define LED_ILLUMINATION_LEVEL_5    ((uint8_t) 10)
#define LED_ILLUMINATION_LEVEL_6    ((uint8_t) 12)
#define LED_ILLUMINATION_LEVEL_7    ((uint8_t) 14)
#define LED_ILLUMINATION_LEVEL_8    ((uint8_t) 16)
#define LED_ILLUMINATION_LEVEL_9    ((uint8_t) 18)
#define LED_ILLUMINATION_LEVEL_10   ((uint8_t) 20)
#define LED_ILLUMINATION_LEVEL_11   ((uint8_t) 22)
#define LED_ILLUMINATION_LEVEL_12   ((uint8_t) 24)
#define LED_ILLUMINATION_LEVEL_13   ((uint8_t) 26)
#define LED_ILLUMINATION_LEVEL_14   ((uint8_t) 28)
#define LED_ILLUMINATION_LEVEL_15   ((uint8_t) 30)

#endif /* MAIN_H_ */