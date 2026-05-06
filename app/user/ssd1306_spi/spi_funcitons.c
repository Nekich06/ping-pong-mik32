#include "spi_functions.h"

#include "ssd1306_gpio_config.h"
#include "mik32_memory_map.h"

static SPI_HandleTypeDef hspi;

void SPI_Init(void)
{
  SPI_InitTypeDef spi_init =
  {
      .SPI_Mode    = HAL_SPI_MODE_MASTER,
      .BaudRateDiv = SPI_BAUDRATE_DIV64,
      .ManualCS    = SPI_MANUALCS_OFF,
      .CLKPhase    = SPI_PHASE_ON,
      .CLKPolarity = SPI_POLARITY_HIGH,
      .Decoder     = SPI_DECODER_NONE,
      .ChipSelect  = SPI_CS_0
  };

  hspi.Instance = SPI_0;
  hspi.Init = spi_init;

  if (HAL_SPI_Init(&hspi) == HAL_OK)
  {
    HAL_SPI_Enable(&hspi);
  }
}

void SPI_Transfer(uint8_t * data, size_t size)
{
  uint8_t recieve_buf[size];
  HAL_SPI_Exchange(&hspi, data, recieve_buf, size, 0xFFFFFFFF);
}

void SPI_Set_Reset_State(GPIO_PinState pin_state)
{
  HAL_GPIO_WritePin(RES_PORT, 1 << RES_PIN, pin_state);
}

void SPI_Set_CS_State(GPIO_PinState pin_state)
{
  HAL_GPIO_WritePin(CS_PORT, 1 << CS_PIN, pin_state);
}

void SPI_Set_DC_State(GPIO_PinState pin_state)
{
  HAL_GPIO_WritePin(DC_PORT, 1 << DC_PIN, pin_state);
}
