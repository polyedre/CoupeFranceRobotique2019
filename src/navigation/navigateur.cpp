#include "navigateur.hpp"
#include <stdio.h>
#include "../config.hpp"


float modulo_angle(float angle);

Navigateur::Navigateur(Position *_position, PwmOut *_m_l, PwmOut *_m_r, DigitalOut *_d_l, DigitalOut *_d_r)
{
    position = _position;
    cible = NULL;

    // FIXME : Trouver bonnes valeurs de pid.
    PIDDistance _pid_d(0.5, 1, 1, 0.05, 1, position);
    PIDAngle _pid_a(0.5, 0.0001, 100, 0.02, 0, position);

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


void limiter_consigne(float* consigne, int *direction){
    if (*consigne < 0) {
        *direction == 1 ? *direction = 0 : *direction = 1;
        *consigne = abs(*consigne);
    }
    // Commenté car géré directement dans la fonction update
    // consigne = min(*consigne, CONSIGNE_MAX);
    *consigne = min(*consigne, 0.2f);
    *consigne = max(*consigne, -0.2f);
}

float min (float a, float b) {
    return (a > b) ? b : a;
}

float max (float a, float b) {
    return (a < b) ? b : a;
}
void Navigateur::update()
{

    position->update();

    // Initialisation des consignes
    float angle_cons = 0;
    float dist_cons = 0;
    float angle_relatif = 0;
    float angle_absolu_destination = 0;
    int i = 1;
    int dir_l;
    int dir_r;
    float cmr;
    float cml;

    float x = position->get_x();
    float y = position->get_y();
    float theta = position->get_theta();

    // printf("x = %f, y = %f, theta=%f\n", x, y, theta);
    // printf("%f, %f, %f\r\n", x, y, theta);
    // printf("Accumulateurs : (Dist : %f) (Angle : %f)\r\n", pid_d.accumulateur, pid_a.accumulateur);


    angle_absolu_destination = calculerAngle(x, y, cible->x(), cible->y());

    angle_relatif = angle_absolu_destination - theta;

    angle_relatif = modulo_angle(angle_relatif);

    if (angle_relatif > PI / 2 || angle_relatif < - PI / 2) i = -1;

    dist_cons = pid_d.getConsigne();

    //  Calcul Consigne pour angle avec angle cible rafraichie
    pid_a.setCommande(angle_absolu_destination);

    angle_cons = pid_a.getConsigne();

    dist_cons = min(dist_cons, 0.1f);
    angle_cons = min(angle_cons, 0.3f);

    dist_cons = max(dist_cons, -0.1f);
    angle_cons = max(angle_cons, -0.3f);

    /*
      Initialisation des directions pour que les moteurs tournent dans
      le même sens. On inverse ensuite selon la consigne lors de l'appel
      à `limiter_consigne`.
      */
    if (i > 0) {
        dir_r = 1;
        dir_l = 0;

        cmr = (dist_cons + angle_cons) * 2; // Consigne moteur droit
        cml = dist_cons - angle_cons; // Consigne moteur gauche
    } else {
        dir_r = 0;
        dir_l = 1;

        cmr = (dist_cons - angle_cons) * 2; // Consigne moteur droit
        cml = dist_cons + angle_cons; // Consigne moteur gauche
    }

    printf("cr:%.2f cl:%.2f CA:%.2f CD:%.2f AA:%.2f AR:%.2f AF:%s, INV?:%s\n",
           cmr, cml,
           angle_cons, dist_cons,
           angle_absolu_destination, angle_relatif,
           pid_d.actionFinished ? "T" : "F",
           i < 0 ? "Y" : "N");

    limiter_consigne(&cmr, &dir_r);
    limiter_consigne(&cml, &dir_l);

    // printf("Consignes : (l : %f) (r : %f)\r\n", cmr, cml);
    print_pos();

    m_l->write(cml);
    m_r->write(cmr);

    *d_r = dir_r;
    *d_l = dir_l;

}

/*
 * Tourne le robot de `angle`. Angle représente l'angle relatif,
 * il peut être positif ou négatif.
 */
void Navigateur::rotate_by(float angle) {
    float theta = position->get_theta();
    float angle_dest = theta + angle;

    pid_a.setCommande(angle_dest);

    while (1) {

        position->update();
        float consigne = pid_a.getConsigne();

        consigne = min(consigne, 0.4f);

        float cmr = -consigne;
        float cml = consigne;

        int dir_l = 0;
        int dir_r = 0;

        limiter_consigne(&cmr, &dir_r);
        limiter_consigne(&cml, &dir_l);

        m_l->write(cml);
        m_r->write(cmr);

        *d_r = dir_r;
        *d_l = dir_l;

        print_pos();
    }
}

void Navigateur::print_pos()
{
    printf("\r(%.2f, %.2f, %.2f) - ", position->get_x(), position->get_y(), position->get_theta());
}

float modulo_angle(float angle) {

    while (angle > PI) {
        angle -= 2 * PI;
    }

    while (angle < - PI) {
        angle += 2 * PI;
    }

    return angle;
}
