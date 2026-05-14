#include "ssd1306_spi.h"

#include <string.h>

#include "spi_functions.h"
#include "scr1_timer_delay.h"
#include "ssd1306_gpio_config.h"

#define SSD1306_SWAP(a, b) (((a) ^= (b)), ((b) ^= (a)), ((a) ^= (b)))
#define DISPLAY_BUF_SIZE ((SSD1306_WIDTH * ((SSD1306_HEIGHT + 7) / 8)))

static uint8_t display_buffer[DISPLAY_BUF_SIZE];

static void SSD1306_SendCommands(uint8_t * commands, uint8_t size) __attribute__((section(".ram_text")));
static void SSD1306_SendData(uint8_t * data, uint16_t size) __attribute__((section(".ram_text")));

void SSD1306_Begin(uint8_t vcs, bool reset)
{
  SSD1306_ClearDisplay();
  SSD1306_GPIO_Pins_Init();
  SPI_Init();

  if (reset)
  {
    SPI_Set_Reset_State(GPIO_PIN_HIGH);
    SCR1_Timer_Delay(1000);
    SPI_Set_Reset_State(GPIO_PIN_LOW);
    SCR1_Timer_Delay(10000);
    SPI_Set_Reset_State(GPIO_PIN_HIGH);
  }

  uint8_t init_data[] =
  {
    SSD1306_DISPLAYOFF,
    SSD1306_SETDISPLAYCLOCKDIV,
    0x80,
    SSD1306_SETMULTIPLEX,
    SSD1306_HEIGHT - 1,
    SSD1306_SETDISPLAYOFFSET,
    0x0,
    SSD1306_SETSTARTLINE | 0x0,
    SSD1306_CHARGEPUMP,
    (vcs == SSD1306_EXTERNALVCC) ? 0x10 : 0x14,
    SSD1306_MEMORYMODE,
    0x00,
    SSD1306_SEGREMAP | 0x1,
    SSD1306_COMSCANDEC,
    SSD1306_SETCOMPINS,
    0x12,
    SSD1306_SETCONTRAST,
    (vcs == SSD1306_EXTERNALVCC) ? 0x9F : 0xCF,
    SSD1306_SETPRECHARGE,
    (vcs == SSD1306_EXTERNALVCC) ? 0x22 : 0xF1,
    SSD1306_SETVCOMDETECT,
    0x40,
    SSD1306_DISPLAYALLON_RESUME,
    SSD1306_NORMALDISPLAY,
    SSD1306_DEACTIVATE_SCROLL,
    SSD1306_DISPLAYON
  };

  SPI_Set_CS_State(GPIO_PIN_LOW);
  SSD1306_SendCommands(init_data, sizeof(init_data));
  SPI_Set_CS_State(GPIO_PIN_HIGH);
}

void SSD1306_ClearDisplay(void)
{
  memset(display_buffer, 0, DISPLAY_BUF_SIZE);
}

void SSD1306_Display(void)
{
  SPI_Set_CS_State(GPIO_PIN_LOW);

  static uint8_t data[] =
  {
    SSD1306_PAGEADDR,
    0x00,
    0xFF,
    SSD1306_COLUMNADDR,
    0x00,
    SSD1306_WIDTH - 1
  };

  SPI_Set_CS_State(GPIO_PIN_LOW);
  SSD1306_SendCommands(data, sizeof(data));
  SPI_Set_CS_State(GPIO_PIN_HIGH);
  SCR1_Timer_Delay(1000);

  SPI_Set_CS_State(GPIO_PIN_LOW);
  SSD1306_SendData(display_buffer, DISPLAY_BUF_SIZE);
  SPI_Set_CS_State(GPIO_PIN_HIGH);
}

