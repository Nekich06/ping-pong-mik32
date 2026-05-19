#include "ping_pong.h"

#include <stdlib.h>

#include "scr1_timer_delay.h"
#include "ping_pong_buttons.h"
#include "ping_pong_sound.h"
#include "mik32_hal_gpio.h"
#include "ssd1306_gfx.h"
#include "sound.h"

#define PLATFORM_WIDTH      (SSD1306_WIDTH / 32)
#define PLATFORM_HEIGHT     (SSD1306_HEIGHT / 4)
#define PLAYER_1_START_XPOS (0)
#define PLAYER_1_START_YPOS (24)
#define PLAYER_2_START_XPOS (124)
#define PLAYER_2_START_YPOS (24)
#define PLAYER_SPEED        (2)

#define ORB_START_XPOS (64)
#define ORB_START_YPOS (32)
#define ORB_RADIUS     (2)
#define ORB_SPEED_MIN  (1)
#define ORB_SPEED_MAX  (2)

#define ORB_SPEED_RAND() (ORB_SPEED_MIN + rand() % (ORB_SPEED_MAX - ORB_SPEED_MIN + 1))
#define DIRECTION_RAND() (0 + rand() % (2 - 0 + 1))

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

static struct Gameplay
{
  enum Mode
  {
    PLAYER_VS_PLAYER,
    PLAYER_VS_ROBOT,
    SETTINGS
  } mode;

  enum Settings
  {
    MAX_SCORE,
    MUSIC,
    GO_BACK
  } settings;

  struct Counter
  {
    int8_t player_1_score;
    int8_t player_2_score;
    char score_buf[3];
  } counter;

  enum Music music;
  uint8_t max_score;
  bool is_game_over;
  bool player_1_won;
} gameplay;


static Player createPlayer(int8_t x_pos, int8_t y_pos);
static Robot createRobot(int8_t x_pos, int8_t y_pos, bool direction);
static Orb createOrb(int8_t x_pos, int8_t y_pos, int8_t x_speed, int8_t y_speed, bool x_dir, bool y_dir);

static void drawOrb(Orb * orb);
static void drawPlatform(Player * player);

static void showMenu(void) __attribute__((section(".ram_text")));
static void chooseMode(void);
static void showSettings(void);
static void settingsMode(void);
static void initGraphics(void);
static void showGameOver(void);
static void incScorePlayer_1();
static void incScorePlayer_2();
static void resetCounter();

static void changeOrbPosition(Orb * orb) __attribute__((section(".ram_text")));
static bool changeOrbDirIfCollisions(Orb * orb, Player * player_1, Player * player_2) __attribute__((section(".ram_text")));
static void changePlayersPosIfButtonsPressed(Player * player_1, Player * player_2) __attribute__((section(".ram_text")));
static void changePlayerPosIfButtonPressedAndManageRobot(Player * player, Robot * robot) __attribute__((section(".ram_text")));

