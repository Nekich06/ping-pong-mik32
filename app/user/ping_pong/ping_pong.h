#ifndef PING_PONG_H
#define PING_PONG_H

#include "ssd1306_spi.h"

// Players Parameters
#define PLATFORM_WIDTH      (SSD1306_WIDTH / 32)
#define PLATFORM_HEIGHT     (SSD1306_HEIGHT / 4)
#define PLAYER_1_START_XPOS (0)
#define PLAYER_1_START_YPOS (24)
#define PLAYER_2_START_XPOS (124)
#define PLAYER_2_START_YPOS (24)
#define PLAYER_SPEED        (2)

// Orb Parameters
#define ORB_START_XPOS (64)
#define ORB_START_YPOS (32)
#define ORB_RADIUS     (2)
#define ORB_SPEED_MIN  (1)
#define ORB_SPEED_MAX  (2)

typedef struct _Player
{
  int8_t x;
  int8_t y;
} Player;

typedef struct _Robot
{
  Player coordinates;
  bool direction;
} Robot;

typedef struct _Orb
{
  int8_t x;
  int8_t y;
  int8_t x_speed;
  int8_t y_speed;
  bool x_direction;
  bool y_direction;
} Orb;

Player createPlayer(int8_t x_pos, int8_t y_pos);
Robot createRobot(int8_t x_pos, int8_t y_pos, bool direction);
Orb createOrb(int8_t x_pos, int8_t y_pos, int8_t x_speed, int8_t y_speed, bool x_dir, bool y_dir);

void drawOrb(Orb * orb);
void drawPlatform(Player * player);

void showMenu(void);
void chooseModeMode(void);
void initGraphics(void);
void showGameOver(void);
void incScorePlayer_1();
void incScorePlayer_2();

void changeOrbPosition(Orb * orb);
bool changeOrbDirIfCollisions(Orb * orb, Player * player_1, Player * player_2);
void changePlayersPosIfButtonsPressed(Player * player_1, Player * player_2);
void changePlayerPosIfButtonPressedAndManageRobot(Player * player, Robot * robot);

void pingpong(void);

#endif
