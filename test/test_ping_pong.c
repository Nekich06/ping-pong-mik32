#include "unity.h"

#define itoa(value, str, base) mock_itoa(value, str, base)

char * mock_itoa(int value, char* str, int base)
{
  return str;
}

#include "ping_pong.c"

#include "mock_scr1_timer_delay.h"
#include "mock_ping_pong_buttons.h"
#include "mock_ping_pong_sound.h"
#include "mock_mik32_hal_gpio.h"
#include "mock_ssd1306_gfx.h"
#include "mock_sound.h"
#include "mock_ssd1306_spi.h"

static Orb test_orb;
static Player test_player_1;
static Player test_player_2;
static Robot test_robot;

void setUp(void)
{
    gameplay.mode = PLAYER_VS_PLAYER;
    gameplay.max_score = 5;
    gameplay.is_game_over = false;
    gameplay.player_1_won = false;
    gameplay.counter.player_1_score = 0;
    gameplay.counter.player_2_score = 0;

    test_orb.x = 64;
    test_orb.y = 32;
    test_orb.x_speed = 1;
    test_orb.y_speed = 1;
    test_orb.x_direction = true;
    test_orb.y_direction = true;

    test_player_1.x = 0;
    test_player_1.y = 24;
    test_player_2.x = 124;
    test_player_2.y = 24;

    test_robot.coordinates.x = 124;
    test_robot.coordinates.y = 24;
    test_robot.direction = false;
}

void tearDown(void) {}

void test_incScorePlayer_1_increments_score_and_ends_game_when_max_reached(void)
{
    gameplay.counter.player_1_score = 4;
    gameplay.max_score = 5;

    incScorePlayer_1();

    TEST_ASSERT_EQUAL(5, gameplay.counter.player_1_score);
    TEST_ASSERT_TRUE(gameplay.is_game_over);
    TEST_ASSERT_TRUE(gameplay.player_1_won);
}

void test_incScorePlayer_1_increments_score_but_does_not_end_game_when_max_not_reached(void)
{
    gameplay.counter.player_1_score = 3;
    gameplay.max_score = 5;

    incScorePlayer_1();

    TEST_ASSERT_EQUAL(4, gameplay.counter.player_1_score);
    TEST_ASSERT_FALSE(gameplay.is_game_over);
    TEST_ASSERT_FALSE(gameplay.player_1_won);
}

void test_incScorePlayer_2_increments_score_and_ends_game_when_max_reached(void)
{
    gameplay.counter.player_2_score = 4;
    gameplay.max_score = 5;

    incScorePlayer_2();

    TEST_ASSERT_EQUAL(5, gameplay.counter.player_2_score);
    TEST_ASSERT_TRUE(gameplay.is_game_over);
    TEST_ASSERT_FALSE(gameplay.player_1_won);
}

void test_incScorePlayer_2_increments_score_but_does_not_end_game_when_max_not_reached(void)
{
    gameplay.counter.player_2_score = 2;
    gameplay.max_score = 5;

    incScorePlayer_2();

    TEST_ASSERT_EQUAL(3, gameplay.counter.player_2_score);
    TEST_ASSERT_FALSE(gameplay.is_game_over);
    TEST_ASSERT_FALSE(gameplay.player_1_won);
}

void test_resetCounter_resets_both_player_scores(void)
{
    gameplay.counter.player_1_score = 5;
    gameplay.counter.player_2_score = 3;

    resetCounter();

    TEST_ASSERT_EQUAL(0, gameplay.counter.player_1_score);
    TEST_ASSERT_EQUAL(0, gameplay.counter.player_2_score);
}

void test_changeOrbPosition_moves_orb_right_and_down(void)
{
    test_orb.x_direction = true;
    test_orb.y_direction = true;
    test_orb.x_speed = 2;
    test_orb.y_speed = 1;
    int8_t original_x = test_orb.x;
    int8_t original_y = test_orb.y;

    changeOrbPosition(&test_orb);

    TEST_ASSERT_EQUAL(original_x + 2, test_orb.x);
    TEST_ASSERT_EQUAL(original_y + 1, test_orb.y);
}

void test_changeOrbPosition_moves_orb_left_and_up(void)
{
    test_orb.x_direction = false;
    test_orb.y_direction = false;
    test_orb.x_speed = 3;
    test_orb.y_speed = 2;
    int8_t original_x = test_orb.x;
    int8_t original_y = test_orb.y;

    changeOrbPosition(&test_orb);

    TEST_ASSERT_EQUAL(original_x - 3, test_orb.x);
    TEST_ASSERT_EQUAL(original_y - 2, test_orb.y);
}

void test_changeOrbPosition_moves_orb_right_and_up(void)
{
    test_orb.x_direction = true;
    test_orb.y_direction = false;
    test_orb.x_speed = 1;
    test_orb.y_speed = 2;
    int8_t original_x = test_orb.x;
    int8_t original_y = test_orb.y;

    changeOrbPosition(&test_orb);

    TEST_ASSERT_EQUAL(original_x + 1, test_orb.x);
    TEST_ASSERT_EQUAL(original_y - 2, test_orb.y);
}

void test_changeOrbDirIfCollisions_returns_true_and_scores_for_player2_when_orb_hits_left_wall(void)
{
    test_orb.x = 0;
    test_orb.y = 32;
    gameplay.counter.player_2_score = 0;

    bool result = changeOrbDirIfCollisions(&test_orb, &test_player_1, &test_player_2);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL(1, gameplay.counter.player_2_score);
}

