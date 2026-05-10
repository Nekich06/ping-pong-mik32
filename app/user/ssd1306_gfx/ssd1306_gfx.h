#ifndef SSD1306_GFX_H
#define SSD1306_GFX_H

#include "ssd1306_spi.h"

void SSD1306_FillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void SSD1306_FillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);

#endif
