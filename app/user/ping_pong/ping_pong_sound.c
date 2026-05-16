#include "ping_pong_sound.h"

#include <stdint.h>

#include "sound.h"

static const uint32_t menuMusicFreqs[] =
{
  330, 0, 392, 0, 494, 0, 440, 0, 392, 0, 330, 0, 262, 0, 330, 0, 392, 0, 0
};

static const uint32_t menuMusicDurationMs[] =
{
  300, 400, 300, 400, 400, 500, 300, 400, 300, 400, 600, 400, 400, 400, 400, 400, 800, 900, 0
};

static const uint32_t collisionSoundFreqs[] =
{
  300, 285, 270, 255, 240, 225, 210, 195,
  180, 165, 150, 135, 120, 105, 90, 0, 120, 0
};

static const uint32_t collisionSoundDurationsMs[] =
{
  3, 3, 3, 3, 3, 3, 3, 3, 3,
  3, 3, 3, 3, 3, 3, 10, 60, 0
};

void playMenuMusic(void)
{
  if (!sndManager.processing)
  {
    sndManager.freqs = menuMusicFreqs;
    sndManager.durationsMs = menuMusicDurationMs;
    sndManager.iteration = 0;
    toneMs(sndManager.freqs[sndManager.iteration],
       sndManager.durationsMs[sndManager.iteration]);
    ++sndManager.iteration;
    sndManager.processing = true;
  }
}

void doCollisionSound(void)
{
  sndManager.freqs = collisionSoundFreqs;
  sndManager.durationsMs = collisionSoundDurationsMs;
  sndManager.iteration = 0;

  if (!sndManager.processing)
  {
    toneMs(sndManager.freqs[sndManager.iteration],
       sndManager.durationsMs[sndManager.iteration]);
    ++sndManager.iteration;
    sndManager.processing = true;
  }
}
