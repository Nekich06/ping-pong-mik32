#ifndef SOUND_H
#define SOUND_H

#include <stdint.h>

void soundInit(void);

void tone(uint32_t freq);
void toneMs(uint32_t freq, uint32_t duration);
void noTone(void);

void doCollisionSound(void);

#endif
