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

DigitalOut direction_r(PF_13);
DigitalOut breal_r(PE_9);
DigitalOut direction_l(PE_11);
DigitalOut beak_l(PF_14);

Position pos(&enc_l, &enc_r);
Navigateur nav(&pos, &motor_l, &motor_r, &direction_l, &direction_r);



int setup() {
  printf("\r\nInitialisation du programme.\r\n");

  // Communication série
  // bt.baud(9600);
  usb.baud(115200);

  wait(1);

  printf("3...\n");

  wait(1);

  printf("2...\n");

  wait(1);

  printf("1...\n");

  wait(1);

  return 1;
}

int main()
{
  setup();


  Vecteur2D destination(0, 0);

  nav.set_destination(&destination);

  // Initialisation de l'interruption
 
  while (1) {
    nav.update();
  }

  return 0;

}
void test_motors() {

  motor_l.write(0.1f);
  direction_l = 1;

  wait(3);

  motor_l.write(0.1f);
  direction_l = 0;

  wait(3);

  motor_r.write(0.1f);
  direction_r = 1;

  wait(3);

  motor_r.write(0.1f);
  direction_r = 0;

  wait(3);

  motor_l.write(0);
  motor_r.write(0);
}
