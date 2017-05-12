#include "hal/uart.h"
#include "main.h"
#include "os/queue.h"
#include "app/cmd.h"
#include <avr/pgmspace.h>

#include <stddef.h>

#define UBRR ((uint16_t) 16) // 57.6k @ 8MHz/U2X=1
#define MESSAGE_START ((uint8_t) 0x89)

typedef enum {
    START       = ((uint8_t) 0),
    COMMAND     = ((uint8_t) 1),
    CHECKSUM    = ((uint8_t) 2),
} UartRxState;

extern volatile bool isEvent;

void uartInit(void)
{
    /*Set baud rate */
    UBRRH = (uint8_t)(UBRR >> 8);
    UBRRL = (uint8_t)(UBRR >> 0);
    /* Set double speed */
    UCSRA |= (1 << U2X);
    /* Enable receiver and transmitter */
    UCSRB = (1 << RXCIE) | (1 << RXEN) | (1 << TXEN);
    /* Set frame format: 8 data, 1 stop bit */
    UCSRC = (1 << URSEL) | (3 << UCSZ0);
}ISR(USART_RXC_vect){    static UartRxState state = START;    static uint8_t command;    uint8_t data = UDR;    switch (state) {        /* Start message */        case START:            if (data == MESSAGE_START) {                state = COMMAND;            }            break;        /* Command receiving */        case COMMAND:            command = data;            state = CHECKSUM;            break;        /* Checksum checking */        case CHECKSUM:            if ((uint8_t)(command + MESSAGE_START) == data) {                QueueElement element = {                    .cnt = 0,                    .func = getActionByRxCommand((command >> 4) & 0x0F),                    .param = (command >> 0) & 0x0F,                };                setQueueElement(&element);                isEvent = true;            }            state = START;            break;        /* Default state */        default:            state = START;            break;    }}