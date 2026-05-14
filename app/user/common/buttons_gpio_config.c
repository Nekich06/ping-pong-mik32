#include "buttons_gpio_config.h"

#include "mik32_hal_gpio.h"
#include "pad_config.h"
#include "gpio.h"

void Buttons_GPIO_Pins_Init()
{
  PAD_CONFIG->PORT_1_CFG |= 0 << (BUTTON_1_PIN * 2);
  PAD_CONFIG->PORT_1_DS |= 0 << (BUTTON_1_PIN * 2);
  PAD_CONFIG->PORT_1_PUPD |= 0 << (BUTTON_1_PIN * 2);

  PAD_CONFIG->PORT_1_CFG |= 0 << (BUTTON_2_PIN * 2);
  PAD_CONFIG->PORT_1_DS |= 0 << (BUTTON_2_PIN * 2);
  PAD_CONFIG->PORT_1_PUPD |= 0 << (BUTTON_2_PIN * 2);

  PAD_CONFIG->PORT_1_CFG |= 0 << (BUTTON_3_PIN * 2);
  PAD_CONFIG->PORT_1_DS |= 0 << (BUTTON_3_PIN * 2);
  PAD_CONFIG->PORT_1_PUPD |= 0 << (BUTTON_3_PIN * 2);

  PAD_CONFIG->PORT_1_CFG |= 0 << (BUTTON_4_PIN * 2);
  PAD_CONFIG->PORT_1_DS |= 0 << (BUTTON_4_PIN * 2);
  PAD_CONFIG->PORT_1_PUPD |= 0 << (BUTTON_4_PIN * 2);

  GPIO_1->DIRECTION_OUT = 1 << BUTTON_1_PIN | 1 << BUTTON_2_PIN |
                          1 << BUTTON_3_PIN | 1 << BUTTON_4_PIN ;
}
