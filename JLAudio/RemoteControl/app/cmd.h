#ifndef CMD_H_
#define CMD_H_

#include "os/queue.h"

void SourceIndicator(uint8_t param);
void ButtonRepeatInterval(uint8_t interval);
void PanelIlluminationLevel(uint8_t level);
void AudioSource(uint8_t source);

Func getActionByRxCommand(uint8_t command);

#endif /* CMD_H_ */