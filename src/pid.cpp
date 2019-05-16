#include "base.hpp"
#include "config.hpp"
#include "encoder.hpp"
#include "pid.hpp"
#include "position.hpp"
#include <math.h>
#include <queue>

// Durée de la rampe en seconde
#define RAMPE_LENGHT 3.0f
// ancienne rampe : 0.7f OK pour angle

extern int debug_monitor;

/* --- PID général --- */

PID::PID() {} // kewa?

PID::PID(float _p, float _i, float _d, float _erreurSeuil,
         float _accumulateurSeuil) {

  p = _p;
  i = _i;
  d = _d;

  accumulateur = 0;
  actionFinished = 0;
  derivee_data[0] = 0;
  derivee_data[1] = 0;
  derivee_data[2] = 0;

  accumulateurSeuil = _accumulateurSeuil;
  erreurSeuil = _erreurSeuil;
  time.start();

  last_time = time.read();
}

float PID::calculerConsigne() {
  // printf("ACC:%f", accumulateur);
  return (p * erreur + i * accumulateur + d * getDerivee()) * getRampe();
}

static void tab_shift(float *tab, int tab_size) {
  for (int i = tab_size; i > 0; i--) {
    tab[i] = tab[i - 1];
  }
}

float PID::getDerivee() {
  float dt = 1 / 1000.0f;
  // printf("%f, %f, %f, %f\n", derivee_data[0], derivee_data[1],
  // derivee_data[2], dt);
  return (((derivee_data[0] - derivee_data[1]) / dt) +
          ((derivee_data[1] - derivee_data[2]) / dt)) /
         2;
}

void PID::AccumulerErreur(float erreur) {

  accumulateur += erreur;

  tab_shift(derivee_data, 3);
  derivee_data[0] = erreur;
}

float PID::getConsigne() {
  erreur = this->calculerErreur();
  if (abs(erreur) < erreurSeuil) {
    reset();
    actionFinished = 1;
  } else {
    AccumulerErreur(erreur);
    actionFinished = 0;
  }
  return calculerConsigne();
}

float PID::getRampe() {
  if (time.read() < RAMPE_LENGHT)
    return time.read() / RAMPE_LENGHT;
  return 1;
}

void PID::reset() {
  accumulateur = 0;
  compteur_acc = 0;
  for (int i = 0; i < 1000; i++) {
    fifo.push(0);
    fifo.pop();
  }
  time.reset();
  actionFinished = 0;
}

/* --- PID en DISTANCE --- */
PIDDistance::PIDDistance() {}

PIDDistance::PIDDistance(float _p, float _i, float _d, float _erreurSeuil,
                         float _accumulateurSeuil, Position *position)
    : PID(_p, _i, _d, _erreurSeuil, _accumulateurSeuil) {
  pos = position;
}

float PIDDistance::calculerErreur() {
  float x = pos->get_x();
  float y = pos->get_y();

  float err = sqrt(carre(x - commande_x) + carre(y - commande_y));

  if (debug_monitor)
    printf("ED:%.2f ", err);

  return err;
}

void PIDDistance::setCommande(float x, float y) {
  commande_x = x;
  commande_y = y;
  PID::reset();
  this->time.reset();
}

/* --- PID en ANGLE --- */

PIDAngle::PIDAngle() {}

PIDAngle::PIDAngle(float _p, float _i, float _d, float _erreurSeuil,
                   float _accumulateurSeuil, Position *position)
    : PID(_p, _i, _d, _erreurSeuil, _accumulateurSeuil) {
  pos = position;
}

float PIDAngle::calculerErreur() {
  float theta = pos->get_theta();
  float err = modulo_angle_relatif(commande_theta - theta);

  if (debug_monitor)
    printf("EA:%.2f", err);
  return err;
}

void PIDAngle::setCommande(float theta) {
  commande_theta = theta;
  actionFinished = 0;
}

float calculerAngle(float x1, float y1, float x2, float y2) {
  /*
   * On calcule l'angle entre le vecteur (0, 1) et le vecteur (x2 - x1, y2 - y1)
   * En calculant le produit scalaire de ces deux vecteur divisé par le produit
   * de leur normes. norme_v représente la norme de ce 2ème vecteur.
   * */

  float dx = x2 - x1;
  float dy = y2 - y1;

  return atan2(dy, dx);
}

/* PID en Vitesse */

PIDVitesse::PIDVitesse() {}

PIDVitesse::PIDVitesse(float _p, float _i, float _d, float _erreurSeuil,
                       float _accumulateurSeuil, Encoder *encoder, float _coef)
    : PID(_p, _i, _d, _erreurSeuil, _accumulateurSeuil) {
  encod = encoder;
  coef = _coef;
  vitesse = 0;
}

void PIDVitesse::updateVitesse(short v) {
  vitesse = coef * (float)v;
  // printf("%d, %f\n", v, vitesse);
}

float PIDVitesse::calculerErreur() {
  float err = (commande_vitesse - vitesse);

  if (debug_monitor)
    printf("EV:(%.2f,%.2f)", err, vitesse);
  return err;
}

void PIDVitesse::setCommande(float v) { commande_vitesse = v; }
