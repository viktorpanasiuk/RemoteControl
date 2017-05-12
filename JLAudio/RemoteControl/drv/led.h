#ifndef LED_H_
#define LED_H_

typedef enum {
    AM,
    FM,
    WB,
    SXM,
    BT,
    AUX,
    USB,
} LedSource;

typedef enum {
    OFF,
    ON,
} LedState;

void ledInit(void);
void ledSet(LedSource source, LedState state);
void ledOff(void);

#endif /* LED_H_ */