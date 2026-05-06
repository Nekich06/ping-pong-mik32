#ifndef SSD1306_GPIO_CONFIG_H
#define SSD1306_GPIO_CONFIG_H

#include "mik32_memory_map.h"

#define D0_PIN    (2) // SCLK        -> D6 ACE-UNO
#define D1_PIN    (1) // MOSI        -> D5 ACE-UNO
#define RES_PIN   (5) // RESET       -> D0 ACE-UNO
#define DC_PIN    (6) // DC          -> D1 ACE-UNO
#define CS_PIN    (4) // CHIP-SELECT -> A2 ACE-UNO

#define D0_PORT   GPIO_0
#define D1_PORT   GPIO_0
#define RES_PORT  GPIO_0
#define DC_PORT   GPIO_0
#define CS_PORT   GPIO_0

void SSD1306_GPIO_Pins_Init();

#endif
