#include "navigateur.hpp"
#include <stdio.h>

Navigateur::Navigateur(Position *position)
{
    position = position;
    cible = NULL;

    // FIXME : Trouver bonnes valeurs de pid.
    PIDDistance _pid_d(1, 1, 1, 1, 1, position);
    PIDAngle _pid_a(1, 1, 1, 1, 1, position);

    pid_d = _pid_d;
    pid_a = _pid_a;
}

void Navigateur::set_destination(Vecteur2D *c)
{
    // FIXME : supprimer la variable cible ?
    cible = c;
    pid_d.setCommande(cible->x(), cible->y());
}

void Navigateur::update()
{

  if (!pid_d.actionFinished) {

    position->update();

    float x = position->get_x();
    float y = position->get_y();
    float theta = position->get_theta();

    float angle_relatif = calculerAngle(x, y, cible->x(), cible->y());

    if (angle_relatif < 30) {
    pid_a.setCommande(angle_relatif);

    float dist_cons = pid_d.getConsigne();
    float angle_cons = pid_a.getConsigne();

    float cmr = dist_cons + angle_cons; // Consigne moteur droit
    float cml = dist_cons - angle_cons; // Consigne moteur gauche

    } else {

    // TODO : implémenter rotation simple

    }

    // TODO : implémenter le travail des moteurs

    }

}
