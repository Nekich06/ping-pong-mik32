#ifndef SPI_FUNCTIONS_H
#define SPI_FUNCTIONS_H

#include "mik32_hal_spi.h"
#include "mik32_hal_gpio.h"

void SPI_Init(void) __attribute__((section(".ram_text.spi_init")));
void SPI_Transfer(uint8_t * data, size_t size) __attribute__((section(".ram_text.spi_transfer")));
void SPI_Set_Reset_State(GPIO_PinState pin_state) __attribute__((section(".ram_text.spi_reset")));
void SPI_Set_CS_State(GPIO_PinState pin_state) __attribute__((section(".ram_text.spi_cs")));
void SPI_Set_DC_State(GPIO_PinState pin_state) __attribute__((section(".ram_text.spi_dc")));

#endif
