#include "drv/button.h"
#include "hal/gpio.h"
#include "main.h"

#include <avr/pgmspace.h>

#define DEBOUNCE_PERIOD 100 // ms
#define LONG_PRESS_PERIOD 800 // ms

static const GpioStruct button[] PROGMEM = {
    [SOURCE] = GPIO_STRUCT_INIT(C, 5),
    [SEEK_LEFT] = GPIO_STRUCT_INIT(C, 4),
    [PLAY_PAUSE] = GPIO_STRUCT_INIT(C, 3),
    [SEEK_RIGHT] = GPIO_STRUCT_INIT(C, 2),
    [VOLUME_DOWN] = GPIO_STRUCT_INIT(C, 1),
    [VOLUME_UP] = GPIO_STRUCT_INIT(C, 0),
};

static const uint8_t action2cmd[][NUM_OF_BUTTONS] PROGMEM = {
    [SOURCE]      = { [RELEASE] = 0xFF,         [SINGLE] = SOURCE_SINGLE,      [HOLD] = SOURCE_HOLD,        },
    [SEEK_LEFT]   = { [RELEASE] = SEEK_RELEASE, [SINGLE] = SEEK_LEFT_SINGLE,   [HOLD] = SEEK_LEFT_HOLD,     },
    [PLAY_PAUSE]  = { [RELEASE] = 0xFF,         [SINGLE] = PLAY_PAUSE_SINGLE,  [HOLD] = PLAY_PAUSE_HOLD,    },
    [SEEK_RIGHT]  = { [RELEASE] = SEEK_RELEASE, [SINGLE] = SEEK_RIGHT_SINGLE,  [HOLD] = SEEK_RIGHT_HOLD,    },
    [VOLUME_DOWN] = { [RELEASE] = 0xFF,         [SINGLE] = VOLUME_DOWN_SINGLE, [HOLD] = VOLUME_DOWN_SINGLE, },
    [VOLUME_UP]   = { [RELEASE] = 0xFF,         [SINGLE] = VOLUME_UP_SINGLE,   [HOLD] = VOLUME_UP_SINGLE,   },
};

static void buttonHandler(ButtonSource source, ButtonState * button);

void buttonInit(void)
{
    SFIOR &= ~_BV(PUD);
    for (ButtonSource source = 0; source < NUM_OF_BUTTONS; ++source) {
        GpioStruct * gpio = getGpioStructFromFlash(&button[source]);
        setGpioDirection(gpio, INPUT);
        setGpioState(gpio, HIGH);
    }
}

ButtonStatus getButtonStatus(ButtonSource source)
{
    GpioStruct * gpio = getGpioStructFromFlash(&button[source]);

    return getGpioState(gpio);
}

static void buttonHandler(ButtonSource button, ButtonState * state)
{
    static const void * const label[] = {
        &&BUTTON_IS_RELEASED,
        &&BUTTON_WAS_PRESSED,
        &&BUTTON_IS_SHORT_PRESSED,
        &&BUTTON_IS_LONG_PRESSED,
        &&BUTTON_WAS_RELEASED,
    };
    ButtonStatus buttonStatus = getButtonStatus(button);
    ButtonAction buttonAction = state->action;
    ButtonCounter buttonCounter = state->counter;
    static ButtonCounter longPressPeriod = LONG_PRESS_PERIOD;
    static uint8_t debounceCounter;
    static ButtonAction prevAction;

    goto * label[buttonAction];

    do {

    BUTTON_IS_RELEASED:
        if (buttonStatus == PRESSED) {
            longPressPeriod = LONG_PRESS_PERIOD;
            buttonAction = BUTTON_WAS_PRESSED;
        }
        break;

    BUTTON_WAS_PRESSED:
        if (buttonStatus == RELEASED) {
            debounceCounter = 0;
            buttonAction = BUTTON_IS_RELEASED;
        } else if (++debounceCounter > DEBOUNCE_PERIOD) {
            buttonCounter = 0;
            buttonAction = BUTTON_IS_SHORT_PRESSED;
        }
        break;

    BUTTON_IS_SHORT_PRESSED:
        if (buttonStatus == RELEASED) {
            debounceCounter = 0;
            prevAction = BUTTON_IS_SHORT_PRESSED;
            buttonAction = BUTTON_WAS_RELEASED;
        } else if (++buttonCounter > LONG_PRESS_PERIOD) {
            buttonAction = BUTTON_IS_LONG_PRESSED;
        }
        break;

    BUTTON_IS_LONG_PRESSED:
        if (buttonStatus == RELEASED) {
            debounceCounter = 0;
            prevAction = BUTTON_IS_LONG_PRESSED;
            buttonAction = BUTTON_WAS_RELEASED;
        } else if (++buttonCounter > longPressPeriod) {
            buttonCounter = 0;
            switch (button) {

                case SOURCE:
                    longPressPeriod = 0xFFFF;
                    UDR = SOURCE_HOLD;
                    break;

                case SEEK_LEFT:
                    longPressPeriod = 1000;
                    UDR = SEEK_LEFT_HOLD;
                    break;

                case PLAY_PAUSE:
                    longPressPeriod = 0xFFFF;
                    UDR = PLAY_PAUSE_HOLD;
                    break;

                case SEEK_RIGHT:
                    longPressPeriod = 1000;
                    UDR = SEEK_RIGHT_HOLD;
                    break;

                case VOLUME_DOWN:
                    longPressPeriod = 500;
                    UDR = VOLUME_DOWN_SINGLE;
                    break;

                case VOLUME_UP:
                    longPressPeriod = 500;
                    UDR = VOLUME_UP_SINGLE;
                    break;

                default:
                    break;
            }
        }
        break;

    BUTTON_WAS_RELEASED:
        if (buttonStatus == PRESSED) {
            debounceCounter = 0;
            buttonAction = prevAction;
        } else if (++debounceCounter > DEBOUNCE_PERIOD) {
            if (prevAction == BUTTON_IS_SHORT_PRESSED) {
                switch (button) {
                    case SOURCE: UDR = SOURCE_SINGLE; break;
                    case SEEK_LEFT: UDR = SEEK_LEFT_SINGLE; break;
                    case PLAY_PAUSE: UDR = PLAY_PAUSE_SINGLE; break;
                    case SEEK_RIGHT: UDR = SEEK_RIGHT_SINGLE; break;
                    case VOLUME_DOWN: UDR = VOLUME_DOWN_SINGLE; break;
                    case VOLUME_UP: UDR = VOLUME_UP_SINGLE; break;
                    default: break;
                }
            } else {
                switch (button) {
                    case SEEK_LEFT:
                    case SEEK_RIGHT:
                        UDR = SEEK_RELEASE;
                        break;

                    case SOURCE:
                    case PLAY_PAUSE:
                    case VOLUME_DOWN:
                    case VOLUME_UP:
                    default: break;
                }
            }
            buttonCounter = 0;
            buttonAction = BUTTON_IS_RELEASED;
        }
        break;

    } while(0);

    state->action = buttonAction;
    state->counter = buttonCounter;
}

void buttonFsm(void)
{
    static ButtonState param[NUM_OF_BUTTONS];

    for (ButtonSource source = 0; source < NUM_OF(button); ++source) {
        buttonHandler(source, &param[source]);
    }
}