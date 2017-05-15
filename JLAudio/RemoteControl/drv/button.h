#ifndef BUTTON_H_
#define BUTTON_H_

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
} ButtonState;

typedef enum {
    SINGLE,
    HOLD,
} ButtonAction;

void buttonFsm(void);
void buttonInit(void);

#endif /* BUTTON_H_ */