#include "hal/gpio.h"

void setGpioDirection(GpioStruct * gpio, GpioDirection direction)
{
    volatile uint8_t * ddr = gpio->ddr;
    uint8_t mask = gpio->mask;

    goto * (direction == INPUT) ? &&INPUT : &&OUTPUT;

    INPUT: *ddr &= ~mask; return;
    OUTPUT: *ddr |= mask; return;
}

void setGpioState(GpioStruct * gpio, GpioState state)
{
    volatile uint8_t * port = gpio->port;
    uint8_t mask = gpio->mask;

    goto * (state == LOW) ? &&LOW : &&HIGH;

    LOW: *port &= ~mask; return;
    HIGH: *port |= mask; return;
}

GpioState getGpioState(GpioStruct * gpio)
{
    volatile uint8_t * pin = gpio->pin;
    uint8_t mask = gpio->mask;

    return (*pin & mask) ? HIGH : LOW;
}

GpioStruct * getGpioStructFromFlash(PGM_VOID_P pgmGpio)
{
    static GpioStruct gpio;

    memcpy_P(&gpio, pgmGpio, sizeof(GpioStruct));

    return &gpio;
}