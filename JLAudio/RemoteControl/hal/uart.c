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

typedef enum {
    SOURCE_INDICATOR_OFF            = ((uint8_t) 0x9A),
    BUTTON_REPEAT_INTERVAL_0        = ((uint8_t) 0xB0),
    BUTTON_REPEAT_INTERVAL_1        = ((uint8_t) 0xB1),
    BUTTON_REPEAT_INTERVAL_2        = ((uint8_t) 0xB2),
    BUTTON_REPEAT_INTERVAL_3        = ((uint8_t) 0xB3),
    BUTTON_REPEAT_INTERVAL_4        = ((uint8_t) 0xB4),
    BUTTON_REPEAT_INTERVAL_5        = ((uint8_t) 0xB5),
    BUTTON_REPEAT_INTERVAL_6        = ((uint8_t) 0xB6),
    BUTTON_REPEAT_INTERVAL_7        = ((uint8_t) 0xB7),
    BUTTON_REPEAT_INTERVAL_8        = ((uint8_t) 0xB8),
    BUTTON_REPEAT_INTERVAL_9        = ((uint8_t) 0xB9),
    BUTTON_REPEAT_INTERVAL_10       = ((uint8_t) 0xBA),
    BUTTON_REPEAT_INTERVAL_11       = ((uint8_t) 0xBB),
    BUTTON_REPEAT_INTERVAL_12       = ((uint8_t) 0xBC),
    BUTTON_REPEAT_INTERVAL_13       = ((uint8_t) 0xBD),
    BUTTON_REPEAT_INTERVAL_14       = ((uint8_t) 0xBE),
    BUTTON_REPEAT_INTERVAL_15       = ((uint8_t) 0xBF),
    PANEL_ILLUMINATION_LEVEL_0      = ((uint8_t) 0xC0),
    PANEL_ILLUMINATION_LEVEL_1      = ((uint8_t) 0xC1),
    PANEL_ILLUMINATION_LEVEL_2      = ((uint8_t) 0xC2),
    PANEL_ILLUMINATION_LEVEL_3      = ((uint8_t) 0xC3),
    PANEL_ILLUMINATION_LEVEL_4      = ((uint8_t) 0xC4),
    PANEL_ILLUMINATION_LEVEL_5      = ((uint8_t) 0xC5),
    PANEL_ILLUMINATION_LEVEL_6      = ((uint8_t) 0xC6),
    PANEL_ILLUMINATION_LEVEL_7      = ((uint8_t) 0xC7),
    PANEL_ILLUMINATION_LEVEL_8      = ((uint8_t) 0xC8),
    PANEL_ILLUMINATION_LEVEL_9      = ((uint8_t) 0xC9),
    PANEL_ILLUMINATION_LEVEL_10     = ((uint8_t) 0xCA),
    PANEL_ILLUMINATION_LEVEL_11     = ((uint8_t) 0xCB),
    PANEL_ILLUMINATION_LEVEL_12     = ((uint8_t) 0xCC),
    PANEL_ILLUMINATION_LEVEL_13     = ((uint8_t) 0xCD),
    PANEL_ILLUMINATION_LEVEL_14     = ((uint8_t) 0xCE),
    PANEL_ILLUMINATION_LEVEL_15     = ((uint8_t) 0xCF),
    AUDIO_SOURCE_AM                 = ((uint8_t) 0xD0),
    AUDIO_SOURCE_FM                 = ((uint8_t) 0xD1),
    AUDIO_SOURCE_WB                 = ((uint8_t) 0xD2),
    AUDIO_SOURCE_SXM                = ((uint8_t) 0xD3),
    AUDIO_SOURCE_BT                 = ((uint8_t) 0xD4),
    AUDIO_SOURCE_AUX                = ((uint8_t) 0xD5),
    AUDIO_SOURCE_USB                = ((uint8_t) 0xD6),
} UartRxCmd;

