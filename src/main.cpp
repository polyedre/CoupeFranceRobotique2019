#include "mbed.h"
#include "encoder.hpp"
#include "navigation/navigateur.hpp"
#include "navigation/vecteur2D.hpp"
#include "position.hpp"
#include "pid.hpp"
#include <math.h>

DigitalOut led(LED3);
Serial usb(USBTX, USBRX);

Encoder enc_l(TIM4); // PD12 - PD13
Encoder enc_r(TIM3); // PC6  - PC7 ou PA6 - PA7 ou PB4 - PB5

PwmOut motor_l(PB_13); // TODO changer les noms des pins
PwmOut motor_r(PB_15);

DigitalOut direction_l(PG_14);
DigitalOut direction_r(PF_14);
DigitalOut beak_l(PG_9);
DigitalOut breal_r(PE_13);

Position pos(&enc_l, &enc_r);
Navigateur nav(&pos, &motor_l, &motor_r, &direction_l, &direction_r);

int main()
{
  // bt.baud(9600); // Bluetooth
  usb.baud(115200); // USB

  usb.printf("\r\nInitialisation du programme.\r\n");

  // (&motor_l)->write(0.5f);
  // wait(1);
  // motor_l.write(0.0f);
  // motor_r.write(0.5f);
  // wait(1);
  // motor_r.write(0.0f);

  wait(1);

  // for (float i = 0; i < 0.4; i += 0.001) {
  //   motor_r.write(i);
  //   printf("\r%f", i);
  //   wait(0.1);
  // }

  usb.printf("\r\nDébut rotation directe.\r\n");

  nav.rotate_by(1.57f);

  nav.print_pos();

  usb.printf("\r\nRotation Terminée.\r\n");
  usb.printf("\r\nDébut rotation indirecte.\r\n");

  motor_l.write(0);
  motor_r.write(0);

  wait(1);

  nav.rotate_by(-1.57f);

  nav.print_pos();

  usb.printf("\r\nRotation Terminée.\r\n");
  usb.printf("\r\nAction Terminée.\r\n");

  motor_l.write(0);
  motor_r.write(0);

  // Vecteur2D destination(1,1);

  // nav.set_destination(&destination);


  // int compteur = 0;
  // while (1) {
  //   nav.update();
  //   // if (compteur % 10000 == 0) nav.print_pos();
  //   compteur++;
  // }

  return 0;
}
