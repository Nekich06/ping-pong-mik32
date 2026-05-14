#include "sound.h"

#include <stdbool.h>

#include "epic.h"
#include "timer32.h"
#include "riscv-irq.h"
#include "pad_config.h"
#include "power_manager.h"
#include "mik32_memory_map.h"

#define SYSTEM_FREQ_HZ  32000000UL
#define MEANDER_PERCENT 50UL

#define PWM_CH_4_PIN    (3)    // D10 ACE-UNO
#define PWM_PORT        GPIO_1

#define PWM_PERIOD_TICKS(freq) (SYSTEM_FREQ_HZ / (freq))
#define PWM_DUTY_CYCLE_TICKS(freq) ((PWM_PERIOD_TICKS(freq) / 100) * MEANDER_PERCENT)

static uint32_t collisionSoundFreqs[] =
{
  300, 285, 270, 255, 240, 225, 210, 195,
  180, 165, 150, 135, 120, 105, 90, 0, 120, 0
};

static uint32_t collisionSoundDurationsMs[] =
{
  3, 3, 3, 3, 3, 3, 3, 3, 3,
  3, 3, 3, 3, 3, 3, 10, 60, 0
};

static struct SoundManager
{
  uint32_t * freqs;
  uint32_t * durationsMs;
  uint8_t iteration;
  bool processing;
} sndManager;

static void Timer32_1_Init(void);
static void Timer32_2_PWM_Init(void);

static void EPIC_trap_handler();

void soundInit(void)
{
  Timer32_2_PWM_Init();
  Timer32_1_Init();

  PM->CLK_APB_M_SET = PM_CLOCK_APB_M_EPIC_M;
  EPIC->MASK_LEVEL_SET |= 1 << (EPIC_LINE_TIMER32_1_S);
  riscv_irq_set_handler(RISCV_IRQ_MEI, EPIC_trap_handler);
  riscv_irq_enable(RISCV_IRQ_MEI);
  riscv_irq_global_enable();
}

void tone(uint32_t freq)
{
  TIMER32_2->TOP = PWM_PERIOD_TICKS(freq);
  TIMER32_2->CHANNELS[3].OCR = PWM_DUTY_CYCLE_TICKS(freq);
  TIMER32_2->ENABLE = 1;
}

void toneMs(uint32_t freq, uint32_t duration)
{
  TIMER32_2->TOP = PWM_PERIOD_TICKS(freq);
  TIMER32_2->CHANNELS[3].OCR = PWM_DUTY_CYCLE_TICKS(freq);
  TIMER32_2->ENABLE = 1;

  TIMER32_1->TOP = (SYSTEM_FREQ_HZ * (duration / 1000.0));
  TIMER32_1->ENABLE = 1;
  TIMER32_1->INT_MASK = TIMER32_INT_OVERFLOW_M;
}

void noTone(void)
{
  TIMER32_2->TOP = 0xFFFFFFFF;
  TIMER32_2->CHANNELS[3].OCR = 0;
  TIMER32_2->ENABLE = 0;
}

void doCollisionSound()
{
  sndManager.freqs = collisionSoundFreqs;
  sndManager.durationsMs = collisionSoundDurationsMs;
  sndManager.iteration = 0;

  if (!sndManager.processing)
  {
    toneMs(sndManager.freqs[sndManager.iteration],
       sndManager.durationsMs[sndManager.iteration]);
    ++sndManager.iteration;
    sndManager.processing = true;
  }
}

void Timer32_1_Init(void)
{
  PM->CLK_APB_P_SET = PM_CLOCK_APB_P_TIMER32_1_M;
  TIMER32_1->ENABLE = 0;
  TIMER32_1->PRESCALER = 0;
  TIMER32_1->CONTROL =
    TIMER32_CONTROL_MODE_UP_M | TIMER32_CONTROL_CLOCK_PRESCALER_M;
  TIMER32_1->INT_MASK = 0;
  TIMER32_1->INT_CLEAR = 0xFFFFFFFF;
}

void Timer32_2_PWM_Init(void)
{
  PAD_CONFIG->PORT_1_CFG |= 2 << (PWM_CH_4_PIN * 2);
  PM->CLK_APB_P_SET = PM_CLOCK_APB_P_TIMER32_2_M;
  TIMER32_2->ENABLE = 0;
  TIMER32_2->PRESCALER = 0;
  TIMER32_2->CONTROL =
    TIMER32_CONTROL_MODE_UP_M | TIMER32_CONTROL_CLOCK_PRESCALER_M;
  TIMER32_2->INT_MASK = 0;
  TIMER32_2->INT_CLEAR = 0xFFFFFFFF;
  TIMER32_2->CHANNELS[3].CNTRL =
    TIMER32_CH_CNTRL_MODE_PWM_M | TIMER32_CH_CNTRL_ENABLE_M;
  TIMER32_2->ENABLE = 1;
}

void EPIC_trap_handler()
{
  if (TIMER32_1->INT_MASK & TIMER32_INT_OVERFLOW_M)
  {
    if (sndManager.durationsMs[sndManager.iteration])
    {
      toneMs(sndManager.freqs[sndManager.iteration],
           sndManager.durationsMs[sndManager.iteration]);
      ++sndManager.iteration;
    }
    else
    {
      noTone();
      TIMER32_1->ENABLE = 0;
      sndManager.processing = false;
    }

    TIMER32_1->INT_CLEAR = TIMER32_INT_OVERFLOW_M;
    EPIC->CLEAR |= EPIC_LINE_TIMER32_1_S;
  }
}
