#ifndef UART_H_
#define UART_H_

#include <stdbool.h>
#include <avr/io.h>

void uartInit(void);
uint8_t getUartCmdFormQueue(void);
bool isCmdQueueEmpty(void);

#endif /* UART_H_ */