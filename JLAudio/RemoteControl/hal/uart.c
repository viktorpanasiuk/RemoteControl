#include "hal/uart.h"
#include "main.h"
#include "os/queue.h"
#include "app/cmd.h"
#include <avr/pgmspace.h>
#include <stddef.h>

#define UBRR ((uint16_t) 16) // 57.6k @ 8MHz/U2X=1
#define MESSAGE_START ((uint8_t) 0x89)
#define SIZE_OF_CMD_QUEUE 8

typedef enum {
    START       = ((uint8_t) 0),
    COMMAND     = ((uint8_t) 1),
    CHECKSUM    = ((uint8_t) 2),
} UartRxState;

static void setUartCmdToQueue(uint8_t cmd);
static bool isCmdQueueFull(void);

static volatile uint8_t cmdQueue[SIZE_OF_CMD_QUEUE];
static volatile uint8_t queueCnt;

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
}ISR(USART_RXC_vect){    static void * state = &&START;    static uint8_t command;
    uint8_t data = UDR;    goto * state;    /* Start message */    START:        if (data == MESSAGE_START) {            state = &&COMMAND;        }        return;    /* Command receiving */    COMMAND:        command = data;        state = &&CHECKSUM;        return;    /* Checksum checking */    CHECKSUM:        if ((uint8_t)(command + MESSAGE_START) == data) {            if (!isCmdQueueFull()) {                setUartCmdToQueue(command);            }        }        state = &&START;        return;}static void setUartCmdToQueue(uint8_t cmd){    cmdQueue[queueCnt++] = cmd;}uint8_t getUartCmdFormQueue(void){    uint8_t cmd = cmdQueue[0];    for (uint8_t i = 0; i < queueCnt; ++i) {        cmdQueue[i] = cmdQueue[i+1];    }    cmdQueue[queueCnt--] = 0;    return cmd;}static bool isCmdQueueFull(void){    return (queueCnt == SIZE_OF_CMD_QUEUE - 1) ? true : false;}bool isCmdQueueEmpty(void){    return (queueCnt == 0) ? true : false;}