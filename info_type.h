#ifndef ETRC21_INFO_TYPE_H_
#define ETRC21_INFO_TYPE_H_

#include <stdint.h>

enum Color {
  kGreen = 0,
  kBlack,
  kRed,
  kYellow,
  kBlue,
  kWhite,
  kInvalidColor,
  kColorNum
};

enum Move {
  kTraceLeftEdge = 0,
  kTraceRightEdge,
  kTraceLeftBlue,
  kTraceRightBlue,
  kGoForward,
  kGoBackward,
  kRotateLeft,
  kRotateRight,
  kRotateBlockLeft,
  kRotateBlockRight,
  kRotateBlockBLeft,
  kRotateBlockBRight,
  kStop,
  kInvalidMove,
  kMoveNum
};

enum End {
  kColorEnd = 0,
  kColorEndB,
  kDistanceEnd,
  kDistanceEndB,
  kThetaEnd,
  kThetaEndB,
  kIvalidEnd,
  kEndNum
};

struct Hsv {
  float h;
  float s;
  float v;
};

struct Pose {
  double x;
  double y;
  double theta;
};

struct Gain {
  float kp;
  float ki;
  float kd;
};

struct DrivingParam {
  Move move_type;
  int8_t ref_power;
  Gain gain;
  End end_type;
  Color end_color;
  float end_threshold;
  bool is_started;
  bool is_finished;
};

#endif  // ETRC21_INFO_TYPE_H_
