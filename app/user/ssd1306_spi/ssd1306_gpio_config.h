#ifndef SSD1306_GPIO_CONFIG_H
#define SSD1306_GPIO_CONFIG_H

#include "mik32_memory_map.h"

#define D0_PIN    (2) // SCLK
#define D1_PIN    (1) // MOSI
#define RES_PIN   (9) // RESET
#define DC_PIN    (10) // DC
#define CS_PIN    (8) // CHIP-SELECT

#define D0_PORT   GPIO_0
#define D1_PORT   GPIO_0
#define RES_PORT  GPIO_0
#define DC_PORT   GPIO_0
#define CS_PORT   GPIO_0

void SSD1306_GPIO_Pins_Init();

#endif
