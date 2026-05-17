#include "ping_pong_sound.h"

#include <stdint.h>

#include "sound.h"
#include "pitches.h"

static const uint32_t retroMusicFreqs[] =
{
  NOTE_E4, 0, NOTE_G4, 0, NOTE_B4, 0, NOTE_A4, 0, NOTE_G4, 0,
  NOTE_E4, 0, NOTE_C4, 0, NOTE_E4, 0, NOTE_G4, 0, 0
};

static const uint32_t retroMusicDurationMs[] =
{
  300, 400, 300, 400, 400, 500, 300, 400, 300,
  400, 600, 400, 400, 400, 400, 400, 800, 900, 0
};

static const uint32_t metalMusicFreqs[] =
{
  // intro1
  NOTE_E3, 0, NOTE_D4, 0, NOTE_CS4, 0, NOTE_C4, 0,
  // intro2
  NOTE_E3, 0, NOTE_E3, 0, NOTE_E4, 0, NOTE_E3, 0, NOTE_E3, 0, NOTE_DS4, 0, NOTE_E3, 0, NOTE_E3, 0, NOTE_D4, 0, NOTE_CS4, 0, NOTE_C4, 0,
  // intro3
  NOTE_E3, 0, NOTE_E3, 0, NOTE_B3, 0, NOTE_E3, 0, NOTE_E3, 0, NOTE_AS3, 0, NOTE_E3, 0, NOTE_E3, 0, NOTE_A3, 0, NOTE_E3, 0, NOTE_GS3, 0, NOTE_E3, 0, NOTE_G3, 0, NOTE_E3, 0, NOTE_FS3, 0, NOTE_E3, 0,
  // intro2 повтор
  NOTE_E3, 0, NOTE_E3, 0, NOTE_E4, 0, NOTE_E3, 0, NOTE_E3, 0, NOTE_DS4, 0, NOTE_E3, 0, NOTE_E3, 0, NOTE_D4, 0, NOTE_CS4, 0, NOTE_C4, 0,
  // intro3 повтор
  NOTE_E3, 0, NOTE_E3, 0, NOTE_B3, 0, NOTE_E3, 0, NOTE_E3, 0, NOTE_AS3, 0, NOTE_E3, 0, NOTE_E3, 0, NOTE_A3, 0, NOTE_E3, 0, NOTE_GS3, 0, NOTE_E3, 0, NOTE_G3, 0, NOTE_E3, 0, NOTE_FS3, 0, NOTE_E3, 0,
  // preverse1
  NOTE_E3, 0, NOTE_F3, 0, NOTE_B3, 0, NOTE_E3, 0, NOTE_F3, 0, NOTE_C4, 0, NOTE_E3, 0, NOTE_F3, 0, NOTE_CS4, 0, NOTE_E3, 0, NOTE_F3, 0, NOTE_C4, 0, NOTE_E3, 0, NOTE_F3, 0, NOTE_B3, 0, NOTE_B3, 0,
  // preverse2
  NOTE_E3, 0, NOTE_F3, 0, NOTE_B3, 0, NOTE_E3, 0, NOTE_F3, 0, NOTE_C4, 0, NOTE_E3, 0, NOTE_F3, 0, NOTE_CS4, 0, NOTE_E3, 0, NOTE_F3, 0, NOTE_C4, 0, NOTE_E3, 0, NOTE_F3, 0, NOTE_B3, 0, 0, 0,
  // preverse1 повтор
  NOTE_E3, 0, NOTE_F3, 0, NOTE_B3, 0, NOTE_E3, 0, NOTE_F3, 0, NOTE_C4, 0, NOTE_E3, 0, NOTE_F3, 0, NOTE_CS4, 0, NOTE_E3, 0, NOTE_F3, 0, NOTE_C4, 0, NOTE_E3, 0, NOTE_F3, 0, NOTE_B3, 0, NOTE_B3, 0,
  // preverse3
  NOTE_E3, 0, NOTE_F3, 0, NOTE_B3, 0, NOTE_E3, 0, NOTE_F3, 0, NOTE_C4, 0, NOTE_E3, 0, NOTE_F3, 0, NOTE_G3, 0, NOTE_FS3, 0, NOTE_E3, 0, NOTE_G3, 0, NOTE_FS3, 0, NOTE_E3, 0, NOTE_G3, 0, NOTE_FS3, 0,
  // endIntro
  NOTE_G3, 0, NOTE_FS3, 0, NOTE_E3, 0, NOTE_G3, 0, NOTE_FS3, 0, NOTE_E3, 0, NOTE_G3, 0, NOTE_FS3, 0, NOTE_E3, 0, NOTE_DS4, 0, NOTE_A5, 0, NOTE_E3, 0, NOTE_DS4, 0, NOTE_A5, 0, NOTE_E3, 0, NOTE_DS4, 0, NOTE_A5, 0, NOTE_E3, 0, NOTE_DS4, 0, NOTE_A5, 0, NOTE_E3, 0, NOTE_DS4, 0, NOTE_A5, 0,
  // eightVampE
  NOTE_E3, 0, NOTE_E3, 0, NOTE_E3, 0, NOTE_E3, 0, NOTE_E3, 0, NOTE_E3, 0, NOTE_E3, 0, NOTE_E3, 0,
  // verse1
  0, 0, NOTE_G3, 0, NOTE_A3, 0, 0, 0, NOTE_AS3, 0, NOTE_A3, 0, NOTE_G3, 0, NOTE_A3, 0,
  // eightVampE повтор
  NOTE_E3, 0, NOTE_E3, 0, NOTE_E3, 0, NOTE_E3, 0, NOTE_E3, 0, NOTE_E3, 0, NOTE_E3, 0, NOTE_E3, 0,
  // verse2
  NOTE_A3, 0, 0, 0, NOTE_A3, 0, 0, 0,
  // eightVampE повтор
  NOTE_E3, 0, NOTE_E3, 0, NOTE_E3, 0, NOTE_E3, 0, NOTE_E3, 0, NOTE_E3, 0, NOTE_E3, 0, NOTE_E3, 0,
  // verse1 повтор
  0, 0, NOTE_G3, 0, NOTE_A3, 0, 0, 0, NOTE_AS3, 0, NOTE_A3, 0, NOTE_G3, 0, NOTE_A3, 0,
  // eightVampE повтор
  NOTE_E3, 0, NOTE_E3, 0, NOTE_E3, 0, NOTE_E3, 0, NOTE_E3, 0, NOTE_E3, 0, NOTE_E3, 0, NOTE_E3, 0,
  // verse2 повтор
  NOTE_A3, 0, 0, 0, NOTE_A3, 0, 0, 0,
  // eightVampFSharp
  NOTE_FS3, 0, NOTE_FS3, 0, NOTE_FS3, 0, NOTE_FS3, 0, NOTE_FS3, 0, NOTE_FS3, 0, NOTE_FS3, 0, NOTE_FS3, 0, 0
};

