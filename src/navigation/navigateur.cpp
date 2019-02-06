#include "navigateur.hpp"
#include <stdio.h>

Navigateur::Navigateur(Position *_position, PwmOut *_m_l, PwmOut *_m_r)
{
    position = _position;
    cible = NULL;

    // FIXME : Trouver bonnes valeurs de pid.
    PIDDistance _pid_d(1, 1, 1, 0.05, 1, position);
    PIDAngle _pid_a(1, 1, 1, 0.02, 100, position);

    pid_d = _pid_d;
    pid_a = _pid_a;

    m_l = _m_l;
    m_r = _m_r;
}

void Navigateur::set_destination(Vecteur2D *c)
{
    // FIXME : supprimer la variable cible ?
    cible = c;
    pid_d.setCommande(cible->x(), cible->y());
}


void limiter_consigne(float* consigne){
    if (*consigne > 1) *consigne = 1;
    else if (*consigne < 0) *consigne = 0;
}

float max(float a, float b) {
    return a < b ? b : a;
}

void Navigateur::update()
{

  if (!pid_d.actionFinished) {

    position->update();

    float x = position->get_x();
    float y = position->get_y();
    float theta = position->get_theta();

    printf("%f, %f, %f\r\n", x, y, theta);
    printf("Accumulateurs : (Dist : %f) (Angle : %f)\r\n", pid_d.accumulateur, pid_a.accumulateur);

    float angle_absolu_destination = calculerAngle(x, y, cible->x(), cible->y());

    float angle_relatif = angle_absolu_destination - theta;

    // Calcul Consigne pour angle avec angle cible rafraichie
    pid_a.setCommande(angle_absolu_destination);
    float angle_cons = pid_a.getConsigne();
    float dist_cons = 0;

    // Si la cible appartient à un cône de vision
    // alors on calcule la consigne de distance
    if (angle_relatif < 2.5f) dist_cons = pid_d.getConsigne();

    float cmr = dist_cons + angle_cons; // Consigne moteur droit
    float cml = dist_cons - angle_cons; // Consigne moteur gauche

    limiter_consigne(&cmr);
    limiter_consigne(&cml);

    printf("Consignes : (l : %f) (r : %f)\r\n", cmr, cml);

    m_l->write(max(cml, 0.7));
    m_r->write(max(cmr, 0.7));
    }

}
