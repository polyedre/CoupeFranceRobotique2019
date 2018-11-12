#include "mbed.h"
#include "encoder.hpp"
#include "position.hpp"

PwmOut led(LED3);
DigitalOut led2(LED2);
Serial bt(PC_12, PD_2);
Timer t0;

PwmOut pwm_r(PF_9);

Encoder encod_l(TIM4);
Encoder encod_r(TIM3);

Position pos(&encod_l, &encod_r);

int main()
{
  bt.baud(9600);

  short x;
  short y;

  pwm_r.period(0.000033f);

  pwm_r.write(0.2f);

  while (1) {


    if (x != 0 || y != 0) led2 = 1;

    // pos.update();

    if (bt.readable()) {
      if (bt.getc() == 'A') {
        x = encod_l.get();
        y = encod_r.get();
        bt.printf("x : %d - y : %d\n", encod_l.get(), encod_l.lastValue);
        bt.printf("Valeur : %f -%f\n", encod_l.diff(), encod_r.diff());
        //  int droite = TIM4->CNT;
        // bt.printf("%d ", droite);
        led2 = 1;
      }
    }

  }

  return 0;
}
