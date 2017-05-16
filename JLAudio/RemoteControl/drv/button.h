#ifndef BUTTON_H_
#define BUTTON_H_

#include <avr/io.h>

typedef enum {
    SOURCE,
    SEEK_LEFT,
    PLAY_PAUSE,
    SEEK_RIGHT,
    VOLUME_DOWN,
    VOLUME_UP,
    
    NUM_OF_BUTTONS
} ButtonSource;

typedef enum {
    PRESSED,
    RELEASED,
} ButtonStatus;

typedef enum {
    RELEASE,
    SINGLE,
    HOLD,
} ButtonPress;

typedef enum {
    BUTTON_IS_RELEASED,
    BUTTON_WAS_PRESSED,
    BUTTON_IS_SHORT_PRESSED,
    BUTTON_IS_LONG_PRESSED,
    BUTTON_WAS_RELEASED,
} ButtonAction;

typedef uint16_t ButtonCounter;

typedef struct {
    ButtonAction action;
    ButtonCounter counter;
} ButtonState;

void buttonFsm(void);
void buttonInit(void);

#endif /* BUTTON_H_ */