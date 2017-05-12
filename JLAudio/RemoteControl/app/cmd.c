#include "app/cmd.h"
#include "drv/led.h"
#include "drv/pwm.h"
#include "main.h"

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stddef.h>

const uint8_t ledIlluminationLevelTable[16] PROGMEM = {
    [0] = LED_ILLUMINATION_LEVEL_0,
    [1] = LED_ILLUMINATION_LEVEL_1,
    [2] = LED_ILLUMINATION_LEVEL_2,
    [3] = LED_ILLUMINATION_LEVEL_3,
    [4] = LED_ILLUMINATION_LEVEL_4,
    [5] = LED_ILLUMINATION_LEVEL_5,
    [6] = LED_ILLUMINATION_LEVEL_6,
    [7] = LED_ILLUMINATION_LEVEL_7,
    [8] = LED_ILLUMINATION_LEVEL_8,
    [9] = LED_ILLUMINATION_LEVEL_9,
    [10] = LED_ILLUMINATION_LEVEL_10,
    [11] = LED_ILLUMINATION_LEVEL_11,
    [12] = LED_ILLUMINATION_LEVEL_12,
    [13] = LED_ILLUMINATION_LEVEL_13,
    [14] = LED_ILLUMINATION_LEVEL_14,
    [15] = LED_ILLUMINATION_LEVEL_15,
};

typedef enum {
    SOURCE_INDICATOR            = ((uint8_t) 0x09),
    BUTTON_REPEAT_INTERVAL      = ((uint8_t) 0x0B),
    PANEL_ILLUMINATION_LEVEL    = ((uint8_t) 0x0C),
    AUDIO_SOURCE                = ((uint8_t) 0x0D),
} UartRxCmd;

static const Func cmd2func[] PROGMEM = {
    [0x00 ... 0x0F]             = NULL,
    [SOURCE_INDICATOR]          = SourceIndicator,
    [BUTTON_REPEAT_INTERVAL]    = ButtonRepeatInterval,
    [PANEL_ILLUMINATION_LEVEL]  = PanelIlluminationLevel,
    [AUDIO_SOURCE]              = AudioSource,
};

void SourceIndicator(uint8_t param)
{
    ledOff();
}

void ButtonRepeatInterval(uint8_t interval)
{
    
}

void PanelIlluminationLevel(uint8_t level)
{
    setPwmLvl(pgm_read_byte(&ledIlluminationLevelTable[level]));
}

void AudioSource(uint8_t source)
{
    ledSet((LedSource)source, ON);
}

Func getActionByRxCommand(uint8_t command)
{
    return (Func)pgm_read_word(&cmd2func[command]);
}