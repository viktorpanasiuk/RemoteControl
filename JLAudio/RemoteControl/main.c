#include "main.h"
/* HAL */
#include "hal/uart.h"
#include "hal/tim2.h"
/* OS */
#include "os/queue.h"
/* DRV */
#include "drv/led.h"
#include "drv/button.h"
#include "mdw/pwm.h"
/* APP */
#include "app/cmd.h"

#include <stddef.h>

volatile bool isTick = false;

int main(void)
{
    uartInit();
    ledInit();
    pwmInit();
    tim2Init();
    buttonInit();

    sei(); // Global enable interrupt
    /* Replace with your application code */
    for(;;) {
        if (isTick == true) {
            isTick = false;
            if (!isCmdQueueEmpty()) {
                uint8_t cmd = getUartCmdFormQueue();
                uint8_t param = cmd & 0x0F;
                cmd = (cmd >> 4) & 0x0F;
                Func action = getActionByRxCommand(cmd);
                if (action != NULL) {
                    (* action)(param);
                }
            }
            buttonFsm();
        }
    }
}

