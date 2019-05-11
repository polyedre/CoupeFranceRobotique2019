#include "mbed.h"
#include "position.hpp"
#include "encoder.hpp"
#include "config.hpp"
#include "base.hpp"
#include <cmath>

// const float D = 13900; // 0.2200684f * 367.0f; // mm.tick^-1 * distance entre roues

float sg(float val)
{
    return (val < 0.0f) ? -1.0f : 1.0f;
}

Position::Position(){}

Position::Position(Encoder* _encod_l, Encoder* _encod_r)
{
    x = 0;
    y = 0;
    theta = 0;
    encod_l = _encod_l;
    encod_r = _encod_r;
}

float Position::get_x()
{
    return x;
}

float Position::get_y()
{
    return y;
}

float Position::get_theta()
{
    return theta;
}

/**
 * Nouveau modèle de déplacement élémentaire centré autour du milieu des deux encodeurs.
 * */
void Position::update(short vitesses[])
{
    short dl = encod_l->diff();
    short dr = encod_r->diff();

    vitesses[0] = dr;
    vitesses[1] = dl;

    float del = dl / ENCODEUR_ECHELLE;     // mouvement de l'encodeur gauche en mètres
    float der = dr / ENCODEUR_ECHELLE;    // mouvement de l'encodeur droit en mètres

    float dx = (del + der) / 2 * cos(theta);
    float dy = (del + der) / 2 * sin(theta);
    float dTh = (der - del) / DIAMETER;

    // printf("(dx, del, der) = (%f, %f, %f)\n", dx, del, der);

    // TODO : check la position du calcul de theta

    x += dx;
    y += dy;
    theta = theta + dTh;
    theta = modulo_angle_absolu(theta);

}

void Position::init(){
    reset();
}

void Position::reset(){
    x = 0;
    y = 0;
    theta = 0;
}
