#include "main.h"
/* HAL */
#include "hal/uart.h"

volatile UartRx uartRx = {
    .cmd = 0x00,
    .flag = false,
};

typedef void (* Func)(void *);

int main(void)
{
    uartInit();

    sei(); // Global enable interrupt
    /* Replace with your application code */
    while (1) 
    {
        
    }
}

