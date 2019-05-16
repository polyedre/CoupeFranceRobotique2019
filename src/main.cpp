#include "GP2.hpp"
#include "base.hpp"
#include "config.hpp"
#include "encoder.hpp"
#include "mbed.h"
#include "navigation/navigateur.hpp"
#include "navigation/vecteur2D.hpp"
#include "pid.hpp"
#include "position.hpp"
#include <math.h>

/* Class objects */

DigitalOut led(LED3);
Serial usb(USBTX, USBRX);

Encoder enc_l(TIM3); // PD12 - PD13
Encoder enc_r(TIM4); // PC6  - PC7 ou PA6 - PA7 ou PB4 - PB5

PwmOut motor_l(PB_13); // TODO changer les noms des pins
PwmOut motor_r(PB_15);

DigitalOut direction_r(PF_13);
DigitalOut break_r(PE_9);
DigitalOut direction_l(PE_11);
DigitalOut break_l(PF_14);

Ticker updatePos_t;
Ticker checkGP2_t;

Position pos(&enc_l, &enc_r);
Navigateur nav(&pos, &motor_l, &motor_r, &direction_l, &direction_r, &enc_l,
               &enc_r);

// // TODO : Donner des ports aux GP2
AnalogIn gp2_analog_1(PC_0);
// AnalogIn gp2_analog_2();
// AnalogIn gp2_analog_3();
// AnalogIn gp2_analog_4();

DigitalOut alim_gp2_1(PC_3);

GP2 gp2_list[1] = {
    GP2(&gp2_analog_1, 0.5),
    // GP2(gp2_analog_2, 0.3),
    // GP2(gp2_analog_3, 0.3),
    // GP2(gp2_analog_4, 0.3),
};

/* Prototypes */

void handleInput();
void updatePos();
void test_motors();
void test_rotation();
void check_all_GP2();
void frein();

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
  updatePos_t.attach(&updatePos, 0.0001f);
  checkGP2_t.attach(&check_all_GP2, 0.1f);

  alim_gp2_1 = 1;

  // detected_all(gp2_list, 1);

  // for (int i = 0; i < 3; i++) {
  //   printf("%d...\n", i);
  //   wait(1);
  // }

  nav.set_destination(0, 0);
}

void loop() {

  // ====== GOTO ======

  nav.go_to(0.5, 0.0);
  nav.go_to(0.5, 0.5);
  nav.go_to(1.0, 0.5);
  nav.go_to(1.0, 0.0);
  printf("\nLast run\n");
  nav.go_to(0.0, 0.0);

  //  // ======  Homologation  ======

  // printf("\nAvancer\n");
  // nav.avancer(0.4);

  // printf("\nPI/2\n");
  // nav.rotate_by(PI_OVER_TWO);

  // printf("\nAvancer\n");
  // nav.avancer(0.4);

  // printf("\nPI/2\n");
  // nav.rotate_by(-PI_OVER_TWO);

  // printf("\nAvancer\n");
  // nav.avancer(0.4);

  // printf("\nPI/2\n");
  // nav.rotate_by(-PI_OVER_TWO);

  // printf("\nAvancer\n");
  // nav.avancer(0.4);

  // printf("\nPI/2\n");
  // nav.rotate_by(-PI_OVER_TWO);

  // printf("\nAvancer\n");
  // nav.avancer(0.7);

  //   ==========  CARRÉ  ==============
  //   while (1) {
  //   if (running){

  //     nav.avancer(0.3);
  //     nav.rotate_by(PI_OVER_TWO);

  //   }
  // }
}

int main() {
  printf("Setup\n");
  setup();
  printf("loop\n");
  // nav.avancer(0.5);
  // nav.rotate_by(PI_OVER_TWO);
  loop();
  // test_motors();
  // test_rotation();
  return 0;
}

