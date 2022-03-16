#include "etrc_info.h"

#include <math.h>

#include "app.h"

Luminous::Luminous(SensorIo* sensor_io)
    : color_(kInvalidColor), hsv_({0, 0, 0}), sensor_io_(sensor_io) {
  SetColorReference(kGreen, (Hsv){120, 0, 0});
  SetColorReference(kBlack, (Hsv){240, 30, 60});
  SetColorReference(kRed, (Hsv){0, 0, 0});
  SetColorReference(kYellow, (Hsv){50, 0, 0});
  SetColorReference(kBlue, (Hsv){220, 90, 100});
  SetColorReference(kInvalidColor, (Hsv){0, 0, 0});
}

void Luminous::Update() {
  UpdateHsv();
  UpdateColor();
}

void Luminous::SetColorReference(Color c, Hsv hsv) {
  color_ref_[c] = hsv;
}

void Luminous::UpdateHsv() {
  rgb_raw_t val = sensor_io_->color_rgb_raw_;
  float r = static_cast<float>(val.r);
  float g = static_cast<float>(val.g);
  float b = static_cast<float>(val.b);

  float max = r > g ? r : g;
  max = max > b ? max : b;
  float min = r < g ? r : g;
  min = min < b ? min : b;
  float c = max - min;

  float h;
  if (c == 0) {
    h = -1;
  } else if (max == r) {
    h = fmodf(((g - b) / c), 6);
  } else if (max == g) {
    h = ((b - r) / c) + 2;
  } else if (max == b) {
    h = ((r - g) / c) + 4;
  } else {
    h = -1;
  }

  if (h != -1) {
    h = 60 * h;
  }

  float s;
  if (max == 0) {
    s = 0;
  } else {
    s = c / max;
  }

  float v = max;
  if (v > 100) {
    v = 100;
  }

  hsv_.h = h;
  hsv_.s = s * 100;
  hsv_.v = v;

  // char str[32];
  // sprintf(str, "%f, %f, %f\n ", h, s*100, v);
  // syslog(LOG_NOTICE, str);
}

void Luminous::UpdateColor() {
  // float sat_white = color_ref_[kWhite].s;
  float hue_black = color_ref_[kBlack].h;
  float sat_black = color_ref_[kBlack].s;
  float val_black = color_ref_[kBlack].v;
  float hue_green = color_ref_[kGreen].h;
  float hue_red = color_ref_[kRed].h;
  float hue_blue = color_ref_[kBlue].h;
  float sat_blue = color_ref_[kBlue].s;
  float val_blue = color_ref_[kBlue].v;
  float hue_yellow = color_ref_[kYellow].h;

  // if (sat_white - 20 < hsv_.s && hsv_.s < sat_white + 20) {
  //   color_ = kWhite;
  // } else {
  if (hue_black - 10 <= hsv_.h && hsv_.h <= hue_black + 10 && sat_black - 10 <= hsv_.s && hsv_.s <= sat_black + 10 && val_black - 10 <= hsv_.v  && hsv_.v <= val_black + 10) {
    color_ = kBlack;
  } else if (hue_green - 30 < hsv_.h && hsv_.h < hue_green + 70 && hsv_.v > val_black + 10){
    color_ = kGreen;
  } else if (hsv_.h < hue_red + 20 || 360 - 20 < hsv_.h) {
    color_ = kRed;
  } else if (hue_blue - 10 <= hsv_.h && hsv_.h <= hue_blue + 40 && sat_blue - 20 <= hsv_.s && hsv_.s <= sat_blue + 20 && val_blue - 15 <= hsv_.v  && hsv_.v <= val_blue) {
    color_ = kBlue;
  } 
  // else if (hue_blue - 10 <= hsv_.h && hsv_.h <= hue_blue + 40 && sat_blue - 65 <= hsv_.s && hsv_.s <= sat_blue -40 && val_blue - 15 <= hsv_.v  && hsv_.v <= val_blue) {
  // color_ = kBlue;
  // } 
  else if (hue_yellow - 30 < hsv_.h && hsv_.h < hue_yellow + 30) {
    color_ = kYellow;
  } else {
    color_ = kInvalidColor;
  }
}

Localize::Localize(MotorIo* motor_io)
    : pose_({0, 0, 0}), distance_(0), motor_io_(motor_io),
      radius_(45), tread_(140) {
}

void Localize::Update() {
  static int32_t prev_counts_l_ = 0;
  static int32_t prev_counts_r_ = 0;

  int32_t counts_l = motor_io_->counts_l_;
  int32_t counts_r = motor_io_->counts_r_;

  double dPhiL = (counts_l - prev_counts_l_) * M_PI / 180;
  double dPhiR = (counts_r - prev_counts_r_) * M_PI / 180;

  double dLL = radius_ * dPhiL;
  double dLR = radius_ * dPhiR;
  double dL = (dLR + dLL) / 2;
  double dtheta = (dLR - dLL) / tread_;

  // dtheta = dtheta >= 0 ? dtheta : -dtheta;

  pose_.x += dL * cos(pose_.theta + dtheta / 2);
  pose_.y += dL * sin(pose_.theta + dtheta / 2);
  pose_.theta += dtheta;
  //  * 180 / M_PI;

  prev_counts_l_ = counts_l;
  prev_counts_r_ = counts_r;

  distance_ = ((counts_l + counts_r) / 2.0) * M_PI / 180 * radius_;
}
