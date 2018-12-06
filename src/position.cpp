#include "mbed.h"
#include "position.hpp"
#include "encoder.hpp"
#include <cmath>
#include "config.hpp"

const float D = 13900; // 0.2200684f * 367.0f; // mm.tick^-1 * distance entre roues

float sg(float val)
{
    return (val < 0.0f) ? -1.0f : 1.0f;
}

Position::Position(Encoder *_encod_l, Encoder *_encod_r)
{
    RT_x = 0;
    RT_y = 0;
    RT_theta = 0;
    encod_l = _encod_l;
    encod_r = _encod_r;
    temps = new Timer();
}

float Position::get_x()
{
    return RT_x;
}

float Position::get_y()
{
    return RT_y;
}

float Position::get_theta() { return RT_theta; }

/**
 * Nouveau modèle de déplacement élémentaire centré autour du milieu des deux encodeurs.
 * */
void Position::update()
{
    float del = encod_l->diff() / ENCODEUR_ECHELLE;     // mouvement de l'encodeur gauche en mètres
    float der = -encod_r->diff() / ENCODEUR_ECHELLE;    // mouvement de l'encodeur droit en mètres

    float dx = (del + der) * cos(RT_theta) / 2;
    float dy = (del + der) * sin(RT_theta) / 2;
    float dTh = (der - del) / RADIUS;

    RT_x += dx;
    RT_y += dy;
    RT_theta += dTh;

    vit_encod_d = der / temps->read_ms();
    vit_encod_g = del / temps->read_ms();

    temps->reset();
}
