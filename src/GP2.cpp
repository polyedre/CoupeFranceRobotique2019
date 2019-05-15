#include "GP2.hpp"

#define GP2_COEF 0.01f

GP2::GP2(AnalogIn _input, float _distance_seuil) {
  input = _input;
  _distance_seuil = _distance_seuil;
}

bool GP2::detected() { return input.read() < distance_seuil * GP2_COEF; }

float GP2::get_Distance() { return distance_seuil * GP2_COEF; }

void GP2::debug() { printf("\nGP2 : %d, %f", input.read(), get_Distance()); }

bool detected_all(GP2 gp2_list[], int gp2_nb) {
  for (int i = 0; i < gp2_nb; i++) {
    if (!gp2_list[i].detected())
      return true;
  }
  return false;
}
