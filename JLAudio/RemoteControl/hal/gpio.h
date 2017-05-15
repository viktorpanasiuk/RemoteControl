#ifndef GPIO_H_
#define GPIO_H_

#include <avr/io.h>
#include <avr/pgmspace.h>

#define GPIO_STRUCT_INIT(X, N)  \
{                               \
    .ddr = &DDR ## X,           \
    .port = &PORT ## X,         \
    .pin = &PIN ## X,           \
    .mask = _BV(N),             \
}                               \

typedef enum {
    INPUT,
    OUTPUT,
} GpioDirection;

typedef enum {
    LOW,
    HIGH,
} GpioState;

typedef struct {
    volatile uint8_t * ddr;
    volatile uint8_t * port;
    volatile uint8_t * pin;
    uint8_t mask;
} GpioStruct;

void setGpioDirection(GpioStruct * gpio, GpioDirection direction);
void setGpioState(GpioStruct * gpio, GpioState state);
GpioState getGpioState(GpioStruct * gpio);
GpioStruct * getGpioStructFromFlash(PGM_VOID_P pgmGpio);

#endif /* GPIO_H_ */