#ifndef MAIN_H_
#define MAIN_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#define LED_DDRx_AM     DDRD
#define LED_DDRx_FM     DDRD
#define LED_DDRx_WB     DDRD
#define LED_DDRx_SXM    DDRD
#define LED_DDRx_BT     DDRD
#define LED_DDRx_AUX    DDRD
#define LED_DDRx_USB    DDRB

#define LED_DDxn_AM     DDD2
#define LED_DDxn_FM     DDD3
#define LED_DDxn_WB     DDD4
#define LED_DDxn_SXM    DDD5
#define LED_DDxn_BT     DDD6
#define LED_DDxn_AUX    DDD7
#define LED_DDxn_USB    DDB0

#define LED_PORTx_AM    PORTD
#define LED_PORTx_FM    PORTD
#define LED_PORTx_WB    PORTD
#define LED_PORTx_SXM   PORTD
#define LED_PORTx_BT    PORTD
#define LED_PORTx_AUX   PORTD
#define LED_PORTx_USB   PORTB

#define LED_Pxn_AM      PD2
#define LED_Pxn_FM      PD3
#define LED_Pxn_WB      PD4
#define LED_Pxn_SXM     PD5
#define LED_Pxn_BT      PD6
#define LED_Pxn_AUX     PD7
#define LED_Pxn_USB     PB0

#define LED_ILLUMINATION_LEVEL_0    ((uint8_t) 0)
#define LED_ILLUMINATION_LEVEL_1    ((uint8_t) 2)
#define LED_ILLUMINATION_LEVEL_2    ((uint8_t) 4)
#define LED_ILLUMINATION_LEVEL_3    ((uint8_t) 6)
#define LED_ILLUMINATION_LEVEL_4    ((uint8_t) 8)
#define LED_ILLUMINATION_LEVEL_5    ((uint8_t) 10)
#define LED_ILLUMINATION_LEVEL_6    ((uint8_t) 12)
#define LED_ILLUMINATION_LEVEL_7    ((uint8_t) 14)
#define LED_ILLUMINATION_LEVEL_8    ((uint8_t) 16)
#define LED_ILLUMINATION_LEVEL_9    ((uint8_t) 18)
#define LED_ILLUMINATION_LEVEL_10   ((uint8_t) 20)
#define LED_ILLUMINATION_LEVEL_11   ((uint8_t) 22)
#define LED_ILLUMINATION_LEVEL_12   ((uint8_t) 24)
#define LED_ILLUMINATION_LEVEL_13   ((uint8_t) 26)
#define LED_ILLUMINATION_LEVEL_14   ((uint8_t) 28)
#define LED_ILLUMINATION_LEVEL_15   ((uint8_t) 30)

#endif /* MAIN_H_ */