static const Func cmd2func[] PROGMEM = {
    [0x00 ... 0xFF]                 = NULL,
    [SOURCE_INDICATOR_OFF]          = SourceIndicatorOff,
    [BUTTON_REPEAT_INTERVAL_0]      = ButtonRepeatInterval_0,
    [BUTTON_REPEAT_INTERVAL_1]      = ButtonRepeatInterval_1,
    [BUTTON_REPEAT_INTERVAL_2]      = ButtonRepeatInterval_2,
    [BUTTON_REPEAT_INTERVAL_3]      = ButtonRepeatInterval_3,
    [BUTTON_REPEAT_INTERVAL_4]      = ButtonRepeatInterval_4,
    [BUTTON_REPEAT_INTERVAL_5]      = ButtonRepeatInterval_5,
    [BUTTON_REPEAT_INTERVAL_6]      = ButtonRepeatInterval_6,
    [BUTTON_REPEAT_INTERVAL_7]      = ButtonRepeatInterval_7,
    [BUTTON_REPEAT_INTERVAL_8]      = ButtonRepeatInterval_8,
    [BUTTON_REPEAT_INTERVAL_9]      = ButtonRepeatInterval_9,
    [BUTTON_REPEAT_INTERVAL_10]     = ButtonRepeatInterval_10,
    [BUTTON_REPEAT_INTERVAL_11]     = ButtonRepeatInterval_11,
    [BUTTON_REPEAT_INTERVAL_12]     = ButtonRepeatInterval_12,
    [BUTTON_REPEAT_INTERVAL_13]     = ButtonRepeatInterval_13,
    [BUTTON_REPEAT_INTERVAL_14]     = ButtonRepeatInterval_14,
    [BUTTON_REPEAT_INTERVAL_15]     = ButtonRepeatInterval_15,
    [PANEL_ILLUMINATION_LEVEL_0]    = PanelIlluminationLevel_0,
    [PANEL_ILLUMINATION_LEVEL_1]    = PanelIlluminationLevel_1,
    [PANEL_ILLUMINATION_LEVEL_2]    = PanelIlluminationLevel_2,
    [PANEL_ILLUMINATION_LEVEL_3]    = PanelIlluminationLevel_3,
    [PANEL_ILLUMINATION_LEVEL_4]    = PanelIlluminationLevel_4,
    [PANEL_ILLUMINATION_LEVEL_5]    = PanelIlluminationLevel_5,
    [PANEL_ILLUMINATION_LEVEL_6]    = PanelIlluminationLevel_6,
    [PANEL_ILLUMINATION_LEVEL_7]    = PanelIlluminationLevel_7,
    [PANEL_ILLUMINATION_LEVEL_8]    = PanelIlluminationLevel_8,
    [PANEL_ILLUMINATION_LEVEL_9]    = PanelIlluminationLevel_9,
    [PANEL_ILLUMINATION_LEVEL_10]   = PanelIlluminationLevel_10,
    [PANEL_ILLUMINATION_LEVEL_11]   = PanelIlluminationLevel_11,
    [PANEL_ILLUMINATION_LEVEL_12]   = PanelIlluminationLevel_12,
    [PANEL_ILLUMINATION_LEVEL_13]   = PanelIlluminationLevel_13,
    [PANEL_ILLUMINATION_LEVEL_14]   = PanelIlluminationLevel_14,
    [PANEL_ILLUMINATION_LEVEL_15]   = PanelIlluminationLevel_15,
    [AUDIO_SOURCE_AM]               = AudioSource_AM,
    [AUDIO_SOURCE_FM]               = AudioSource_FM,
    [AUDIO_SOURCE_WB]               = AudioSource_WB,
    [AUDIO_SOURCE_SXM]              = AudioSource_SXM,
    [AUDIO_SOURCE_BT]               = AudioSource_BT,
    [AUDIO_SOURCE_AUX]              = AudioSource_AUX,
    [AUDIO_SOURCE_USB]              = AudioSource_USB,
};

extern volatile UartRx uartRx;

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
}ISR(USART_RXC_vect){    static UartRxState state = START;    static uint8_t command;    switch (state) {        /* Start message */        case START:            state = (UDR == MESSAGE_START) ? COMMAND : START;            break;        /* Command receiving */        case COMMAND:            command = UDR;            state = CHECKSUM;            break;        /* Checksum checking */        case CHECKSUM:            if (command + MESSAGE_START == UDR) {                QueueElement element = {                    .cnt = 0,                    .func = (Func)pgm_read_word(&cmd2func[command]),                };                setQueueElement(&element);            }            state = START;            break;        /* Default state */        default:            state = START;            break;    }}