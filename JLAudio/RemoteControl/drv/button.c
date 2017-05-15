#include "drv/button.h"
#include "hal/gpio.h"
#include "main.h"

#define DEBOUNCE_PERIOD 30 // ms
#define LONG_PRESS_PERIOD 800 // ms

typedef struct {
    uint8_t n;
    uint16_t cnt;
} ButtonParam;

static const GpioStruct button[] PROGMEM = {
    [SOURCE] = GPIO_STRUCT_INIT(C, 5),
    [SEEK_LEFT] = GPIO_STRUCT_INIT(C, 4),
    [PLAY_PAUSE] = GPIO_STRUCT_INIT(C, 3),
    [SEEK_RIGHT] = GPIO_STRUCT_INIT(C, 2),
    [VOLUME_DOWN] = GPIO_STRUCT_INIT(C, 1),
    [VOLUME_UP] = GPIO_STRUCT_INIT(C, 0),
};

static void buttonHandler(ButtonSource source, ButtonParam * param);

void buttonInit(void)
{
    SFIOR &= ~_BV(PUD);
    for (ButtonSource source = 0; source < NUM_OF_BUTTONS; ++source) {
        GpioStruct * gpio = getGpioStructFromFlash(&button[source]);
        setGpioDirection(gpio, INPUT);
        setGpioState(gpio, HIGH);
    }
}

ButtonState getButtonState(ButtonSource source)
{
    GpioStruct * gpio = getGpioStructFromFlash(&button[source]);

    return getGpioState(gpio);
}

typedef enum {
    BUTTON_IS_RELEASED,
    BUTTON_WAS_PRESSED,
    BUTTON_IS_PRESSED,
    BUTTON_WAS_RELEASED,
} ButtonLabel;

static void buttonHandler(ButtonSource source, ButtonParam * param)
{
    static uint8_t cmd;
    static uint8_t debCnt;
    static const void * const label[] = {
        &&BUTTON_IS_RELEASED,
        &&BUTTON_WAS_PRESSED,
        &&BUTTON_IS_PRESSED,
        &&BUTTON_WAS_RELEASED,
    };
    ButtonState state = getButtonState(source);

    uint8_t n = param->n;
    uint16_t cnt = param->cnt;

    ButtonAction action;

    goto * label[n];

    do {

    BUTTON_IS_RELEASED:
        n = (state == PRESSED) ? BUTTON_WAS_PRESSED : BUTTON_IS_RELEASED;
        debCnt = 0;
        break;

    BUTTON_WAS_PRESSED:
        n = (state == RELEASED) ? BUTTON_IS_RELEASED : (++debCnt > DEBOUNCE_PERIOD) ? BUTTON_IS_PRESSED : BUTTON_WAS_PRESSED;
        break;

    BUTTON_IS_PRESSED:
        n = (state == RELEASED) ? BUTTON_WAS_RELEASED : BUTTON_IS_PRESSED;
        debCnt = 0;
        //action = (cnt < LONG_PRESS_PERIOD) ? SINGLE : HOLD;
        break;

    BUTTON_WAS_RELEASED:
        n = (state == PRESSED) ? BUTTON_IS_PRESSED : (++debCnt > DEBOUNCE_PERIOD) ? BUTTON_IS_RELEASED : BUTTON_WAS_RELEASED;
        break;

    } while(0);

    switch (n) {
        case BUTTON_IS_RELEASED: cnt = 0; break;
        case BUTTON_WAS_PRESSED: cnt++; break;
        case BUTTON_IS_PRESSED: cnt++; break;
        case BUTTON_WAS_RELEASED: cnt = 0; break;
    }

    param->n = n;
    param->cnt = cnt;
}

void buttonFsm(void)
{
    static ButtonParam param[NUM_OF_BUTTONS];

    for (ButtonSource source = 0; source < NUM_OF(button); ++source) {
        buttonHandler(source, &param[source]);
    }
}