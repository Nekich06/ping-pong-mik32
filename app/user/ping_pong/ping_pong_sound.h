#ifndef PING_PONG_SOUND_H
#define PING_PONG_SOUND_H

void playMenuMusic(void) __attribute__((section(".ram_text")));
void doCollisionSound(void) __attribute__((section(".ram_text")));

#endif