void pingpong(void)
{
  gameplay.max_score = 3;
  gameplay.music = RETRO;

  for (;;)
  {
    gameplay.mode = PLAYER_VS_PLAYER;
    chooseMode();
    SSD1306_ClearDisplay();

    if (gameplay.mode == PLAYER_VS_PLAYER)
    {
      for (;;)
      {
        if (gameplay.is_game_over)
        {
          gameplay.is_game_over = false;
          resetCounter();
          showGameOver();
          break;
        }
        Player player_1 = createPlayer(PLAYER_1_START_XPOS, PLAYER_1_START_YPOS);
        Player player_2 = createPlayer(PLAYER_2_START_XPOS, PLAYER_2_START_YPOS);

        Orb orb = createOrb(ORB_START_XPOS, ORB_START_YPOS, ORB_SPEED_RAND(), ORB_SPEED_RAND(), DIRECTION_RAND(), DIRECTION_RAND());

        initGraphics();
        SSD1306_Display();
        SCR1_Timer_Delay(1500000);
        for (;;)
        {
          SSD1306_ClearDisplay();
          changeOrbPosition(&orb);
          if (changeOrbDirIfCollisions(&orb, &player_1, &player_2))
          {
            SCR1_Timer_Delay(1500000);
            break;
          }
          changePlayersPosIfButtonsPressed(&player_1, &player_2);

          drawPlatform(&player_1);
          drawPlatform(&player_2);
          drawOrb(&orb);
          SSD1306_Display();
        }
      }
    }
    else
    {
      for (;;)
      {
        if (gameplay.is_game_over)
        {
          gameplay.is_game_over = false;
          resetCounter();
          showGameOver();
          break;
        }
        Player player_1 = createPlayer(PLAYER_1_START_XPOS, PLAYER_1_START_YPOS);
        Robot robot = createRobot(PLAYER_2_START_XPOS, PLAYER_2_START_YPOS, DIRECTION_RAND());

        Orb orb = createOrb(ORB_START_XPOS, ORB_START_YPOS, ORB_SPEED_RAND(), ORB_SPEED_RAND(), DIRECTION_RAND(), DIRECTION_RAND());

        initGraphics();
        SSD1306_Display();
        SCR1_Timer_Delay(1500000);
        for (;;)
        {
          SSD1306_ClearDisplay();
          changeOrbPosition(&orb);
          if (changeOrbDirIfCollisions(&orb, &player_1, &robot.coordinates))
          {
            SCR1_Timer_Delay(1500000);
            break;
          }
          changePlayerPosIfButtonPressedAndManageRobot(&player_1, &robot);

          drawPlatform(&player_1);
          drawPlatform(&robot.coordinates);
          drawOrb(&orb);
          SSD1306_Display();
        }
      }
    }
  }
}

Player createPlayer(int8_t x_pos, int8_t y_pos)
{
  return (Player){ .x = x_pos, .y = y_pos };
}

Robot createRobot(int8_t x_pos, int8_t y_pos, bool direction)
{
  return (Robot){ .coordinates.x = x_pos, .coordinates.y = y_pos, .direction = direction };
}

Orb createOrb(int8_t x_pos, int8_t y_pos, int8_t x_speed, int8_t y_speed, bool x_dir, bool y_dir)
{
  return (Orb){ .x = x_pos, .y = y_pos, .x_speed = x_speed, .y_speed = y_speed, .x_direction = x_dir, .y_direction = y_dir };
}

void drawOrb(Orb * orb)
{
  SSD1306_FillCircle(orb->x, orb->y, ORB_RADIUS, SSD1306_WHITE);
}

void drawPlatform(Player * player)
{
  SSD1306_FillRect(player->x, player->y, PLATFORM_WIDTH, PLATFORM_HEIGHT, SSD1306_WHITE);
}

void showMenu(void)
{
  SSD1306_ClearDisplay();

  SSD1306_SetTextColor(SSD1306_WHITE);
  SSD1306_SetTextSize(2);
  SSD1306_SetCursor(10, 10);
  SSD1306_WriteText("PING-PONG");
  SSD1306_FillCircle(87, 15, 1, SSD1306_WHITE);
  SSD1306_SetTextSize(1);
  SSD1306_SetCursor(10, 30);
  switch (gameplay.mode)
  {
    case (PLAYER_VS_PLAYER):

      SSD1306_WriteText("> PLAYER VS PLAYER");
      SSD1306_SetCursor(10, 42);
      SSD1306_WriteText("  PLAYER VS ROBOT");
      SSD1306_SetCursor(10, 54);
      SSD1306_WriteText("  SETTINGS");
      break;

    case (PLAYER_VS_ROBOT):

      SSD1306_WriteText("  PLAYER VS PLAYER");
      SSD1306_SetCursor(10, 42);
      SSD1306_WriteText("> PLAYER VS ROBOT");
      SSD1306_SetCursor(10, 54);
      SSD1306_WriteText("  SETTINGS");
      break;

    case (SETTINGS):

      SSD1306_WriteText("  PLAYER VS PLAYER");
      SSD1306_SetCursor(10, 42);
      SSD1306_WriteText("  PLAYER VS ROBOT");
      SSD1306_SetCursor(10, 54);
      SSD1306_WriteText("> SETTINGS");
      break;
  }

  SSD1306_Display();
}

