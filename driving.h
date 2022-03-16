#ifndef ETRC21_DRIVING_H_
#define ETRC21_DRIVING_H_

#include "device_io.h"
#include "etrc_info.h"
#include "utils.h"

#include <list>

class WheelsControl {
 public:
  WheelsControl(MotorIo* motor_io);
  void Exec(int8_t target_power_l, int8_t target_power_r);

 private:
  MotorIo* motor_io_;
};

class LineTracer {
 public:
  LineTracer(WheelsControl* wheels_control, Luminous* luminous);
  ~LineTracer();
  void SetParam(Move move_type, int8_t ref_power, Gain gain);
  void Run();
  void Stop();

 private:
  WheelsControl* wheels_control_;
  Luminous* luminous_;
  Move move_type_;
  int8_t ref_power_;
  float mv;
  PidControl* pid_control_;
};

class BasicMover {
 public:
  BasicMover(WheelsControl* wheels_control);
  ~BasicMover();
  void SetParam(Move move_type, int8_t ref_power);
  void Run();
  void Stop();

 private:
  WheelsControl* wheels_control_;
  Localize* localize_;
  Move move_type_;
  int8_t ref_power_;
};

class EndCondition {
 public:
  EndCondition(Luminous* luminous, Localize* localize, WheelsControl* wheels_control);
  void SetParam(End end_type, Color end_color, float end_threshold);
  bool IsSatisfied();

 private:
  Luminous* luminous_;
  Localize* localize_;
  WheelsControl* wheels_control_;
  End end_type_;
  Color end_color_;
  float end_threshold_;
  bool end_state_;
  float ref_distance_;
  float ref_theta_;
};

class DrivingManager {
 public:
  DrivingManager(LineTracer* line_tracer, BasicMover* basic_mover, EndCondition* end_condition);
  void Update();
  void AddDrivingParam(DrivingParam param);
  bool DrivingParamsEmpty();

 private:
  void SetMoveParam(DrivingParam& param);
  void SetEndParam(DrivingParam& param);
  void Drive(DrivingParam& param);
  LineTracer* line_tracer_;
  BasicMover* basic_mover_;
  EndCondition* end_condition_;
  std::list<DrivingParam> driving_params_;
};

#endif  // ETRC21_DRIVING_H_
