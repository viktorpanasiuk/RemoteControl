#include "drv/led.h"
#include "hal/gpio.h"

#include <avr/pgmspace.h>

#include "main.h"

static LedSource prevSource = NONE;
static LedState prevState = OFF;

static const GpioStruct led[] PROGMEM = {
    [AM] = GPIO_STRUCT_INIT(D, 2),
    [FM] = GPIO_STRUCT_INIT(D, 3),
    [WB] = GPIO_STRUCT_INIT(D, 4),
    [SXM] = GPIO_STRUCT_INIT(D, 5),
    [BT] = GPIO_STRUCT_INIT(D, 6),
    [AUX] = GPIO_STRUCT_INIT(D, 7),
    [USB] = GPIO_STRUCT_INIT(B, 0),
};

static void setLed(LedSource source, LedState state)
{
    GpioStruct * gpio = getGpioStructFromFlash(&led[source]);

    setGpioState(gpio, state);
}

void ledInit(void)
{
    for(LedSource source = 0; source < NUM_OF(led); ++source) {
        GpioStruct * gpio = getGpioStructFromFlash(&led[source]);

        setGpioDirection(gpio, OUTPUT);
        setGpioState(gpio, LOW);
    }
}

void switchLedTo(LedSource source)
{
    if (source != prevSource) {
        if (prevState == ON)
            setLed(prevSource, prevState = OFF);
        if ((prevSource = source) != NONE)
            setLed(source, prevState = ON);
    }
}