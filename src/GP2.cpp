#include "GP2.hpp"

#define GP2_COEF (1.0f / 0.93f)

// 1.0  -> 6 cm
// 0.4  -> 20 cm
// 0.72 -> 10 cm
// O.19 -> 50 cm

GP2::GP2() {}
GP2::GP2(AnalogIn *_input, float _distance_seuil) {
  input = _input;
  distance_seuil = _distance_seuil;
}

int GP2::detected() { return input->read() * GP2_COEF > distance_seuil; }

float GP2::get_Distance() { return (input->read() * GP2_COEF); }

void GP2::debug() { printf("\nGP2 : %f, %f", input->read(), get_Distance()); }

/*
 * Return the id of the first GP2 that is triggered
 * or 0 else.
 * */
int detected_all(GP2 gp2_list[], int gp2_nb) {
  for (int i = 0; i < gp2_nb; i++) {
    if (gp2_list[i].detected())
      return i + 1;
  }
  return 0;
}