void test_motors() {

  printf("Avancer\n");
  // Avancer tout droit
  motor_l.write(0.1f);
  direction_l = 0;
  motor_r.write(0.1f);
  direction_r = 1;

  wait(1);

  // motor_l.write(0.0f);
  // motor_r.write(0.0f);

  printf("Reculer\n");
  // wait(2);

  // Reculer tout droit
  // motor_l.write(0.2f);
  direction_l = 1;
  // motor_r.write(0.2f);
  direction_r = 0;

  wait(1);

  motor_l.write(0.0f);
  motor_r.write(0.0f);

  // wait(1);

  // // Tourner
  // motor_l.write(0.2f);
  // direction_l = 1;
  // motor_r.write(0.2f);
  // direction_r = 1;

  // wait(1);

  // // Tourner
  // direction_l = 0;
  // direction_r = 0;

  // wait(1);

  // motor_l.write(0.0f);
  // motor_r.write(0.0f);

  // // tourner
  // direction_l = 0;
  // direction_r = 1;

  // nav.pid_v_l.reset();
  // nav.pid_v_r.reset();
  // nav.pid_v_l.setCommande(0.2f);
  // nav.pid_v_r.setCommande(0.2f);

  // printf("DEBUT\n");

  // while (running) {
  //   float cml = nav.pid_v_l.getConsigne();
  //   float cmr = nav.pid_v_r.getConsigne();

  //   printf("%f %f\n", cmr, cml);

  //   motor_l.write(cml);
  //   motor_r.write(cmr);

  // }

  // printf("FIN_TEST\n");

  // motor_l.write(0.0f);
  // motor_r.write(0.0f);
}

void handleInput() {
  char c = usb.getc();

  if (c == 'q') {
    running = running ? 0 : 1;
    if (running)
      printf("\nComputation activé.\n");
    else
      printf("\nComputation désactivé.\n");
    if (!running) {
      motor_l.write(0.0f);
      motor_r.write(0.0f);
    }
  }

  if (c == 'm') {
    move = move ? 0 : 1;
    if (move)
      printf("\nMouvement activé.\n");
    else
      printf("\nMouvement désactivé.\n");
    if (!move) {
      frein();
    }
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
    nav.set_destination(x, y);

    printf("\nNew destination set : (%f, %f)\n", x, y);
  }

  if (c == 'd') { // Toggle debug
    debug_monitor = debug_monitor ? 0 : 1;
    if (debug_monitor)
      printf("\nDebug activé.\n");
    else
      printf("\nDebug désactivé.\n");
  }

  if (c == 'r') { // reset
    printf("\nResetting the program.\n");
    SCB->AIRCR = 0x05fa0004;
    // reset();
    running = 1;
    setup();
  }

  if (c == 'p') { // Print position
    nav.print_pos();
  }

  if (c == 't') { // Run Test
    test_motors();
  }
}

void updatePos() { nav.updatePos(); }

void check_all_GP2() {
  if (detected_all(gp2_list, 1)) {
    frein();
    printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!TRIGGERED!!!!!!!!!!!!!!!");
  }
}

void test_rotation() {
  nav.pid_a.setCommande(-PI_OVER_TWO);

  while (running) {
    float command = nav.pid_a.getConsigne();

    int d_l = 0;
    int d_r = 1;

    nav.pid_v_l.setCommande(-command);
    nav.pid_v_r.setCommande(command);

    float cml = nav.pid_v_l.getConsigne();
    float cmr = nav.pid_v_r.getConsigne();

    limiter_consigne(&cml, &d_l);
    limiter_consigne(&cmr, &d_r);

    nav.print_pos();
    printf("%f %f %f ", command, cml, cmr);

    if (move) {
      motor_l.write(cml);
      motor_r.write(cmr);
    }

    direction_l = d_l;
    direction_r = d_r;
  }
}

void frein() {
  motor_l.write(0.001f);
  motor_r.write(0.001f);
  break_l = 1;
  break_r = 1;
  wait(0.5f);
  break_l = 0;
  break_r = 0;
}
