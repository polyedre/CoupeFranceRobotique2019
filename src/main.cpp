#include "mbed.h"
#include "encoder.hpp"
#include "position.hpp"

PwmOut led(LED3);
DigitalOut led2(LED2);
Serial bt(PC_12, PD_2);
Serial usb(USBTX, USBRX);

Timer t0;

PwmOut pwm_r(PF_9);

Encoder encod_l(TIM4);
Encoder encod_r(TIM3);

Position pos(&encod_l, &encod_r);

int main()
{
  bt.baud(9600);
  usb.baud(9600);

  // while (1) {
  //   printf("%x", usb.getc());
  // }
  printf("\r\nLancement du dev.\r\n");

  pwm_r.period(0.000033f);

  pwm_r.write(0.2f);

  int compteur = 0;

  while (1) {


    pos.update();
    if (compteur % 100 == 0) printf("x : %fm ; y : %fm ; theta : %f ; el : %d ; er : %d                           \r", pos.get_x(), pos.get_y(), pos.get_theta(), encod_l.get(), encod_r.get());

    if (usb.readable()) {
      if (usb.getc() == 'a') {
        printf("Pos : x = %f ; y = %f ; theta = %f\r\n", pos.get_x(), pos.get_y(), pos.get_theta());
        led2 = 1;
      }
    }
  }

  return 0;
}
