#ifndef BUTTONS_GPIO_CONFIG_H
#define BUTTONS_GPIO_CONFIG_H

#include "mik32_memory_map.h"

#define BUTTON_1_PIN   (0)  // D12 ACE-UNO
#define BUTTON_2_PIN   (1)  // D11 ACE-UNO
#define BUTTON_3_PIN   (2)  // D13 ACE-UNO
#define BUTTON_4_PIN   (12) // D18 ACE-UNO

#define BUTTON_1_PORT  GPIO_1
#define BUTTON_2_PORT  GPIO_1
#define BUTTON_3_PORT  GPIO_1
#define BUTTON_4_PORT  GPIO_1

void Buttons_GPIO_Pins_Init();

#endif
