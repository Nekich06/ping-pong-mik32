#include "mik32_hal_usart.h"
#include "scr1_timer_delay.h"
#include "ssd1306_spi.h"
#include "ssd1306_gfx.h"

static void USART_Init();
static void SystemClock_Config(void);

USART_HandleTypeDef husart0;

int main()
{
  SystemClock_Config();
  USART_Init();

  SCR1_Timer_Delay(3000000);

  SSD1306_Begin(SSD1306_SWITCHCAPVCC, true);
  SSD1306_ClearDisplay();
  SSD1306_Display();

  SSD1306_DefaultCursorInit();
  while (1)
  {
    SSD1306_SetCursor(12, 10);
    SSD1306_SetTextSize(2);
    SSD1306_WriteText("Hello, world!", 14);
    SSD1306_Display();
    SCR1_Timer_Delay(3000000);
  }
}

void SystemClock_Config(void)
{
  PCC_InitTypeDef PCC_OscInit = {0};

  PCC_OscInit.OscillatorEnable = PCC_OSCILLATORTYPE_ALL;
  PCC_OscInit.FreqMon.OscillatorSystem = PCC_OSCILLATORTYPE_OSC32M;
  PCC_OscInit.FreqMon.ForceOscSys = PCC_FORCE_OSC_SYS_UNFIXED;
  PCC_OscInit.FreqMon.Force32KClk = PCC_FREQ_MONITOR_SOURCE_OSC32K;
  PCC_OscInit.AHBDivider = 0;
  PCC_OscInit.APBMDivider = 0;
  PCC_OscInit.APBPDivider = 0;
  PCC_OscInit.HSI32MCalibrationValue = 128;
  PCC_OscInit.LSI32KCalibrationValue = 8;
  PCC_OscInit.RTCClockSelection = PCC_RTC_CLOCK_SOURCE_AUTO;
  PCC_OscInit.RTCClockCPUSelection = PCC_CPU_RTC_CLOCK_SOURCE_OSC32K;
  HAL_PCC_Config(&PCC_OscInit);
}

void USART_Init()
{
  husart0.Instance = UART_0;
  husart0.transmitting = Enable;
  husart0.receiving = Enable;
  husart0.frame = Frame_8bit;
  husart0.parity_bit = Disable;
  husart0.parity_bit_inversion = Disable;
  husart0.bit_direction = LSB_First;
  husart0.data_inversion = Disable;
  husart0.tx_inversion = Disable;
  husart0.rx_inversion = Disable;
  husart0.swap = Disable;
  husart0.lbm = Disable;
  husart0.stop_bit = StopBit_1;
  husart0.mode = Asynchronous_Mode;
  husart0.xck_mode = XCK_Mode3;
  husart0.last_byte_clock = Disable;
  husart0.overwrite = Disable;
  husart0.rts_mode = AlwaysEnable_mode;
  husart0.dma_tx_request = Disable;
  husart0.dma_rx_request = Disable;
  husart0.channel_mode = Duplex_Mode;
  husart0.tx_break_mode = Disable;
  husart0.Interrupt.ctsie = Disable;
  husart0.Interrupt.eie = Disable;
  husart0.Interrupt.idleie = Disable;
  husart0.Interrupt.lbdie = Disable;
  husart0.Interrupt.peie = Disable;
  husart0.Interrupt.rxneie = Disable;
  husart0.Interrupt.tcie = Disable;
  husart0.Interrupt.txeie = Disable;
  husart0.Modem.rts = Disable; //out
  husart0.Modem.cts = Disable; //in
  husart0.Modem.dtr = Disable; //out
  husart0.Modem.dcd = Disable; //in
  husart0.Modem.dsr = Disable; //in
  husart0.Modem.ri = Disable;  //in
  husart0.Modem.ddis = Disable;//out
  husart0.baudrate = 115200;
  HAL_USART_Init(&husart0);
}

