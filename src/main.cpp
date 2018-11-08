#include "mbed.h"
#include "encoder.hpp"

PwmOut led(LED3);
DigitalOut led2(LED2);
Serial bt(PC_12, PD_2);
Timer t0;

PwmOut pwm_r(PF_9);

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
