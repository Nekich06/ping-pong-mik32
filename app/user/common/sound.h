#ifndef SOUND_H
#define SOUND_H

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
  const uint32_t * freqs;
  const uint32_t * durationsMs;
  uint8_t iteration;
  bool processing;
} SoundManager;

extern SoundManager sndManager;

void soundInit(void);

void tone(uint32_t freq) __attribute__((section(".ram_text")));
void toneMs(uint32_t freq, uint32_t duration) __attribute__((section(".ram_text")));
void noTone(void) __attribute__((section(".ram_text")));

#endif
