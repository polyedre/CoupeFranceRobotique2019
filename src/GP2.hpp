#ifndef __GP2_H_
#define __GP2_H_

bool detected_all(GP2 gp2_list[], int gp2_nb);

class GP2 {
public:
  GP2(AnalogIn input, float distance);

  bool detected();
  float get_Distance();
  void debug();

private:
  AnalogIn input;
  float distance_seuil;
};

#endif // __GP2_H_
