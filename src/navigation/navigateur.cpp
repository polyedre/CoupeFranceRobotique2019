#include "navigateur.hpp"

Navigateur::Navigateur(Position *position, Controleur* controleur)
{
    position = position;
    cible = NULL;
    angle_reel = NULL;
}

void Navigateur::chg_destination(Vecteur2D *c)
{
    cible = c;
}

void Navigateur::update()
{
    // TODO

    float angle_cible = angle(cible->x() - position->get_x(), cible->y() - position->get_y());
    if (angle != NULL && abs(angle_cible - position->get_theta()) > 0.1)
    {
        // TODO rotation
    }

    float distance = (position->get_x() - cible->x()) * (position->get_x() - cible->x()) +
                     (position->get_y() - cible->y()) * (position->get_y() - cible->y());

    if (distance > NAV_PRECISION * NAV_PRECISION)
    {
        // TODO avancer
    }
}