void test_changeOrbDirIfCollisions_returns_true_and_scores_for_player1_when_orb_hits_right_wall(void)
{
    test_orb.x = SSD1306_WIDTH - ORB_RADIUS;
    test_orb.y = 32;
    gameplay.counter.player_1_score = 0;

    bool result = changeOrbDirIfCollisions(&test_orb, &test_player_1, &test_player_2);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL(1, gameplay.counter.player_1_score);
}

void test_changeOrbDirIfCollisions_changes_y_direction_and_plays_sound_on_top_wall_collision(void)
{
    test_orb.x = 64;
    test_orb.y = 0;
    test_orb.y_direction = false;

    doCollisionSound_Expect();
    bool result = changeOrbDirIfCollisions(&test_orb, &test_player_1, &test_player_2);

    TEST_ASSERT_FALSE(result);
    TEST_ASSERT_TRUE(test_orb.y_direction);
}

void test_changeOrbDirIfCollisions_changes_y_direction_and_plays_sound_on_bottom_wall_collision(void)
{
    test_orb.x = 64;
    test_orb.y = SSD1306_HEIGHT - ORB_RADIUS;
    test_orb.y_direction = true;

    doCollisionSound_Expect();
    bool result = changeOrbDirIfCollisions(&test_orb, &test_player_1, &test_player_2);

    TEST_ASSERT_FALSE(result);
    TEST_ASSERT_FALSE(test_orb.y_direction);
}

void test_changeOrbDirIfCollisions_changes_x_direction_on_player1_collision(void)
{
    test_orb.x = test_player_1.x + PLATFORM_WIDTH;
    test_orb.y = test_player_1.y + (PLATFORM_HEIGHT / 2);
    test_orb.x_direction = false;

    doCollisionSound_Expect();
    bool result = changeOrbDirIfCollisions(&test_orb, &test_player_1, &test_player_2);

    TEST_ASSERT_FALSE(result);
    TEST_ASSERT_TRUE(test_orb.x_direction);
}

void test_changeOrbDirIfCollisions_changes_x_direction_on_player2_collision(void)
{
    test_orb.x = test_player_2.x - ORB_RADIUS;
    test_orb.y = test_player_2.y + (PLATFORM_HEIGHT / 2);
    test_orb.x_direction = true;

    doCollisionSound_Expect();
    bool result = changeOrbDirIfCollisions(&test_orb, &test_player_1, &test_player_2);

    TEST_ASSERT_FALSE(result);
    TEST_ASSERT_FALSE(test_orb.x_direction);
}

void test_changeOrbDirIfCollisions_no_collision_returns_false(void)
{
    test_orb.x = 64;
    test_orb.y = 32;

    bool result = changeOrbDirIfCollisions(&test_orb, &test_player_1, &test_player_2);

    TEST_ASSERT_FALSE(result);
}

void test_changePlayerPosIfButtonPressedAndManageRobot_moves_robot_down_when_direction_true_and_within_bounds(void)
{
    test_robot.direction = true;
    test_robot.coordinates.y = 30;

    HAL_GPIO_ReadPin_IgnoreAndReturn(GPIO_PIN_LOW);
    HAL_GPIO_ReadPin_IgnoreAndReturn(GPIO_PIN_LOW);
    changePlayerPosIfButtonPressedAndManageRobot(&test_player_1, &test_robot);

    TEST_ASSERT_EQUAL(32, test_robot.coordinates.y);
}

void test_changePlayerPosIfButtonPressedAndManageRobot_moves_robot_up_when_direction_false_and_within_bounds(void)
{
    test_robot.direction = false;
    test_robot.coordinates.y = 30;

    HAL_GPIO_ReadPin_IgnoreAndReturn(GPIO_PIN_LOW);
    HAL_GPIO_ReadPin_IgnoreAndReturn(GPIO_PIN_LOW);
    changePlayerPosIfButtonPressedAndManageRobot(&test_player_1, &test_robot);

    TEST_ASSERT_EQUAL(28, test_robot.coordinates.y);
}

void test_changePlayerPosIfButtonPressedAndManageRobot_robot_changes_direction_at_top_boundary(void)
{
    test_robot.direction = false;
    test_robot.coordinates.y = 0;

    HAL_GPIO_ReadPin_IgnoreAndReturn(GPIO_PIN_LOW);
    HAL_GPIO_ReadPin_IgnoreAndReturn(GPIO_PIN_LOW);
    changePlayerPosIfButtonPressedAndManageRobot(&test_player_1, &test_robot);

    TEST_ASSERT_TRUE(test_robot.direction);
}

void test_changePlayerPosIfButtonPressedAndManageRobot_robot_changes_direction_at_bottom_boundary(void)
{
    test_robot.direction = true;
    test_robot.coordinates.y = SSD1306_HEIGHT - PLATFORM_HEIGHT;

    HAL_GPIO_ReadPin_IgnoreAndReturn(GPIO_PIN_LOW);
    HAL_GPIO_ReadPin_IgnoreAndReturn(GPIO_PIN_LOW);
    changePlayerPosIfButtonPressedAndManageRobot(&test_player_1, &test_robot);

    TEST_ASSERT_FALSE(test_robot.direction);
}
