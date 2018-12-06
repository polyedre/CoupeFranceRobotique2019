#ifndef __POSITION_H_
#define __POSITION_H_

#include "encoder.hpp"
#include "mbed.h"

/** Classe pour mesurer la position du robot. */
class Position {
public:
  Position(Encoder *encod_g, Encoder *encod_d);
  float get_x();
  float get_y();
  float get_theta();
  void update();

  Encoder *encod_l;
  Encoder *encod_r;

  float RT_x;     // Référentiel terrestre position x
  float RT_y;     // Référentiel terrestre position y
  float RT_theta; // Référentiel terrestre angle

  float vit_encod_d;
  float vit_encod_g;

private:
  Timer* temps;
};

#endif // __POSITION_H_
