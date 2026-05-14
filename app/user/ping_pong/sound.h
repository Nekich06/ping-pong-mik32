#ifndef SOUND_H
#define SOUND_H

#include <stdint.h>

void soundInit(void);

void tone(uint32_t freq) __attribute__((section(".ram_text")));
void toneMs(uint32_t freq, uint32_t duration) __attribute__((section(".ram_text")));
void noTone(void) __attribute__((section(".ram_text")));

void doCollisionSound(void) __attribute__((section(".ram_text")));

#endif
