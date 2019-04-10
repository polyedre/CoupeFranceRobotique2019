#include "mbed.h"
#include "encoder.hpp"
#include "navigation/navigateur.hpp"
#include "navigation/vecteur2D.hpp"
#include "position.hpp"
#include "pid.hpp"
#include "config.hpp"
#include <math.h>
#include "base.hpp"

/* Class objects */

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

/* Prototypes */

void handleInput();

/* Global variables */

int debug_monitor = 1;
int running = 1;
int move = 1;

void setup() {

  // Communication série
  // bt.baud(9600);
  usb.baud(115200);
  printf("\r\nInitialisation du programme.\r\n");

  usb.attach(&handleInput);

  for (int i = 0; i < 3; i++) {
    printf("%d...\n", i);
    wait(1);
  }

  printf("%f", convert_degree(3.14));

  Vecteur2D destination(0, 0);
  nav.set_destination(&destination);

}

void loop() {

    while (1) {
    if (running){
      nav.update();
    } else {
      motor_l = 0;
      motor_r = 0;
      wait(0.5);
    }
  }

}

// void reset() {
//   nav.reset();
// }

int main()
{
  setup();
  loop();

  return 0;

}



void test_motors() {

  // Avancer tout droit
  motor_l.write(0.2f);
  direction_l = 0;
  motor_r.write(0.2f);
  direction_r = 1;
  
  wait(1);

  motor_l.write(0.0f);
  motor_r.write(0.0f);

  // wait(2);

  // Reculer tout droit
  motor_l.write(0.2f);
  direction_l = 1;
  motor_r.write(0.2f);
  direction_r = 0;

  wait(1);

  motor_l.write(0.0f);
  motor_r.write(0.0f);

  wait(1);

  motor_l.write(0.2f);
  direction_l = 0;

  wait(1);

  motor_l.write(0.0f);
  motor_r.write(0.2f);
  direction_r = 1;

  wait(1);
  motor_l.write(0.0f);
  motor_r.write(0.0f);
}

void handleInput() {
  char c = usb.getc();

  if (c == 'q') {
    running = running ? 0 : 1;
    if (running) printf("\nComputation activé.\n");
    else printf("\nComputation désactivé.\n");
  }

  if (c == 'm') {
    move = move ? 0 : 1;
    if (move) printf("\nMouvement activé.\n");
    else printf("\nMouvement désactivé.\n");
  }

  if (c == 's') { // set destination
    printf("\nSet mode, chose a destination: \n");

    float x;
    printf("\nx = ");
    usb.scanf("%f", &x);

    float y;
    printf("\ny = ");
    usb.scanf("%f", &y);

    Vecteur2D destination(x, y);
    nav.set_destination(&destination);

    printf("\nNew destination set : (%f, %f)\n", x, y);

  }

  if (c == 'd') { // Toggle debug
    debug_monitor = debug_monitor ? 0 : 1;
    if (debug_monitor) printf("\nDebug activé.\n");
    else printf("\nDebug désactivé.\n");
  }

  if (c == 'r') { // reset
    printf("\nResetting the program.\n");
    SCB->AIRCR = 0x05fa0004;
    // reset();
    running = 1;
    setup();
  }

  if (c == 'p') {
    nav.print_pos();
  }

  if (c == 't') {
    test_motors();
  }
}
