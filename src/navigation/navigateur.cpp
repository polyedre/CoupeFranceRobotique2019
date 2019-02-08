#include "navigateur.hpp"
#include <stdio.h>

Navigateur::Navigateur(Position *_position, PwmOut *_m_l, PwmOut *_m_r, DigitalOut *_d_l, DigitalOut *_d_r)
{
    position = _position;
    cible = NULL;

    // FIXME : Trouver bonnes valeurs de pid.
    PIDDistance _pid_d(1, 1, 1, 0.20, 1, position);
    PIDAngle _pid_a(5, 1, 1, 0.02, 100, position);

    pid_d = _pid_d;
    pid_a = _pid_a;

    m_l = _m_l;
    m_r = _m_r;
    m_l->period(PWM_PERIOD);
    m_r->period(PWM_PERIOD);
    d_l = _d_l;
    d_r = _d_r;

}

void Navigateur::set_destination(Vecteur2D *c)
{
    // FIXME : supprimer la variable cible ?
    cible = c;
    pid_d.setCommande(cible->x(), cible->y());
}


void limiter_consigne(float* consigne, DigitalOut *direction){
    if (*consigne > 1) *consigne = 1;
    else if (*consigne < 0) {
    *consigne *= -1;
    *direction == 1 ? *direction = 0 : *direction = 1; // On inverse le moteur
    }
}

float min (float a, float b) {
    return (a > b) ? b : a;
}

void Navigateur::update()
{

  if (!pid_d.actionFinished) {

    position->update();

    float x = position->get_x();
    float y = position->get_y();
    float theta = position->get_theta();

    // printf("x = %f, y = %f, theta=%f\n", x, y, theta);
    // printf("%f, %f, %f\r\n", x, y, theta);
    // printf("Accumulateurs : (Dist : %f) (Angle : %f)\r\n", pid_d.accumulateur, pid_a.accumulateur);

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

    /*
      Initialisation des directions pour que les moteurs tournent dans
      le même sens. On inverse ensuite selon la consigne lors de l'appel
      à `limiter_consigne`.
      */
    *d_l = 0;
    *d_r = 1;

    limiter_consigne(&cmr, d_r);
    limiter_consigne(&cml, d_l);

    cmr = min(cmr, 0.4);
    cml = min(cml, 0.4);

    // printf("Consignes : (l : %f) (r : %f)\r\n", cmr, cml);
    printf("(cmr, cml, cons) = (%.2f, %.2f, %.2f)\n", cmr, cml, angle_cons);

    m_l->write(cml);
    m_r->write(cmr);
    }

}

void Navigateur::print_pos()
{
    printf("(x, y, theta) = (%.2f, %.2f, %.2f)\n", position->get_x(), position->get_y(), position->get_theta());
}