void chooseMode(void)
{
  showMenu();

  while (1)
  {
    playMenuMusic(gameplay.music);
    if (HAL_GPIO_ReadPin(BUTTON_DOWN_PLAYER_1_PORT, 1 << BUTTON_DOWN_PLAYER_1_PIN) == GPIO_PIN_HIGH)
    {
      switch (gameplay.mode)
      {
        case (PLAYER_VS_PLAYER):

          gameplay.mode = PLAYER_VS_ROBOT;
          break;

        case (PLAYER_VS_ROBOT):

          gameplay.mode = SETTINGS;
          break;

        case (SETTINGS):

          gameplay.mode = PLAYER_VS_PLAYER;
          break;
      }

      showMenu();
      SCR1_Timer_Delay(200000);
    }

    if (HAL_GPIO_ReadPin(BUTTON_UP_PLAYER_1_PORT, 1 << BUTTON_UP_PLAYER_1_PIN) == GPIO_PIN_HIGH)
    {
      if (gameplay.mode == SETTINGS)
      {
        noTone();
        settingsMode();
        gameplay.mode = PLAYER_VS_PLAYER;
        showMenu();
        SCR1_Timer_Delay(200000);
      }
      else
      {
        break;
      }
    }
  }

  noTone();
}

void showSettings(void)
{
  SSD1306_ClearDisplay();

  SSD1306_SetTextSize(2);
  SSD1306_SetCursor(10, 10);
  SSD1306_WriteText("SETTINGS");
  SSD1306_SetTextSize(1);
  SSD1306_SetCursor(10, 30);
  switch (gameplay.settings)
  {
    case (MAX_SCORE):

      SSD1306_WriteText("> MAX SCORE: ");
      SSD1306_SetCursor(10, 42);
      SSD1306_WriteText("  MUSIC: ");
      SSD1306_SetCursor(10, 54);
      SSD1306_WriteText("  GO BACK");
      break;

    case (MUSIC):

      SSD1306_WriteText("  MAX SCORE: ");
      SSD1306_SetCursor(10, 42);
      SSD1306_WriteText("> MUSIC: ");
      SSD1306_SetCursor(10, 54);
      SSD1306_WriteText("  GO BACK");
      break;

    case (GO_BACK):

      SSD1306_WriteText("  MAX SCORE: ");
      SSD1306_SetCursor(10, 42);
      SSD1306_WriteText("  MUSIC: ");
      SSD1306_SetCursor(10, 54);
      SSD1306_WriteText("< GO BACK");
      break;
  }

  SSD1306_SetCursor(100, 30);
  SSD1306_WriteText(itoa(gameplay.max_score, gameplay.counter.score_buf, 10));

  SSD1306_SetCursor(90, 42);
  switch (gameplay.music)
  {
    case (RETRO):

      SSD1306_WriteText("RETRO");
      break;

    case (METAL):

      SSD1306_WriteText("METAL");
      break;

    case (OFF):

      SSD1306_WriteText(" OFF");
      break;
  }

  SSD1306_Display();
}

