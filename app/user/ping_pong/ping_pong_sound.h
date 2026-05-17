#ifndef PING_PONG_SOUND_H
#define PING_PONG_SOUND_H

enum Music
{
  RETRO,
  METAL,
  OFF
};

void playMenuMusic(enum Music music) __attribute__((section(".ram_text")));
void doCollisionSound(void) __attribute__((section(".ram_text")));

#endif
