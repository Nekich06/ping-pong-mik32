    #include "ssd1306_spi.h"

#include <string.h>

#include "spi_functions.h"
#include "scr1_timer_delay.h"
#include "ssd1306_gpio_config.h"

#define SSD1306_SWAP(a, b) (((a) ^= (b)), ((b) ^= (a)), ((a) ^= (b)))
#define DISPLAY_BUF_SIZE (((SSD1306_WIDTH * SSD1306_HEIGHT) / 8))

uint8_t display_buffer[DISPLAY_BUF_SIZE];

bool SSD1306_Begin(uint8_t vcs, bool reset)
{
  SSD1306_ClearDisplay();
  SSD1306_GPIO_Pins_Init();
  SPI_Init();

  // SPI_Set_Reset_State(GPIO_PIN_HIGH);
  // SCR1_Timer_Delay(1000);
  // SPI_Set_Reset_State(GPIO_PIN_LOW);
  // SCR1_Timer_Delay(10000);
  // SPI_Set_Reset_State(GPIO_PIN_HIGH);

  uint8_t init_data[] =
  {
    SSD1306_DISPLAYOFF,
    SSD1306_SETDISPLAYCLOCKDIV,
    0x80,
    SSD1306_SETMULTIPLEX,
    SSD1306_HEIGHT - 1,
    SSD1306_SETDISPLAYOFFSET,
    0x00,
    SSD1306_SETSTARTLINE,
    SSD1306_CHARGEPUMP,
    (vcs == SSD1306_EXTERNALVCC) ? 0x10 : 0x14,
    SSD1306_MEMORYMODE,
    0x00,
    SSD1306_SEGREMAP,
    SSD1306_COMSCANDEC,
    SSD1306_SETCOMPINS,
    0x12,
    SSD1306_SETCONTRAST,
    (vcs == SSD1306_EXTERNALVCC) ? 0x9F : 0xFF,
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

  SSD1306_SendMultipleCommands(init_data, sizeof(init_data));

  SPI_Set_CS_State(GPIO_PIN_HIGH);

  return true;
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
    0x07,
    SSD1306_COLUMNADDR,
    0x00,
    SSD1306_WIDTH - 1
  };

  SPI_Set_CS_State(GPIO_PIN_LOW);

  SSD1306_SendMultipleCommands(data, sizeof(data));
  SSD1306_SendData(display_buffer, DISPLAY_BUF_SIZE);

  SPI_Set_CS_State(GPIO_PIN_HIGH);
}

void SSD1306_DrawPixel(int16_t x, int16_t y, uint16_t color)
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

void SSD1306_SendSingleCommand(uint8_t command)
{
  SPI_Set_DC_State(GPIO_PIN_LOW);
  SPI_Transfer(&command, 1);
}

void SSD1306_SendMultipleCommands(uint8_t * commands, uint8_t size)
{
  SPI_Set_DC_State(GPIO_PIN_LOW);
  SPI_Transfer(commands, size);
}

void SSD1306_SendData(uint8_t * data, uint16_t size)
{
  SPI_Set_DC_State(GPIO_PIN_HIGH);
  SPI_Transfer(data, size);
}
