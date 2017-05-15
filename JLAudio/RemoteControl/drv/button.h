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
    RELEASED,
    PRESSED,
} ButtonState;

void buttonFsm(void);

#endif /* BUTTON_H_ */