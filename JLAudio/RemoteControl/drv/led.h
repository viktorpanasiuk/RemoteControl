#ifndef LED_H_
#define LED_H_

#include <avr/io.h>

typedef enum {
    AM,
    FM,
    WB,
    SXM,
    BT,
    AUX,
    USB,

    NONE,
    NUM_OF_LEDS = NONE,
} LedSource;

typedef enum {
    OFF,
    ON,
} LedState;

void ledInit(void);
void switchLedTo(LedSource source);

#endif /* LED_H_ */