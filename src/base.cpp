#include "base.hpp"
#include "config.hpp"

float carre(float a){
    return a * a;
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