void settingsMode(void)
{
  gameplay.settings = MAX_SCORE;
  showSettings();
  SCR1_Timer_Delay(200000);

  while (1)
  {
    if (HAL_GPIO_ReadPin(BUTTON_DOWN_PLAYER_1_PORT, 1 << BUTTON_DOWN_PLAYER_1_PIN) == GPIO_PIN_HIGH)
    {
      switch (gameplay.settings)
      {
        case (MAX_SCORE):

          gameplay.settings = MUSIC;
          break;

        case (MUSIC):

          gameplay.settings = GO_BACK;
          break;

        case (GO_BACK):

          gameplay.settings = MAX_SCORE;
          break;
      }

      showSettings();
      SCR1_Timer_Delay(200000);
    }

    if (HAL_GPIO_ReadPin(BUTTON_UP_PLAYER_1_PORT, 1 << BUTTON_UP_PLAYER_1_PIN) == GPIO_PIN_HIGH)
    {
      if (gameplay.settings == MAX_SCORE)
      {
        if (gameplay.max_score != 10)
        {
          ++gameplay.max_score;
        }
        else
        {
          gameplay.max_score = 1;
        }
      }
      else if (gameplay.settings == MUSIC)
      {
        if (gameplay.music == RETRO)
        {
          gameplay.music = METAL;
        }
        else if (gameplay.music == METAL)
        {
          gameplay.music = OFF;
        }
        else
        {
          gameplay.music = RETRO;
        }
      }
      else
      {
        break;
      }

      showSettings();
      SCR1_Timer_Delay(200000);
    }
  }
}

void initGraphics(void)
{
  SSD1306_FillRect(PLAYER_1_START_XPOS, PLAYER_1_START_YPOS, PLATFORM_WIDTH, PLATFORM_HEIGHT, SSD1306_WHITE);
  SSD1306_FillRect(PLAYER_2_START_XPOS, PLAYER_2_START_YPOS, PLATFORM_WIDTH, PLATFORM_HEIGHT, SSD1306_WHITE);
  SSD1306_FillCircle(ORB_START_XPOS, ORB_START_YPOS, ORB_RADIUS, SSD1306_WHITE);
  SSD1306_SetTextSize(2);
  SSD1306_SetCursor(32, 5);
  SSD1306_WriteText(itoa(gameplay.counter.player_1_score, gameplay.counter.score_buf, 10));
  SSD1306_SetCursor(88, 5);
  SSD1306_WriteText(itoa(gameplay.counter.player_2_score, gameplay.counter.score_buf, 10));
}

void showGameOver(void)
{
  SSD1306_SetTextSize(2);
  if (gameplay.player_1_won)
  {
    SSD1306_SetCursor(14, 20);
    SSD1306_WriteText("PLAYER 1");
    SSD1306_SetCursor(43, 36);
    SSD1306_WriteText("WON");
  }
  else
  {
    if (gameplay.mode == PLAYER_VS_PLAYER)
    {
      SSD1306_SetCursor(14, 20);
      SSD1306_WriteText("PLAYER 2");
      SSD1306_SetCursor(43, 36);
      SSD1306_WriteText("WON");
    }
    else
    {
      SSD1306_SetCursor(12, 20);
      SSD1306_WriteText("  ROBOT");
      SSD1306_SetCursor(48, 36);
      SSD1306_WriteText("WON");
    }
  }
  SSD1306_Display();
  SCR1_Timer_Delay(2000000);
}

void incScorePlayer_1()
{
  if (++gameplay.counter.player_1_score == gameplay.max_score)
  {
    gameplay.is_game_over = true;
    gameplay.player_1_won = true;
  }
}

void incScorePlayer_2()
{
  if (++gameplay.counter.player_2_score == gameplay.max_score)
  {
    gameplay.is_game_over = true;
    gameplay.player_1_won = false;
  }
}

void resetCounter()
{
  gameplay.counter.player_1_score = 0;
  gameplay.counter.player_2_score = 0;
}

void changeOrbPosition(Orb * orb)
{
  if (orb->x_direction)
  {
    orb->x += orb->x_speed;
  }
  else
  {
    orb->x -= orb->x_speed;
  }

  if (orb->y_direction)
  {
    orb->y += orb->y_speed;
  }
  else
  {
    orb->y -= orb->y_speed;
  }
}

