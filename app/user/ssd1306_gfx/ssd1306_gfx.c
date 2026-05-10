#include "ssd1306_gfx.h"

static void SSD1306_FillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t corners, int16_t delta, uint16_t color);

void SSD1306_FillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
  for (int16_t i = x; i < x + w; i++)
  {
    SSD1306_DrawFastVLineInternal(i, y, h, color);
  }
}

void SSD1306_FillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
  SSD1306_DrawFastVLineInternal(x0, y0 - r, 2 * r + 1, color);
  SSD1306_FillCircleHelper(x0, y0, r, 3, 0, color);
}

void SSD1306_FillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t corners, int16_t delta, uint16_t color)
{
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;
  int16_t px = x;
  int16_t py = y;

  delta++;

  while (x < y)
  {
    if (f >= 0)
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
    if (x < (y + 1))
    {
      if (corners & 1)
        SSD1306_DrawFastVLineInternal(x0 + x, y0 - y, 2 * y + delta, color);
      if (corners & 2)
        SSD1306_DrawFastVLineInternal(x0 - x, y0 - y, 2 * y + delta, color);
    }
    if (y != py)
    {
      if (corners & 1)
        SSD1306_DrawFastVLineInternal(x0 + py, y0 - px, 2 * px + delta, color);
      if (corners & 2)
        SSD1306_DrawFastVLineInternal(x0 - py, y0 - px, 2 * px + delta, color);
      py = y;
    }
    px = x;
  }
}


