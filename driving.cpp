#include "driving.h"

#include <math.h>

extern Hsv gBlackVal;

WheelsControl::WheelsControl(MotorIo* motor_io) : motor_io_(motor_io) {
}

void WheelsControl::Exec(int8_t target_power_l, int8_t target_power_r) {
  if (target_power_l == 0 && target_power_r == 0) {
    motor_io_->StopWheels(true);
  } else {
    int8_t curr_power_l = motor_io_->power_l_;
  
    if (target_power_l > curr_power_l) {
      curr_power_l += 1;
    } else if (target_power_l < curr_power_l) {
      curr_power_l -= 1;
    }

    int8_t curr_power_r = motor_io_->power_r_;
    if (target_power_r > curr_power_r) {
      curr_power_r += 1;
    } else if (target_power_r < curr_power_r) {
      curr_power_r -= 1;
    }
    motor_io_->SetWheelsPower(curr_power_l, curr_power_r);
  }
}

LineTracer::LineTracer(WheelsControl* wheels_control, Luminous* luminous)
    : wheels_control_(wheels_control), luminous_(luminous),
      move_type_(kInvalidMove), ref_power_(0) {
  pid_control_ = new PidControl();
}

LineTracer::~LineTracer() {
  delete pid_control_;
}

void LineTracer::SetParam(Move move_type, int8_t ref_power, Gain gain) {
  move_type_ = move_type;
  ref_power_ = ref_power;
  pid_control_->SetGain(gain.kp, gain.ki, gain.kd);
}

void LineTracer::Run() {
  float mv = pid_control_->GetMv(gBlackVal, luminous_->hsv_);
  if (move_type_ == kTraceRightEdge) {
    mv *= -1;
  }

  int8_t power_l = static_cast<int8_t>(ref_power_ + mv);
  int8_t power_r = static_cast<int8_t>(ref_power_ - mv);
  wheels_control_->Exec(power_l, power_r);
}

void LineTracer::Stop() {
  wheels_control_->Exec(0, 0);
}

BasicMover::BasicMover(WheelsControl* wheels_control)
    : wheels_control_(wheels_control),
      move_type_(kInvalidMove), ref_power_(0) {
}

BasicMover::~BasicMover() {
}

void BasicMover::SetParam(Move move_type, int8_t ref_power) {
  move_type_ = move_type;
  ref_power_ = ref_power;
}

void BasicMover::Run() {
  int8_t power_l;
  int8_t power_r;

  if (move_type_ == kGoForward) {
    power_l = power_r = ref_power_;
  } else if (move_type_ == kGoBackward) {
    power_l = power_r = -ref_power_;
  } else if (move_type_ == kRotateLeft) {
    power_l = -ref_power_;
    power_r = ref_power_;
  } else if (move_type_ == kRotateRight) {
    power_l = ref_power_;
    power_r = -ref_power_;
  } else if (move_type_ == kRotateBlockLeft) {
    power_l = -5;
    power_r = ref_power_;
  } else if (move_type_ == kRotateBlockRight) {
    power_l = ref_power_;
    power_r = -5;
  } else if (move_type_ == kRotateBlockBLeft) {
    power_l = 5;
    power_r = -ref_power_;
  } else if (move_type_ == kRotateBlockBRight) {
    power_l = -ref_power_;
    power_r = 5; 
  } else if (move_type_ == kStop) {
    power_l = power_r = 0;
    } else {
    power_l = power_r = 0;
  }

  wheels_control_->Exec(power_l, power_r);
}

void BasicMover::Stop() {
  wheels_control_->Exec(0, 0);
}

EndCondition::EndCondition(Luminous* luminous, Localize* localize, WheelsControl* wheels_control)
    : luminous_(luminous), localize_(localize), wheels_control_(wheels_control),
      end_type_(kIvalidEnd), end_color_(kInvalidColor), end_threshold_(0),
      end_state_(false), ref_distance_(0), ref_theta_(0) {
}

void EndCondition::SetParam(End end_type, Color end_color, float end_threshold) {
  end_type_ = end_type;
  end_color_ = end_color;
  end_threshold_ = end_threshold;
  end_state_ = false;
  
  // if (end_type_ == kDistanceEnd) {
  //   ref_distance_ = localize_->distance_;
  // } else if (end_type_ == kThetaEnd) {
  //   ref_theta_ = localize_->pose_.theta;
  // }
  if (end_type_ == kDistanceEnd) {
    ref_distance_ = localize_->distance_;
  } else if (end_type_ == kDistanceEndB) {
    ref_distance_ = localize_->distance_;
  } else if (end_type_ == kThetaEnd) {
    ref_theta_ = localize_->pose_.theta;
  } else if (end_type_ == kThetaEndB) {
    ref_theta_ = localize_->pose_.theta;
  }
}

