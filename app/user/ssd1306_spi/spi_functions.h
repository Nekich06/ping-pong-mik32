#ifndef SPI_FUNCTIONS_H
#define SPI_FUNCTIONS_H

#include "mik32_hal_spi.h"
#include "mik32_hal_gpio.h"

void SPI_Init(void);
void SPI_Transfer(uint8_t * data, size_t size) __attribute__((section(".ram_text")));
void SPI_Set_Reset_State(GPIO_PinState pin_state);
void SPI_Set_CS_State(GPIO_PinState pin_state);
void SPI_Set_DC_State(GPIO_PinState pin_state);

#endif
