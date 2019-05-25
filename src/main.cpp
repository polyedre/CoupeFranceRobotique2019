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

PwmOut motor_l(PB_13);
PwmOut motor_r(PB_15);

DigitalOut direction_r(PF_13);
DigitalOut break_r(PE_9);
DigitalOut direction_l(PE_11);
DigitalOut break_l(PF_14);

DigitalIn starterBtn(PE_4);
DigitalIn sideBtn(PF_15);

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

enum Side { BLUE_LEFT, BLUE_RIGHT };

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
enum Side side;

void setup() {

  // Communication série
  usb.baud(115200);
  printf("\n\nInitialisation du programme.\n");

  usb.attach(&handleInput);
  updatePos_t.attach(&updatePos, 0.001f);
  checkGP2_t.attach(&check_all_GP2, 0.1f);

  starterBtn.mode(PullUp);
  sideBtn.mode(PullUp);

  if (sideBtn.read()) {
    side = BLUE_LEFT;
    printf("Coté détecté : Bleu à gauche.\n");
  } else {
    side = BLUE_RIGHT;
    printf("Coté détecté : Bleu à droite.\n");
  }
  nav.set_destination(0, 0);

  printf("Je suis prêt\n");
}

void loop() {

  // ===== MAIN CODE =====

  printf("Attente du démarrage...\n");

  while (!starterBtn.read()) {
  }

  printf("C'est parti !\n");

  if (side == BLUE_LEFT) { // Bleu à gauche
    nav.go_to(0.1f, 0.0f);
    nav.rotate_by(PI_OVER_TWO);
    nav.go_to(0.1f, 0.3f);
    nav.rotate_by(-PI_OVER_TWO);
    nav.go_to(0.5f, 0.3f);
    nav.rotate_by(-3 * PI / 4 + 0.05);
    nav.go_to(0.2f, -0.4f);
  } else {
    printf("Ce coté n'a pas été implémenté\n");
  }
  // ===== TESTS =====

  // nav.go_to(1, 0);
  // wait(0.5);
  // nav.rotate_by(PI_OVER_TWO);
  // wait(0.5);
  // nav.rotate_by(-PI);

  // ==========  CARRÉ  ==============
  //   while (1) {
  //   if (running){

  //     nav.avancer(0.3);
  //     nav.rotate_by(PI_OVER_TWO);

  //   }
  // }
}

int main() {
  setup();
  loop();
  return 0;
}

void test_motors() {

  printf("Avancer\n");
  // Avancer tout droit
  motor_l.write(0.1f);
  direction_l = 1;
  motor_r.write(0.1f);
  direction_r = 0;

  wait(1);

  // motor_l.write(0.0f);
  // motor_r.write(0.0f);

  printf("Reculer\n");
  // wait(2);

  // Reculer tout droit
  // motor_l.write(0.2f);
  direction_l = 0;
  // motor_r.write(0.2f);
  direction_r = 1;

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

  if (c == 'f') {
    frein();
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

void updatePos() { nav.updatePos(); }

/* Return whether the position might be a wall */
int pos_is_a_wall(float x, float y) {
  return (abs(x - (0 - X_INIT)) < WALL_DETECTION_GAP) ||
         (abs(x - (HEIGHT_TABLE - X_INIT)) < WALL_DETECTION_GAP) ||
         (abs(x - (0 - Y_INIT)) < WALL_DETECTION_GAP) ||
         (abs(x - (WIDTH_TABLE - X_INIT)) < WALL_DETECTION_GAP);
}

/* Return whether the position (x, y) is on the table */
int pos_is_on_table(float x, float y) {
  return (x < HEIGHT_TABLE - X_INIT) && (x > 0 - X_INIT) &&
         (y < WIDTH_TABLE - Y_INIT) && (y > 0 - Y_INIT);
}

void check_all_GP2() {
  if (int num = detected_all(gp2_list, 1)) {
    float theta = pos.get_theta();
    float obj_x;
    float obj_y;
    switch (num) {
    case 1: // Front GP2
      obj_x = pos.get_x() + gp2_list[num].real_distance * cos(theta);
      obj_y = pos.get_y() + gp2_list[num].real_distance * sin(theta);
      if (pos_is_on_table(obj_x, obj_y) && !pos_is_a_wall(obj_x, obj_y)) {
        frein();
        if (debug_monitor)
          printf("\nStopped because object on the table\n");
      };
      break;
    case 2: // Back GP2
      obj_x = pos.get_x() - gp2_list[num].real_distance * cos(theta);
      obj_y = pos.get_y() - gp2_list[num].real_distance * sin(theta);
      if (pos_is_on_table(obj_x, obj_y) && !pos_is_a_wall(obj_x, obj_y)) {
        if (debug_monitor)
          printf("\nObject behind on the table\n");
      };
      break;
    }
  }
}

void frein() {
  motor_l.write(0.0f);
  motor_r.write(0.0f);
  wait(1);
  nav.reset_pids();
}
