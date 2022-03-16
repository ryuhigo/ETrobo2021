#ifndef ETRC21_UTILS_H_
#define ETRC21_UTILS_H_

#include "info_type.h"

class PidControl {
 public:
  PidControl();
  void SetGain(float kp, float ki, float kd);
  float GetMv(Hsv target_val, Hsv current_val);

 private:
  float kp_;
  float ki_;
  float kd_;
  float dt_;
  float diff0_;
  float diff1_;
  float integral_;
};

#endif  // ETRC21_UTILS_H_
