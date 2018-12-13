#include "mbed.h"
#include "position.hpp"
#include "encoder.hpp"

const float D = 13900;// 0.2200684f * 367.0f; // mm.tick^-1 * distance entre roues

// TODO
// Ces 6 lignes doivent aller dans un fichier indépendant.
#define PI (3.141592654f)
#define TWOPI (6.283185307f)

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

void Position::update()
{
    float dl = (float)encod_l->diff();
    float dr = (float)encod_r->diff();

    float angle = (dr - dl) / D;
    float dx = (dl + dr) / 2.0f;
    float dy = 0.0f;

    if (abs(angle) > 0.0000175f) {
        float radius = angle * (dl + dr) / 2.0f;
        dx = radius * sin(angle);
        dy = radius * (1.0f - cos(angle));
    }

    x += dx;// cos(theta) * dx - sin(theta) * dy;
    y += dy;// sin(theta) * dx + cos(theta) * dy;
    theta += angle;
    if (abs(theta) > PI) theta -= sg(theta) * TWOPI;

}

void Position::init(){
    reset();
}

void Position::reset(){
    x = 0;
    y = 0;
    theta = 0;
}
