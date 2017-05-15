#include "drv/button.h"
#include "hal/gpio.h"

#define DEBOUNCE_PERIOD 10 // ms
#define LONG_PRESS_PERIOD 300 // ms

typedef struct {
    uint8_t n;
    uint8_t cnt;
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
    static const void * const label[] = {
        &&BUTTON_IS_RELEASED,
        &&BUTTON_WAS_PRESSED,
        &&BUTTON_IS_PRESSED,
        &&BUTTON_WAS_RELEASED,
    };
    ButtonState state = getButtonState(source);

    uint8_t n = param->n;
    uint8_t cnt = param->cnt;

    goto * label[n];

    do {

    BUTTON_IS_RELEASED:
        if (state == PRESSED) {
            n = BUTTON_WAS_PRESSED;
        }
        break;

    BUTTON_WAS_PRESSED:
        if (++cnt == DEBOUNCE_PERIOD) {
            n = BUTTON_IS_PRESSED;
        } else if (state == RELEASED) {
            n = BUTTON_IS_RELEASED;
            cnt = 0;
        }
        break;

    BUTTON_IS_PRESSED:
        if (state == RELEASED) {
            n = BUTTON_WAS_RELEASED;
            cmd = (cnt < LONG_PRESS_PERIOD) ? 0xA1 : 0xA9;
            cnt = 0;
        } else {
            cnt++;
        }
        break;

    BUTTON_WAS_RELEASED:
        if (++cnt == DEBOUNCE_PERIOD) {
            n = BUTTON_IS_RELEASED;
            UDR = cmd;
        } else if (state == PRESSED) {
            n = BUTTON_IS_PRESSED;
            cnt = 0;
        }
        break;

    } while(0);

    param->n = n;
    param->cnt = cnt;
}

void buttonFsm(void)
{
    static ButtonParam param[NUM_OF_BUTTONS];

    for (ButtonSource source = 0; source < NUM_OF_BUTTONS; ++source) {
        buttonHandler(source, &param[source]);
    }
}