static const uint32_t metalMusicDurationMs[] =
{
  // intro1
  125, 163, 250, 325, 125, 163, 500, 650,
  // intro2
  63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,125,163,125,163,250,325,
  // intro3
  63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,
  // intro2
  63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,125,163,125,163,250,325,
  // intro3
  63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,
  // preverse1
  63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,
  // preverse2
  63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,
  // preverse1 повтор
  63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,
  // preverse3
  63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,
  // endIntro
  63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,250,325,
  // eightVampE
  63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,
  // verse1
  63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,
  // eightVampE
  63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,
  // verse2
  63,82,63,82,63,82,63,82,
  // eightVampE
  63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,
  // verse1
  63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,
  // eightVampE
  63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,82,
  // verse2
  63,82,63,82,63,82,63,82,
  // eightVampFSharp
  63,82,63,82,63,82,63,82,63,82,63,82,63,82,63,83,0
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

void playMenuMusic(enum Music music)
{
  if (!sndManager.processing)
  {
    switch (music)
    {
      case (RETRO):

        sndManager.freqs = retroMusicFreqs;
        sndManager.durationsMs = retroMusicDurationMs;
        sndManager.iteration = 0;
        toneMs(sndManager.freqs[sndManager.iteration],
        sndManager.durationsMs[sndManager.iteration]);
        ++sndManager.iteration;
        sndManager.processing = true;
        break;

      case (METAL):

        sndManager.freqs = metalMusicFreqs;
        sndManager.durationsMs = metalMusicDurationMs;
        sndManager.iteration = 0;
        toneMs(sndManager.freqs[sndManager.iteration],
        sndManager.durationsMs[sndManager.iteration]);
        ++sndManager.iteration;
        sndManager.processing = true;
        break;

      case (OFF):

        noTone();
        break;
    }


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
