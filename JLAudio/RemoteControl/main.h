#ifndef MAIN_H_
#define MAIN_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

typedef struct {
    uint8_t cmd;
    bool flag;
} UartRx;

#endif /* MAIN_H_ */