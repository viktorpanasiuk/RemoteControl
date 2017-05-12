#include "drv/led.h"
#include "hal/gpio.h"

#include "main.h"

#define LED_LVL(name, lvl) GPIO_LVL_ ## lvl(LED_PORTx_ ## name, LED_Pxn_ ## name)
#define LED_DIR(name, dir) GPIO_DIR_ ## dir(LED_DDRx_ ## name, LED_DDxn_ ## name)

#define LED(name, dir, lvl) \
do {                        \
    LED_LVL(name, lvl);     \
    LED_DIR(name, dir);     \
} while(0)                  \

void ledInit(void)
{
    ledOff();
}

void ledSet(LedSource source, LedState state)
{

    ledOff();

    if (state == ON) {
        switch (source) {
        case AM: LED(AM, O, 1); break;
        case FM: LED(FM, O, 1); break;
        case WB: LED(WB, O, 1); break;
        case SXM:LED(SXM, O, 1); break;
        case BT: LED(BT, O, 1); break;
        case AUX: LED(AUX, O, 1); break;
        case USB: LED(USB, O, 1); break;
        default: break;
        }
    } else {
        switch (source) {
        case AM: LED(AM, O, 0); break;
        case FM: LED(FM, O, 0); break;
        case WB: LED(WB, O, 0); break;
        case SXM: LED(SXM, O, 0); break;
        case BT: LED(BT, O, 0); break;
        case AUX: LED(AUX, O, 0); break;
        case USB: LED(USB, O, 0); break;
        default: break;
        }
    }
}

void ledOff(void)
{
    LED(AM, O, 0);
    LED(FM, O, 0);
    LED(WB, O, 0);
    LED(SXM, O, 0);
    LED(BT, O, 0);
    LED(AUX, O, 0);
    LED(USB, O, 0);
}