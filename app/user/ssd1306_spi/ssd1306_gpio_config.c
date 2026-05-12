#include "ssd1306_gpio_config.h"

#include "mik32_hal_gpio.h"
#include "power_manager.h"
#include "pad_config.h"
#include "gpio.h"

void SSD1306_GPIO_Pins_Init()
{
  PM->CLK_APB_P_SET = PM_CLOCK_APB_P_GPIO_0_M;
  PM->CLK_APB_P_SET = PM_CLOCK_APB_P_GPIO_1_M;
  PM->CLK_APB_P_SET = PM_CLOCK_APB_P_GPIO_2_M;
  PM->CLK_APB_P_SET = PM_CLOCK_APB_P_GPIO_IRQ_M;

  PAD_CONFIG->PORT_0_CFG |= 0b01 << (D0_PIN * 2);
  PAD_CONFIG->PORT_0_DS |= 0 << (D0_PIN * 2);
  PAD_CONFIG->PORT_0_PUPD |= 0 << (D0_PIN * 2);

  PAD_CONFIG->PORT_0_CFG |= 0b01 << (D1_PIN * 2);
  PAD_CONFIG->PORT_0_DS |= 0 << (D1_PIN * 2);
  PAD_CONFIG->PORT_0_PUPD |= 0 << (D1_PIN * 2);

  PAD_CONFIG->PORT_0_CFG |= 0 << (RES_PIN * 2);
  PAD_CONFIG->PORT_0_DS |= 0 << (RES_PIN * 2);
  PAD_CONFIG->PORT_0_PUPD |= 0 << (RES_PIN * 2);

  PAD_CONFIG->PORT_0_CFG |= 0 << (DC_PIN * 2);
  PAD_CONFIG->PORT_0_DS |= 0 << (DC_PIN * 2);
  PAD_CONFIG->PORT_0_PUPD |= 0 << (DC_PIN * 2);

  PAD_CONFIG->PORT_0_CFG |= 0 << (CS_PIN * 2);
  PAD_CONFIG->PORT_0_DS |= 0 << (CS_PIN * 2);
  PAD_CONFIG->PORT_0_PUPD |= 0 << (CS_PIN * 2);

  GPIO_0->DIRECTION_OUT = 1 << RES_PIN | 1 << DC_PIN | 1 << CS_PIN;
  HAL_GPIO_WritePin(CS_PORT, 1 << CS_PIN, GPIO_PIN_HIGH);
}
