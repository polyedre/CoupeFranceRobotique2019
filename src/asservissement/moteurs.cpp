#include "asservissement/moteurs.hpp"
#include <cmath>

#define PuissanceMax 0.2f
#define Limite 10000

Controleur::Controleur(Position *pos, PwmOut *moteur_d, PwmOut *moteur_g) {
  puissance = 0;
  position = pos;
  moteur_droit = moteur_d;
  moteur_gauche = moteur_g;
}

/** Avance sur la distance demandée en conservant le robot droit */
void Controleur::avancer(float distance) {
  position->encod_l->total = 0;
  position->encod_r->total = 0;

  int ecart_d = distance - position->encod_r->total;
  int ecart_g = distance - position->encod_l->total;

  while (ecart_d > 0 || ecart_g > 0) {
    if (ecart_d > Limite) {
      moteur_droit->write(PuissanceMax);
      printf("\rTotal: %d,\tPuissance: %f     ", position->encod_r->total,
             PuissanceMax);
    } else {
      moteur_droit->write(PuissanceMax * (float)ecart_d / Limite);
      printf("\rTotal:%d,\tpuissance: %f     ", position->encod_r->total,
             PuissanceMax * ecart_d / Limite);
    }

    if (ecart_g > Limite) {
      moteur_gauche->write(PuissanceMax);
    } else {
      moteur_gauche->write(PuissanceMax * (float)ecart_g / Limite);
    }
    // printf("cote droit: %d \t cote gauche: %d       \r", parcours_d,
    // parcours_g);
    position->update();
    ecart_d = distance + position->encod_r->total;
    ecart_g = distance - position->encod_l->total;
  }

  moteur_droit->write(0);
  moteur_gauche->write(0);
}

/** Fait tourner le robot sur lui-même */
void Controleur::rotation(float angle) {}