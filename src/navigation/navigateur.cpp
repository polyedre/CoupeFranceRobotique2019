#include "../base.hpp"
#include "../config.hpp"
#include "navigateur.hpp"
#include <math.h>
#include <stdio.h>

extern int debug_monitor;
extern int move;
extern int running;

Navigateur::Navigateur(Position *_position, PwmOut *_m_l, PwmOut *_m_r,
                       DigitalOut *_d_l, DigitalOut *_d_r, Encoder *encod_l,
                       Encoder *encod_r) {
  position = _position;

  float p_vitesse = 6;
  float k = 0.023;
  // FIXME : Trouver bonnes valeurs de pid.
  PIDDistance _pid_d(0.8, 0.0, 0.0, 0.02, 1, position,
                     0.4f); // 0.5cm de précision
  PIDAngle _pid_a(0.5, 0.0, 0.01, 0.02, 0, position, 0.1f); // 1 degré
  // PIDAngle _pid_a(0.03, 0.001, 0.001, 0.02, 0, position);
  PIDVitesse _pid_v_l(p_vitesse * (1 - k), 0.01, 0, 0.001, 0, encod_l, 0.004,
                      1.0f);
  PIDVitesse _pid_v_r(p_vitesse * (1 + k), 0.01, 0, 0.001, 0, encod_r, 0.004,
                      1.0f);

  pid_d = _pid_d;
  pid_a = _pid_a;
  pid_v_l = _pid_v_l;
  pid_v_r = _pid_v_r;
  cible_x = 0;
  cible_y = 0;

  m_l = _m_l;
  m_r = _m_r;
  m_l->period(PWM_PERIOD);
  m_r->period(PWM_PERIOD);
  d_l = _d_l;
  d_r = _d_r;
}

void Navigateur::set_destination(float x, float y) {
  cible_x = x;
  cible_y = y;
  // Cible sert de stokage de la consigne
  pid_d.setCommande(cible_x, cible_y);
}

void Navigateur::reset() {
  position->reset();
  set_destination(0, 0);
  reset_pids();
}

void Navigateur::reset_pids() {
  pid_a.reset();
  pid_d.reset();
}

void Navigateur::update() {

  float x = position->get_x();
  float y = position->get_y();
  float theta = position->get_theta();

  // Initialisation des consignes
  float angle_cons = 0;
  float dist_cons = 0;
  float angle_relatif = 0;
  float angle_absolu_destination = theta;
  int i = 1;
  int dir_l;
  int dir_r;
  float cmr;
  float cml;

  int triggered = 0; // False

  float distance_cible = sqrt(carre(x - cible_x) + carre(y - cible_y));

  if (distance_cible > 0.01) {
    angle_absolu_destination =
        modulo_angle_absolu(calculerAngle(x, y, cible_x, cible_y));
  } else {
    angle_absolu_destination = theta;
  }

  angle_relatif = modulo_angle_relatif(angle_absolu_destination - theta);

  if ((abs(angle_relatif) < 0.3) || (abs(abs(angle_relatif) - PI) < 0.3)) {
    triggered = 1;
    dist_cons = pid_d.getConsigne();
  } else {
    pid_d.reset();
  }

  if ((angle_relatif < PI + 0.3) && (angle_relatif > PI - 0.3))
    i = -1;

  //  Calcul Consigne pour angle avec angle cible rafraichie
  if (i > 0) {
    pid_a.setCommande(angle_absolu_destination);
  } else {
    pid_a.setCommande(modulo_angle_absolu(angle_absolu_destination + PI));
  }
  angle_cons = pid_a.getConsigne();

  dist_cons = min(dist_cons, 0.2);
  angle_cons = min(angle_cons, 0.2);

  dist_cons = max(dist_cons, -0.2f);
  angle_cons = max(angle_cons, -0.2f);

  /*
    Initialisation des directions pour que les moteurs tournent dans
    le même sens. On inverse ensuite selon la consigne lors de l'appel
    à `limiter_consigne`.
    */
  // if (i > 0) {
  //   dir_r = 1;
  //   dir_l = 0;

  //   cmr = dist_cons + angle_cons; // Consigne moteur droit
  //   cml = dist_cons - angle_cons; // Consigne moteur gauche
  // } else {
  dir_r = 0;
  dir_l = 1;

  cmr = dist_cons - angle_cons; // Consigne moteur droit
  cml = dist_cons + angle_cons; // Consigne moteur gauche
  // }

  pid_v_r.setCommande(cmr);
  float cmr_v = pid_v_r.getConsigne();

  pid_v_l.setCommande(cml);
  float cml_v = pid_v_l.getConsigne();

  if (debug_monitor) {
    print_pos();
    printf("cx:%.2f cy:%.2f t:%.2f r:%.2f l:%.2f A:%.2f D:%.2f R:%.2f F:%s "
           "I:%s T:%s ",
           cible_x, cible_y, convert_degree(angle_absolu_destination), cmr_v,
           cml_v, angle_cons, dist_cons, convert_degree(angle_relatif),
           pid_d.actionFinished ? "T" : "F", i < 0 ? "Y" : "N",
           triggered == 1 ? "Y" : "N");
  }

  limiter_consigne(&cmr_v, &dir_r);
  limiter_consigne(&cml_v, &dir_l);

  // printf("Consignes : (l : %f) (r : %f)\r\n", cmr, cml);

  if (move) {
    m_l->write(cml_v);
    m_r->write(cmr_v);
  }

  *d_r = dir_r;
  *d_l = dir_l;
}

/*
 * Tourne le robot de `angle`. Angle représente l'angle relatif,
 * il peut être positif ou négatif.
 */
void Navigateur::rotate_by(float angle) {
  float theta = position->get_theta();
  float angle_dest = modulo_angle_absolu(theta + angle);

  pid_a.setCommande(angle_dest);
}

/*
 * Avance de `distance` mètres.
 * TODO : Tester cette fonction
 */
void Navigateur::avancer(float distance) {
  float x = position->get_x();
  float y = position->get_y();
  float theta = position->get_theta();

  float new_x = x + distance * cos(theta);
  float new_y = y + distance * sin(theta);

  set_destination(new_x, new_y);
}

/*
 * Va à la position `x`, `y` mètres.
 */
void Navigateur::go_to(float cx, float cy) { set_destination(cx, cy); }

void Navigateur::print_pos() {
  printf("\n\rx:%.2f y:%.2f t:%.2f ", position->get_x(), position->get_y(),
         convert_degree(position->get_theta()));
}

void Navigateur::updatePos() {
  short vitesses[2] = {0, 0};
  position->update(vitesses);
  pid_v_r.updateVitesse(vitesses[0]);
  pid_v_l.updateVitesse(vitesses[1]);
}