bool EndCondition::IsSatisfied() {
  switch (end_type_) {
    case kColorEnd:
      if (end_color_ == luminous_->color_){
        end_state_ = true;}
      break;

    case kColorEndB:
      if (end_color_ == luminous_->color_){
      wheels_control_->Exec(0, 0);
        end_state_ = true;}
      break;

    case kDistanceEnd:
      if (end_threshold_ >= 0) {
        if (localize_->distance_ - ref_distance_ > end_threshold_) {
          end_state_ = true;
        }
      } else {
        if (localize_->distance_ - ref_distance_ < end_threshold_) {
          end_state_ = true;
        }
      }

      break;

    case kDistanceEndB:
      if (end_threshold_ >= 0) {
        if (localize_->distance_ - ref_distance_ > end_threshold_) {
          wheels_control_->Exec(0, 0);
          end_state_ = true;
        }
      } else {
        if (localize_->distance_ - ref_distance_ < end_threshold_) {
          wheels_control_->Exec(0, 0);
          end_state_ = true;
        }
      }

      break;      

    case kThetaEnd:
      if (end_threshold_ > 0 && localize_->pose_.theta - ref_theta_ > end_threshold_) {
        wheels_control_->Exec(0, 0);
        end_state_ = true;
      } else if (end_threshold_ < 0 && localize_->pose_.theta - ref_theta_ < end_threshold_) {
        wheels_control_->Exec(0, 0);
        end_state_ = true;
      }
      break;

    case kThetaEndB:
      if (end_threshold_ > 0 && localize_->pose_.theta - ref_theta_ > end_threshold_) {
        end_state_ = true;
      } else if (end_threshold_ < 0 && localize_->pose_.theta - ref_theta_ < end_threshold_) {
        end_state_ = true;
      }
      break;

    default:
      break;
  }

  return end_state_;
}

DrivingManager::DrivingManager(LineTracer* line_tracer, BasicMover* basic_mover, EndCondition* end_condition)
    : line_tracer_(line_tracer), basic_mover_(basic_mover), end_condition_(end_condition) {
}

void DrivingManager::Update() {
  if (driving_params_.size() <= 0) {
    return;
  }

  DrivingParam& curr_param = driving_params_.front();
  if (!curr_param.is_started) {
    SetMoveParam(curr_param);
    SetEndParam(curr_param);
    curr_param.is_started = true;
  }

  Drive(curr_param);
  if (end_condition_->IsSatisfied()) {
    curr_param.is_finished = true;
  }

  if (curr_param.is_finished) {
    driving_params_.pop_front();
  }

}

void DrivingManager::AddDrivingParam(DrivingParam param) {
  driving_params_.push_back(param);
}

bool DrivingManager::DrivingParamsEmpty() {
  return driving_params_.empty();
}

void DrivingManager::SetMoveParam(DrivingParam& param) {
  Move move_type = param.move_type;
  int8_t ref_power = param.ref_power;
  Gain gain = param.gain;

  switch (move_type) {
    case kTraceLeftEdge:
    case kTraceRightEdge:
      line_tracer_->SetParam(move_type, ref_power, gain);
      break;

    case kGoForward:
    case kGoBackward:
    case kRotateLeft:
    case kRotateRight:
    case kRotateBlockLeft:
    case kRotateBlockRight:
    case kRotateBlockBLeft:
    case kRotateBlockBRight:
    case kStop:

      basic_mover_->SetParam(move_type, ref_power);
      break;

    default:
      break;
  }
}

void DrivingManager::SetEndParam(DrivingParam& param) {
  End end_type = param.end_type;
  Color end_color = param.end_color;
  float end_threshold = param.end_threshold;

  end_condition_->SetParam(end_type, end_color, end_threshold);
}

void DrivingManager::Drive(DrivingParam& param) {
  Move move_type = param.move_type;

  switch (move_type) {
    case kTraceLeftEdge:
    case kTraceRightEdge:
      line_tracer_->Run();
      break;

    case kGoForward:
    case kGoBackward:
    case kRotateLeft:
    case kRotateRight:
    case kRotateBlockLeft:
    case kRotateBlockRight:
    case kRotateBlockBLeft:
    case kRotateBlockBRight:
    case kStop:

      basic_mover_->Run();
      break;

    default:
      break;
  }
}
