#include "mbed.h"

PwmOut led(LED3);
DigitalOut led2(LED2);
Serial bt(PC_12, PD_2);
Timer t0;

PwmOut pwm_r(PF_9);

void TIM3_EncoderInit() {
  __GPIOA_CLK_ENABLE();
  __TIM3_CLK_ENABLE();
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  TIM3->SMCR = 3;          // Encoder mode 3
  TIM3->CCER = 0;          // rising edge polarity
  TIM3->CCER |= (1 << 1);
  TIM3->ARR = 0xFFFF;      // count from 0-ARR or ARR-0
  TIM3->CCMR1 = 0xC1C1;    // f_DTS/16, N=8, IC1->TI1, IC2->TI2
  TIM3->EGR = 1;           // Generate an update event
  TIM3->CR1 = 1;           // Enable the counter
  TIM3->CNT = (0xFFFF / 2);           // Initialize counter
}

void TIM4_EncoderInit() {
  GPIO_InitTypeDef GPIO_InitStruct;

  __GPIOD_CLK_ENABLE();
  __TIM4_CLK_ENABLE();
  GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  TIM4->SMCR = 3;          // Encoder mode 3
  TIM4->CCER = 0;          // rising edge polarity
  TIM4->ARR = 0xFFFF;      // count from 0-ARR or ARR-0
  TIM4->CCMR1 = 0xC1C1;    // f_DTS/16, N=8, IC1->TI1, IC2->TI2
  TIM4->EGR = 1;           // Generate an update event
  TIM4->CR1 = 1;           // Enable the counter
  TIM4->CNT = (0xFFFF / 2);          // Initialize counter
}

int main()
{
  bt.baud(9600);

  TIM3_EncoderInit();
  TIM4_EncoderInit();

  pwm_r.period(0.000033f);

  pwm_r.write(0.2f);
  led.write(0.01f);

  while (1) {

    if (bt.readable()) {
      if (bt.getc() == 'A') {
        int droite = TIM4->CNT;
        bt.printf("%d ", droite);
        led2 = 1;
      }
      if (bt.getc() == 'R') {

      }
    }

  }

  return 0;
}
