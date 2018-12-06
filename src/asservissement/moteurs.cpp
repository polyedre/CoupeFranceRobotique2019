#include "asservissement/moteurs.hpp"
#include <cmath>

#define PuissanceMax 0.1f
#define Limite 1000

Controleur::Controleur(Position *pos) {
  puissance = 0;
  position = pos;
  moteur_droit = new PwmOut(PIN_Moteur_droit);
  moteur_gauche = new PwmOut(PIN_Moteur_gauche);
}

/** Avance sur la distance demandée en conservant le robot droit */
void Controleur::avancer(float distance) {
  int parcours_d = 0;
  int parcours_g = 0;
  int encodeur_d = position->encod_r->get();
  int encodeur_g = position->encod_l->get();

  double ecart_d = abs(distance - parcours_d);
  double ecart_g = abs(distance - parcours_g);

  while (ecart_d > 0 || ecart_g > 0) {
    if (ecart_d > Limite) {
      moteur_droit->write(PuissanceMax);
      printf("Puissance: %f     \r", PuissanceMax);
    } else {
      moteur_droit->write(PuissanceMax * ecart_d / Limite);
      printf("puissance: %f     \r", PuissanceMax * ecart_d / Limite);
    }

    /*if (ecart_g > Limite) {
      moteur_gauche->write(PuissanceMax);
    } else {
      moteur_gauche->write(PuissanceMax * ecart_g / Limite);
    }*/

    parcours_d = abs(position->encod_r->get() - encodeur_d);
    parcours_g = abs(position->encod_l->get() - encodeur_g);
    //printf("cote droit: %d \t cote gauche: %d       \r", parcours_d, parcours_g);
    ecart_d = abs(distance - parcours_d);
    ecart_g = abs(distance - parcours_g);
  }
}

/** Fait tourner le robot sur lui-même */
void Controleur::rotation(float angle) {}