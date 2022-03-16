#include "utils.h"

#include "app.h"

PidControl::PidControl()
    : kp_(0), ki_(0), kd_(0), dt_(EXEC_ACTION_DT_MS/1000.0),
      diff0_(0), diff1_(0), integral_(0) {
}

void PidControl::SetGain(float kp, float ki, float kd) {
  kp_ = kp;
  ki_ = ki;
  kd_ = kd;
}

float PidControl::GetMv(Hsv target_val, Hsv current_val) {
  float p, i, d, mv;

  diff0_ = diff1_;
  diff1_ = current_val.v - target_val.v;

  integral_ += (diff1_ + diff0_) / 2.0 * dt_;
  p = kp_ * diff1_;
  i = ki_ * integral_;
  d = kd_ * (diff1_ - diff0_) / dt_;
  mv = p + i + d;

  return mv;
}
