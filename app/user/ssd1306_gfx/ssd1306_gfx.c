#include "ssd1306_gfx.h"
#include "font.h"

static struct TextCursor
{
  int16_t x;
  int16_t y;
  uint8_t size_x;
  uint8_t size_y;
  uint16_t color;
  uint16_t bg;
} cursor;

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

void SSD1306_DrawChar(int16_t x, int16_t y, unsigned char c,
                      uint16_t color, uint16_t bg, uint8_t size_x,
                      uint8_t size_y)
{
  if ((x >= SSD1306_WIDTH) ||
      (y >= SSD1306_HEIGHT) ||
      ((x + 6 * size_x - 1) < 0) ||
      ((y + 8 * size_y - 1) < 0))
    return;

  for (int8_t i = 0; i < 5; i++)
  {
    uint8_t line = font[c * 5 + i];
    for (int8_t j = 0; j < 8; j++, line >>= 1)
    {
      if (line & 1) {
        if (size_x == 1 && size_y == 1)
          SSD1306_DrawPixelInternal(x + i, y + j, color);
        else
          SSD1306_FillRect(x + i * size_x, y + j * size_y, size_x, size_y,
                        color);
      } else if (bg != color) {
        if (size_x == 1 && size_y == 1)
          SSD1306_DrawPixelInternal(x + i, y + j, bg);
        else
          SSD1306_FillRect(x + i * size_x, y + j * size_y, size_x, size_y, bg);
      }
    }
  }
  if (bg != color)
  {
    if (size_x == 1 && size_y == 1)
      SSD1306_DrawFastVLineInternal(x + 5, y, 8, bg);
    else
      SSD1306_FillRect(x + 5 * size_x, y, size_x, 8 * size_y, bg);
  }
}

void SSD1306_DefaultCursorInit(void)
{
  cursor.x = 0;
  cursor.y = 0;
  cursor.size_x = 1;
  cursor.size_y = 1;
  cursor.color = SSD1306_WHITE;
  cursor.bg = SSD1306_BLACK;
}

void SSD1306_SetCursor(int16_t x, int16_t y)
{
  cursor.x = x;
  cursor.y = y;
}

void SSD1306_SetTextSize(uint8_t size)
{
  cursor.size_x = size;
  cursor.size_y = size;
}

void SSD1306_SetTextColor(uint16_t color)
{
  cursor.color = color;
}

void SSD1306_SetTextBackgroundColor(uint16_t bg_color)
{
  cursor.bg = bg_color;
}

void SSD1306_WriteText(char * text)
{
  if (text)
  {
    size_t i = 0;
    while (text[i] != '\0')
    {
      unsigned char c = text[i];
      if (c == '\n')
      {
        cursor.x = 0;
        cursor.y += cursor.size_y * 8;
      }
      else if (c != '\r')
      {
        if ((cursor.x + cursor.size_x * 6) > SSD1306_WIDTH)
        {
          cursor.x = 0;
          cursor.y += cursor.size_y * 8;
        }
        SSD1306_DrawChar(cursor.x, cursor.y, c, cursor.color, cursor.bg, cursor.size_x, cursor.size_y);
        cursor.x += cursor.size_x * 6;
      }
      ++i;
    }
  }
}