bool changeOrbDirIfCollisions(Orb * orb, Player * player_1, Player * player_2)
{
  if (orb->x <= 0)
  {
    incScorePlayer_2();
    return 1;
  }
  else if (orb->x >= (SSD1306_WIDTH - ORB_RADIUS))
  {
    incScorePlayer_1();
    return 1;
  }

  if (orb->y <= 0)
  {
    doCollisionSound();
    orb->y_direction = true;
    orb->y_speed = ORB_SPEED_RAND();
  }
  else if (orb->y >= (SSD1306_HEIGHT - ORB_RADIUS))
  {
    doCollisionSound();
    orb->y_direction = false;
    orb->y_speed = ORB_SPEED_RAND();
  }

  if ( (orb->y <= (player_1->y + PLATFORM_HEIGHT)) && (orb->y >= player_1->y) && (orb->x <= (player_1->x + PLATFORM_WIDTH + ORB_RADIUS)) )
  {
    doCollisionSound();
    orb->x_direction = true;
    orb->x_speed = ORB_SPEED_RAND();
  }
  else if ( (orb->y <= (player_2->y + PLATFORM_HEIGHT)) && (orb->y >= player_2->y) && (orb->x >= (player_2->x - ORB_RADIUS)) )
  {
    doCollisionSound();
    orb->x_direction = false;
    orb->x_speed = ORB_SPEED_RAND();
  }
  return 0;
}

void changePlayersPosIfButtonsPressed(Player * player_1, Player * player_2)
{
  if (player_1->y > 0 && HAL_GPIO_ReadPin(BUTTON_DOWN_PLAYER_1_PORT, 1 << BUTTON_DOWN_PLAYER_1_PIN) == GPIO_PIN_HIGH)
  {
    player_1->y -= PLAYER_SPEED;
  }

  if (player_1->y < (SSD1306_HEIGHT - PLATFORM_HEIGHT) && HAL_GPIO_ReadPin(BUTTON_UP_PLAYER_1_PORT, 1 << BUTTON_UP_PLAYER_1_PIN) == GPIO_PIN_HIGH)
  {
    player_1->y += PLAYER_SPEED;
  }

  if (player_2->y > 0 && HAL_GPIO_ReadPin(BUTTON_DOWN_PLAYER_2_PORT, 1 << BUTTON_DOWN_PLAYER_2_PIN) == GPIO_PIN_HIGH)
  {
    player_2->y -= PLAYER_SPEED;
  }

  if (player_2->y < (SSD1306_HEIGHT - PLATFORM_HEIGHT) && HAL_GPIO_ReadPin(BUTTON_UP_PLAYER_2_PORT, 1 << BUTTON_UP_PLAYER_2_PIN) == GPIO_PIN_HIGH)
  {
    player_2->y += PLAYER_SPEED;
  }
}

void changePlayerPosIfButtonPressedAndManageRobot(Player * player, Robot * robot)
{
  if (player->y > 0 && HAL_GPIO_ReadPin(BUTTON_DOWN_PLAYER_1_PORT, 1 << BUTTON_DOWN_PLAYER_1_PIN) == GPIO_PIN_HIGH)
  {
    player->y -= PLAYER_SPEED;
  }

  if (player->y < (SSD1306_HEIGHT - PLATFORM_HEIGHT) && HAL_GPIO_ReadPin(BUTTON_UP_PLAYER_1_PORT, 1 << BUTTON_UP_PLAYER_1_PIN) == GPIO_PIN_HIGH)
  {
    player->y += PLAYER_SPEED;
  }

  if (!robot->direction)
  {
    if (robot->coordinates.y > 0)
    {
      robot->coordinates.y -= PLAYER_SPEED;
    }
    else
    {
      robot->direction = true;
    }
  }
  else if (robot->direction)
  {
    if (robot->coordinates.y < (SSD1306_HEIGHT - PLATFORM_HEIGHT))
    {
      robot->coordinates.y += PLAYER_SPEED;
    }
    else
    {
      robot->direction = false;
    }
  }
}
