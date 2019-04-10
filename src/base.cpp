#include "base.hpp"
#include "config.hpp"

float carre(float a){
    return a * a;
}

float modulo_angle_absolu(float angle) {

    while (angle > 2 * PI) {
        angle -= 2 * PI;
    }

    while (angle < 0) {
        angle += 2 * PI;
    }

    return angle;
}

float modulo_angle_relatif(float angle) {

    while (angle > PI) {
        angle -= 2 * PI;
    }

    while (angle < - PI) {
        angle += 2 * PI;
    }

    return angle;
}

float convert_degree(float angle) {
    return angle / PI * 180;
}