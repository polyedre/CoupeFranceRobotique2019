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
  usb.printf("\r\nNouveau code ?.\r\n");

  // (&motor_l)->write(0.5f);
  // wait(1);
  // motor_l.write(0.0f);
  // motor_r.write(0.5f);
  // wait(1);
  // motor_r.write(0.0f);

  Vecteur2D destination(1,0);

  nav.set_destination(&destination);

  int compteur = 0;
  while (1) {
    wait(0.5);
    nav.update();
  }

  return 0;
}
