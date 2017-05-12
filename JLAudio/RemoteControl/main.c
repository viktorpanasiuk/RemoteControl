#include "main.h"
/* HAL */
#include "hal/uart.h"
/* OS */
#include "os/queue.h"
/* DRV */
#include "drv/led.h"
#include "drv/pwm.h"
#include <stddef.h>

volatile bool isEvent = false;

int main(void)
{
    QueueElement element;
    
    uartInit();
    ledInit();
    pwmInit();

    sei(); // Global enable interrupt
    /* Replace with your application code */
    while (1)
    {
        if (isEvent == true) {
            isEvent = false;
            if (getQueueElement(&element) == true) {
                if (element.func != NULL) {
                    (* element.func)(element.param);
                }
            }
        }
    }
}

