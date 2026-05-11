#ifndef SSD1306_GFX_H
#define SSD1306_GFX_H

#include <stddef.h>

#include "ssd1306_spi.h"

void SSD1306_FillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) __attribute__((section(".ram_text")));
void SSD1306_FillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) __attribute__((section(".ram_text")));
void SSD1306_DrawChar(int16_t x, int16_t y, unsigned char c,
                      uint16_t color, uint16_t bg, uint8_t size_x,
                      uint8_t size_y) __attribute__((section(".ram_text")));

void SSD1306_DefaultCursorInit(void) __attribute__((section(".ram_text")));
void SSD1306_SetCursor(int16_t x, int16_t y) __attribute__((section(".ram_text")));
void SSD1306_SetTextSize(uint8_t size) __attribute__((section(".ram_text")));
void SSD1306_SetTextColor(uint16_t color) __attribute__((section(".ram_text")));
void SSD1306_SetTextBackgroundColor(uint16_t bg_color) __attribute__((section(".ram_text")));
void SSD1306_WriteText(char * text) __attribute__((section(".ram_text")));

#endif