void SSD1306_DrawPixelInternal(int16_t x, int16_t y, uint16_t color)
{
  if ((x >= 0) && (x < SSD1306_WIDTH) && (y >= 0) && (y < SSD1306_HEIGHT))
  {
    switch (color)
    {
      case SSD1306_WHITE:
        display_buffer[x + (y / 8) * SSD1306_WIDTH] |= (1 << (y & 7));
        break;
      case SSD1306_BLACK:
        display_buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y & 7));
        break;
      case SSD1306_INVERSE:
        display_buffer[x + (y / 8) * SSD1306_WIDTH] ^= (1 << (y & 7));
        break;
    }
  }
}

void SSD1306_DrawFastHLineInternal(int16_t x, int16_t y, int16_t w, uint16_t color)
{
  if ((y >= 0) && (y < SSD1306_HEIGHT))
  {
    if (x < 0)
    {
      w += x;
      x  = 0;
    }
    if ((x + w) > SSD1306_WIDTH)
    {
      w = (SSD1306_WIDTH - x);
    }
    if(w > 0)
    {
      uint8_t * pBuf = &display_buffer[(y / 8) * SSD1306_WIDTH + x];
      uint8_t mask = 1 << (y & 7);
      switch(color)
      {
        case SSD1306_WHITE:
          while (w--)
          {
            *pBuf++ |= mask;
          }
          break;
        case SSD1306_BLACK:
          mask = ~mask;
          while (w--)
          {
            *pBuf++ &= mask;
          }
          break;
        case SSD1306_INVERSE:
          while (w--)
          {
            *pBuf++ ^= mask;
          }
          break;
      }
    }
  }
}

void SSD1306_DrawFastVLineInternal(int16_t x, int16_t y, int16_t h, uint16_t color)
{
  int16_t __y = y;
  int16_t __h = h;
  if((x >= 0) && (x < SSD1306_WIDTH))
  {
    if(__y < 0)
    {
      __h += __y;
      __y = 0;
    }
    if((__y + __h) > SSD1306_HEIGHT)
    {
      __h = (SSD1306_HEIGHT - __y);
    }
    if(__h > 0)
    {
      uint8_t  y = __y, h = __h;
      uint8_t *pBuf = &display_buffer[(y / 8) * SSD1306_WIDTH + x];
      uint8_t mod = (y & 7);

      if(mod) {

        mod = 8 - mod;
        static const uint8_t premask[8] =
        {
          0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE
        };

        uint8_t mask = premask[mod];
        if(h < mod) mask &= (0XFF >> (mod - h));

        switch(color)
        {
          case SSD1306_WHITE:
            *pBuf |=  mask;
            break;
          case SSD1306_BLACK:
            *pBuf &= ~mask;
            break;
          case SSD1306_INVERSE:
            *pBuf ^=  mask;
            break;
        }
        pBuf += SSD1306_WIDTH;
      }

      if (h >= mod)
      {
        h -= mod;
        if(h >= 8)
        {
          if(color == SSD1306_INVERSE)
          {
            do
            {
              *pBuf ^= 0xFF;
              pBuf  += SSD1306_WIDTH;
              h     -= 8;
            }
            while(h >= 8);
          }
          else
          {
            uint8_t val = (color != SSD1306_BLACK) ? 255 : 0;
            do
            {
              *pBuf = val;
              pBuf += SSD1306_WIDTH;
              h    -= 8;
            }
            while(h >= 8);
          }
        }

        if (h)
        {
          mod = h & 7;
          static const uint8_t postmask[8] =
          {
            0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F
          };
          uint8_t mask = postmask[mod];
          switch(color) {
           case SSD1306_WHITE:   *pBuf |=  mask; break;
           case SSD1306_BLACK:   *pBuf &= ~mask; break;
           case SSD1306_INVERSE: *pBuf ^=  mask; break;
          }
        }
      }
    }
  }
}

void SSD1306_SendCommands(uint8_t * commands, uint8_t size)
{
  SPI_Set_DC_State(GPIO_PIN_LOW);
  SPI_Transfer(commands, size);
}

void SSD1306_SendData(uint8_t * data, uint16_t size)
{
  SPI_Set_DC_State(GPIO_PIN_HIGH);
  SPI_Transfer(data, size);
}
