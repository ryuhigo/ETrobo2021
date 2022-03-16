#include "game_play.h"

#include <math.h>
#include <string.h>
#include <limits>
#include <algorithm>

#include "ev3api.h"

const int kMax = std::numeric_limits<int>::max();
const int kRouteCharNum = 16;

ParamMaker::ParamMaker(bool is_Rcourse) {
  JudgeTraceEdge(is_Rcourse);
}

void ParamMaker::JudgeTraceEdge(bool is_Rcourse) {
  if (is_Rcourse)
    is_RightEdge_ = true;
  else
    is_RightEdge_ = false;
}

DrivingParam ParamMaker::MakeForward(End end_type, Color end_color, float end_threshold) {
  DrivingParam p = {kGoForward, 40, {}, end_type, end_color, end_threshold};
  return p;
}

DrivingParam ParamMaker::MakePlaceForward2Center() {
  DrivingParam p = {kGoForward, 15, {}, kDistanceEndB, kInvalidColor, 90};
  return p;
}

DrivingParam ParamMaker::MakeForward2Center(End end_type, Color end_color, bool is_carrying, bool slowdown) {
  int8_t ref_power = 80;
  float end_threshold = 54;
  if (slowdown) {
    end_threshold  = 20;
    ref_power = 80;
  } else if (is_carrying) {
    end_threshold  = 30;
    ref_power = 50;
  }
  DrivingParam p = { kGoForward, ref_power, {}, end_type, end_color, end_threshold };
  return p;
}

DrivingParam ParamMaker::MakeForwardOutCircle(End end_type, Color end_color, bool is_carrying, bool slowdown) {
  int8_t ref_power = 80;
  float end_threshold = 25;
  if (is_carrying ) {
    end_threshold = 5;
    ref_power = 50;
  } else if (slowdown) {
    end_threshold = 30;
  }
  DrivingParam p = { kGoForward, ref_power, {}, end_type, end_color, end_threshold };
  return p;
}

DrivingParam ParamMaker::MakeForwardPassCircle(End end_type, Color end_color, bool is_carrying, bool slowdown) {
  int8_t ref_power = 80;
  float end_threshold = 140;
  if (is_carrying || slowdown) {
    end_threshold = 140;
    ref_power = 50;
  }
  DrivingParam p = { kGoForward, ref_power, {}, end_type, end_color, end_threshold };
  return p;
}

DrivingParam ParamMaker::MakeBackward(End end_type, Color end_color, float end_threshold) {
  DrivingParam p = { kGoBackward, 20, {}, end_type, end_color, end_threshold };
  return p;
}

DrivingParam ParamMaker::MakeFirstRightLineTrace(End end_type, Color end_color, float end_threshold, bool is_carrying, bool slowdown) {
  DrivingParam p = { kTraceRightEdge, 25, { 0.2, 0, 0.04 }, end_type, end_color, 0 };
  return p;
}

DrivingParam ParamMaker::MakeRightLineTrace(End end_type, Color end_color, float end_threshold, bool is_carrying, bool slowdown) {
  int8_t ref_power = 80;
  Gain gain = { 0.15, 0, 0.04 };
  if (slowdown) {
    ref_power = 50;
    gain = { 0.15, 0, 0.04 };
  } else if (is_carrying) {
    ref_power = 50;
    gain = { 0.18, 0, 0.04 };
  }

  DrivingParam p = { kTraceRightEdge, ref_power, gain, end_type, end_color, end_threshold };
  return p;
}

DrivingParam ParamMaker::MakeLeftLineTrace(End end_type, Color end_color, float end_threshold, bool is_carrying, bool slowdown) {
  int8_t ref_power = 80;
  Gain gain = { 0.15, 0, 0.04 };
  if (slowdown) {
    ref_power = 50;
    gain = { 0.15, 0, 0.04 };
  } else if (is_carrying) {
    ref_power = 50;
    gain = { 0.18, 0, 0.04 };
  }
  DrivingParam p = { kTraceLeftEdge, ref_power, gain, end_type, end_color, end_threshold };
  return p;
}

DrivingParam ParamMaker::MakeRotateLeft(double theta, bool is_carrying, bool slowdown) {
  int8_t ref_power;
  Move move_type;
  End end_type;

  if (is_carrying ) {
    if (theta > M_PI/2) {
      theta *= 1.3;
      move_type = kRotateBlockRight;
      ref_power = 50;
      end_type = kThetaEndB;
    } else {
    theta *= 0.9;
    ref_power = 40;
    move_type = kRotateBlockLeft;
    is_RightEdge_ = true;
    end_type = kThetaEnd;
    }
  } else {
    is_RightEdge_ = false;
    if (fabs(theta) > M_PI/2) {
      theta *= 0.95;
      ref_power = 30;
    } else if (fabs(theta) > M_PI/4) {
      theta *= 0.8;
      ref_power = 50;
    } else {
      theta *= 0.75;
      ref_power = 50;
    }
    
    move_type = kRotateLeft;
    end_type = kThetaEnd;

  }

  DrivingParam p = { move_type, ref_power, { }, kThetaEnd, kInvalidColor, static_cast<float>(theta) };

  return p;
}

DrivingParam ParamMaker::MakeRotateRight(double theta, bool is_carrying, bool slowdown) {
  int8_t ref_power;
  Move move_type;
  End end_type;

  if (is_carrying) {
    if (theta < -M_PI/2) {
      theta *= 1.3;
      move_type = kRotateBlockRight;
      ref_power = 50;
      end_type = kThetaEndB;
    } else {
    is_RightEdge_ = false;
    theta *= 0.91;
    ref_power = 40;
    move_type = kRotateBlockRight;
    end_type = kThetaEnd;
    }
  } else {
    is_RightEdge_ = true;
    if (fabs(theta) > M_PI/2) {
      theta *= 0.90;
      ref_power = 30;
    } else if (fabs(theta) > M_PI/4) {
      theta *= 0.76;
    } else {
      theta *= 0.75;
    }
    ref_power = 50;
    move_type = kRotateRight;
    end_type = kThetaEnd;


  }

  DrivingParam p = { move_type, ref_power, { }, kThetaEnd, kInvalidColor, static_cast<float>(theta) };

  return p;
}

DrivingParam ParamMaker::MakeRotateLeft(double theta) {
  int8_t ref_power = 20;
  if (fabs(theta) > M_PI/2) {
    ref_power = 8;
  }
  DrivingParam p = { kRotateLeft, ref_power, { }, kThetaEnd, kInvalidColor, static_cast<float>(theta*0.9) };
  is_RightEdge_ = false;
  return p;
}

DrivingParam ParamMaker::MakeRotateRight(double theta) {
  int8_t ref_power = 20;
  if (fabs(theta) > M_PI/2) {
    ref_power = 8;
  }
  DrivingParam p = { kRotateRight, ref_power, { }, kThetaEnd, kInvalidColor, static_cast<float>(theta*0.9) };
  is_RightEdge_ = true;
  return p;
}

DrivingParam ParamMaker::MakeRotateBlockBLeft() {
  DrivingParam p = { kRotateBlockBLeft, 50, { }, kThetaEnd, kInvalidColor, 2.1*1.38 };
  return p;
}

DrivingParam ParamMaker::MakeRotateBlockBRight() {
  DrivingParam p = { kRotateBlockBRight, 50, { }, kThetaEnd, kInvalidColor, 2.1*1.38 };
  return p;
}

ParamStore::ParamStore(BingoArea* bingo_area, RouteStore* route_store, ParamMaker* param_maker)
    : bingo_area_(bingo_area), route_store_(route_store), param_maker_(param_maker),
      is_wayback_(false), is_wayback_after_(false), is_first_RotateRight_(true), Rtransport(false), Ltransport(false){
}

bool ParamStore::CheckCarryingRoute(const char* route) {
  if (strchr(route, '1') != NULL ||
      strchr(route, '2') != NULL ||
      strchr(route, '3') != NULL ||
      strchr(route, '4') != NULL ||
      strchr(route, '5') != NULL ||
      strchr(route, '6') != NULL ||
      strchr(route, '7') != NULL ||
      strchr(route, '8') != NULL ||
      strchr(route, '9') != NULL ) {
    return true;
  } else {
    return false;
  }
}

double ParamStore::LimitRotationAngle(double angle) {
  double ret_angle = angle;
  if (fabs(angle) > M_PI) {
    if (angle >= 0)
      ret_angle =  angle - 2*M_PI;
    else
      ret_angle =  angle + 2*M_PI;
  }
  return ret_angle;
}


void ParamStore::AddTraceParam(Robot* robot, Circle* next_circle, Direction next_direction, bool is_carrying) {
  // bool slowdown = (next_circle->block != NULL) ? true : false;
  bool slowdown = false;
  // ブロック運搬直後の動き
  if (is_wayback_after_) {
    is_wayback_after_ = false;
    // 回転が必要なとき
    if (robot->direction != next_direction) {
      double from = static_cast<int>(robot->direction) * M_PI / 4;
      double to = static_cast<int>(next_direction) * M_PI / 4;
      double dtheta = LimitRotationAngle(to - from);
      // 左回転のとき
      if (dtheta > 0) {

        if (dtheta > M_PI/2){
          driving_params_.push_back(param_maker_->MakeRotateLeft(dtheta, is_carrying, slowdown));
        } else {
          driving_params_.push_back(param_maker_->MakeRotateLeft(dtheta, is_carrying, slowdown));
        }

        bool slowdown = (next_circle->block != NULL) ? true : false;
        next_circle->block = NULL;
        driving_params_.push_back(param_maker_->MakeForwardOutCircle(kDistanceEnd, kInvalidColor, is_carrying, slowdown));
        switch (next_circle->color) {
          case 'R':
            driving_params_.push_back(param_maker_->MakeLeftLineTrace(kColorEnd, kRed, 0, is_carrying, slowdown));
            break;
          case 'G':
            driving_params_.push_back(param_maker_->MakeLeftLineTrace(kColorEnd, kGreen, 0, is_carrying, slowdown));
            break;
          case 'B':
            driving_params_.push_back(param_maker_->MakeLeftLineTrace(kColorEnd, kBlue, 0, is_carrying, slowdown));
            break;
          case 'Y':
            driving_params_.push_back(param_maker_->MakeLeftLineTrace(kDistanceEnd, kInvalidColor, -50, is_carrying, slowdown));
            driving_params_.push_back(param_maker_->MakeLeftLineTrace(kColorEnd, kYellow, 0, is_carrying, slowdown));
            break;

          default:
            break;
        }
      // 右回転のとき
      } else { 
        if (dtheta < -M_PI/2) {
          driving_params_.push_back(param_maker_->MakeRotateRight(dtheta, is_carrying, slowdown));
        } else {
          driving_params_.push_back(param_maker_->MakeRotateRight(dtheta, is_carrying, slowdown));
        }
        bool slowdown = (next_circle->block != NULL) ? true : false;
        next_circle->block = NULL;
        driving_params_.push_back(param_maker_->MakeForwardOutCircle(kDistanceEnd, kInvalidColor, is_carrying, slowdown));
        switch (next_circle->color) {
          case 'R':
            driving_params_.push_back(param_maker_->MakeRightLineTrace(kColorEnd, kRed, 0, is_carrying, slowdown));
            break;
          case 'G':
            driving_params_.push_back(param_maker_->MakeRightLineTrace(kColorEnd, kGreen, 0, is_carrying, slowdown));
            break;
          case 'B':
            driving_params_.push_back(param_maker_->MakeRightLineTrace(kColorEnd, kBlue, 0, is_carrying, slowdown));
            break;
          case 'Y':
            driving_params_.push_back(param_maker_->MakeRightLineTrace(kColorEnd, kYellow, 0, is_carrying, slowdown));
            break;

          default:
            break;

        }
      }
    }
  // 交点サークル間移動
  } else {
    // 回転が必要なとき
    if (robot->direction != next_direction) {
      double from = static_cast<int>(robot->direction) * M_PI / 4;
      double to = static_cast<int>(next_direction) * M_PI / 4;
      double dtheta = LimitRotationAngle(to - from);
      // 左回転のとき
      if (is_first_RotateRight_) {
        bool slowdown = (next_circle->block != NULL) ? true : false;
        next_circle->block = NULL;
        driving_params_.push_back(param_maker_->MakeForward2Center(kDistanceEndB, kInvalidColor, is_carrying, slowdown));
        //最初の右回転
        driving_params_.push_back(param_maker_->MakeRotateRight(dtheta, is_carrying, slowdown));
        is_first_RotateRight_ = false;
        driving_params_.push_back(param_maker_->MakeForwardOutCircle(kDistanceEnd, kInvalidColor, is_carrying, slowdown));
        driving_params_.push_back(param_maker_->MakeFirstRightLineTrace(kColorEnd, kYellow, 0, is_carrying, slowdown));
      } else {
        if (dtheta > 0) {
          driving_params_.push_back(param_maker_->MakeForward2Center(kDistanceEndB, kInvalidColor, is_carrying, slowdown));
          driving_params_.push_back(param_maker_->MakeRotateLeft(dtheta, is_carrying, slowdown));
          bool slowdown = (next_circle->block != NULL) ? true : false;
          next_circle->block = NULL;
          driving_params_.push_back(param_maker_->MakeForwardOutCircle(kDistanceEnd, kInvalidColor, is_carrying, slowdown));
          if (param_maker_->is_RightEdge_) {
              switch (next_circle->color) {
                case 'R':
                  driving_params_.push_back(param_maker_->MakeRightLineTrace(kColorEnd, kRed, 0, is_carrying, slowdown));
                  break;
                case 'G':
                  driving_params_.push_back(param_maker_->MakeRightLineTrace(kColorEnd, kGreen, 0, is_carrying, slowdown));
                  break;
                case 'B':
                  driving_params_.push_back(param_maker_->MakeRightLineTrace(kColorEnd, kBlue, 0, is_carrying, slowdown));
                  break;
                case 'Y':
                  driving_params_.push_back(param_maker_->MakeRightLineTrace(kColorEnd, kYellow, 0, is_carrying, slowdown));
                  break;

                default:
                  break;
              }
            } else {
              switch (next_circle->color) {
                case 'R':
                  driving_params_.push_back(param_maker_->MakeLeftLineTrace(kColorEnd, kRed, 0, is_carrying, slowdown));
                  break;
                case 'G':
                  driving_params_.push_back(param_maker_->MakeLeftLineTrace(kColorEnd, kGreen, 0, is_carrying, slowdown));
                  break;
                case 'B':
                  driving_params_.push_back(param_maker_->MakeLeftLineTrace(kColorEnd, kBlue, 0, is_carrying, slowdown));
                  break;
                case 'Y':
                  driving_params_.push_back(param_maker_->MakeLeftLineTrace(kColorEnd, kYellow, 0, is_carrying, slowdown));
                  break;

                default:
                  break;
              }
            }
          // switch (next_circle->color) {
          //   case 'R':
          //     driving_params_.push_back(param_maker_->MakeLeftLineTrace(kColorEnd, kRed, 0, is_carrying, slowdown));
          //     break;
          //   case 'G':
          //     driving_params_.push_back(param_maker_->MakeLeftLineTrace(kColorEnd, kGreen, 0, is_carrying, slowdown));
          //     break;
          //   case 'B':
          //     driving_params_.push_back(param_maker_->MakeLeftLineTrace(kColorEnd, kBlue, 0, is_carrying, slowdown));
          //     break;
          //   case 'Y':
          //     driving_params_.push_back(param_maker_->MakeLeftLineTrace(kColorEnd, kYellow, 0, is_carrying, slowdown));
          //     break;

          //   default:
          //     break;
          // }
      // 右回転のとき
        } else {
            //２回目以降の右回転
            driving_params_.push_back(param_maker_->MakeForward2Center(kDistanceEndB, kInvalidColor, is_carrying, slowdown));
            driving_params_.push_back(param_maker_->MakeRotateRight(dtheta, is_carrying, slowdown));
            driving_params_.push_back(param_maker_->MakeForwardOutCircle(kDistanceEnd, kInvalidColor, is_carrying, slowdown));
            bool slowdown = (next_circle->block != NULL) ? true : false;
            next_circle->block = NULL;
            if (param_maker_->is_RightEdge_) {
              switch (next_circle->color) {
                case 'R':
                  driving_params_.push_back(param_maker_->MakeRightLineTrace(kColorEnd, kRed, 0, is_carrying, slowdown));
                  break;
                case 'G':
                  driving_params_.push_back(param_maker_->MakeRightLineTrace(kColorEnd, kGreen, 0, is_carrying, slowdown));
                  break;
                case 'B':
                  driving_params_.push_back(param_maker_->MakeRightLineTrace(kColorEnd, kBlue, 0, is_carrying, slowdown));
                  break;
                case 'Y':
                  driving_params_.push_back(param_maker_->MakeRightLineTrace(kColorEnd, kYellow, 0, is_carrying, slowdown));
                  break;

                default:
                  break;
              }
            } else {
              switch (next_circle->color) {
                case 'R':
                  driving_params_.push_back(param_maker_->MakeLeftLineTrace(kColorEnd, kRed, 0, is_carrying, slowdown));
                  break;
                case 'G':
                  driving_params_.push_back(param_maker_->MakeLeftLineTrace(kColorEnd, kGreen, 0, is_carrying, slowdown));
                  break;
                case 'B':
                  driving_params_.push_back(param_maker_->MakeLeftLineTrace(kColorEnd, kBlue, 0, is_carrying, slowdown));
                  break;
                case 'Y':
                  driving_params_.push_back(param_maker_->MakeLeftLineTrace(kColorEnd, kYellow, 0, is_carrying, slowdown));
                  break;

                default:
                  break;
              }
            }
          }
        }     
    } else {
      //交点サークル上を通過するための直進
        driving_params_.push_back(param_maker_->MakeForwardPassCircle(kDistanceEnd, kInvalidColor, is_carrying, slowdown));
          if (param_maker_->is_RightEdge_) {
            bool slowdown = (next_circle->block != NULL) ? true : false;
            next_circle->block = NULL;
            switch (next_circle->color) {
              case 'R':
                driving_params_.push_back(param_maker_->MakeRightLineTrace(kColorEnd, kRed, 0, is_carrying, slowdown));
                break;
              case 'G':
                driving_params_.push_back(param_maker_->MakeRightLineTrace(kColorEnd, kGreen, 0, is_carrying, slowdown));
                break;
              case 'B':
                driving_params_.push_back(param_maker_->MakeRightLineTrace(kColorEnd, kBlue, 0, is_carrying, slowdown));
                break;
              case 'Y':
                driving_params_.push_back(param_maker_->MakeRightLineTrace(kColorEnd, kYellow, 0, is_carrying, slowdown));
                break;

              default:
                break;
            }
          } else {
            bool slowdown = (next_circle->block != NULL) ? true : false;
            next_circle->block = NULL;
            switch (next_circle->color) {
              case 'R':
                driving_params_.push_back(param_maker_->MakeLeftLineTrace(kColorEnd, kRed, 0, is_carrying, slowdown));
                break;
              case 'G':
                driving_params_.push_back(param_maker_->MakeLeftLineTrace(kColorEnd, kGreen, 0, is_carrying, slowdown));
                break;
              case 'B':
                driving_params_.push_back(param_maker_->MakeLeftLineTrace(kColorEnd, kBlue, 0, is_carrying, slowdown));
                break;
              case 'Y':
                driving_params_.push_back(param_maker_->MakeLeftLineTrace(kColorEnd, kYellow, 0, is_carrying, slowdown));
                break;

              default:
                break;
            }
          }
    }
  }

  robot->direction = next_direction;
  robot->circle = next_circle;
}

void ParamStore::AddPlaceParam(Robot* robot, Circle* next_circle, Direction next_direction, bool is_carrying) {
  // bool slowdown = (next_circle->block != NULL) ? true : false;
  bool slowdown = true;
  if (is_wayback_) {
    //設置後後退
    if (Rtransport) {
      driving_params_.push_back(param_maker_->MakeRotateBlockBRight());
      // driving_params_.push_back(param_maker_->MakeForward(kDistanceEndB, kInvalidColor, 50));
      Rtransport = false;
    } else if(Ltransport){
      driving_params_.push_back(param_maker_->MakeRotateBlockBLeft());
      // driving_params_.push_back(param_maker_->MakeForward(kDistanceEndB, kInvalidColor, 50));
      Rtransport = false;
    } else {
      switch (next_circle->color) {
        case 'R':
          driving_params_.push_back(param_maker_->MakeBackward(kDistanceEnd, kInvalidColor, -50));
          driving_params_.push_back(param_maker_->MakeBackward(kColorEndB, kRed, 0));
          break;
        case 'G':
          driving_params_.push_back(param_maker_->MakeBackward(kColorEndB, kGreen, 0));
          break;
        case 'B':
          driving_params_.push_back(param_maker_->MakeBackward(kColorEndB, kBlue, 0));
          break;
        case 'Y':
          driving_params_.push_back(param_maker_->MakeBackward(kColorEndB, kYellow, 0));
          break;
        
        default:
          break;
        
      }
    }
    robot->circle = next_circle;
    is_wayback_ = false;
    is_wayback_after_ = true;
  } else {
    //設置動作
    driving_params_.push_back(param_maker_->MakePlaceForward2Center());

    if (robot->direction != next_direction) {
      double from = static_cast<int>(robot->direction) * M_PI / 4;
      double to = static_cast<int>(next_direction) * M_PI / 4;
      double dtheta = LimitRotationAngle(to - from);
      if (dtheta< M_PI/2 && dtheta > 0) {
        driving_params_.push_back(param_maker_->MakeRotateLeft(dtheta));

        driving_params_.push_back(param_maker_->MakeForward(kDistanceEndB, kInvalidColor, 57));
        switch (next_circle->color) {
          case 'R':
            driving_params_.push_back(param_maker_->MakeForward(kColorEndB, kRed, 0));
            break;
          case 'G':
            driving_params_.push_back(param_maker_->MakeForward(kColorEndB, kGreen, 0));
            break;
          case 'B':
            driving_params_.push_back(param_maker_->MakeForward(kColorEndB, kBlue, 0));
            break;
          case 'Y':
            driving_params_.push_back(param_maker_->MakeForward(kColorEndB, kYellow, 0));
            
            break;
          default:
            break;
        }
        robot->direction = next_direction;
      } else if (dtheta > M_PI/2){
        driving_params_.push_back(param_maker_->MakeRotateLeft(dtheta, is_carrying, slowdown));
        Ltransport = true;
      } else if (dtheta < 0 && dtheta > -M_PI/2){
        driving_params_.push_back(param_maker_->MakeRotateRight(dtheta));

        driving_params_.push_back(param_maker_->MakeForward(kDistanceEndB, kInvalidColor, 57));
        switch (next_circle->color) {
          case 'R':
            driving_params_.push_back(param_maker_->MakeForward(kColorEndB, kRed, 0));
            break;
          case 'G':
            driving_params_.push_back(param_maker_->MakeForward(kColorEndB, kGreen, 0));
            break;
          case 'B':
            driving_params_.push_back(param_maker_->MakeForward(kColorEndB, kBlue, 0));
            break;
          case 'Y':
            driving_params_.push_back(param_maker_->MakeForward(kColorEndB, kYellow, 0));
            
            break;
          default:
            break;
        }
        robot->direction = next_direction;
      } else {
        driving_params_.push_back(param_maker_->MakeRotateRight(dtheta, is_carrying, slowdown));
        Rtransport = true;
      }
    }
    robot->circle = next_circle;
    is_wayback_ = true;
  }
}

bool ParamStore::GenerateParam() {
  static bool is_entry = true;
  if (is_entry) {
    bingo_area_->InitRobot();
    bingo_area_->ResetBlockPos();
    is_entry = false;
  }

  std::list<char*>& routes = route_store_->routes_;
  if (routes.empty())
    return true;

  Robot* robot = &bingo_area_->robot_;
  char* route = routes.front();
  bool is_carrying = CheckCarryingRoute(route);
  routes.pop_front();
  for (size_t i = 0; i < strlen(route); ++i) {
    Circle* next_circle = bingo_area_->SameIdCircle(route[i]);
    if (robot->circle->id == next_circle->id)
      continue;
    

    Direction next_direction = bingo_area_->DirectionToGo(robot->circle, next_circle);
    switch (next_direction) {
      case kEast:
      case kNorth:
      case kWest:
      case kSouth:
        AddTraceParam(robot, next_circle, next_direction, is_carrying);
        break;

      case kNorthEast:
      case kNorthWest:
      case kSouthWest:
      case kSouthEast:
        AddPlaceParam(robot, next_circle, next_direction, is_carrying);
        break;

      default:
        break;
    }
  }
  return false;
}

RouteStore::RouteStore(BingoArea* bingo_area, RouteSearch* route_search)
    : bingo_area_(bingo_area), route_search_(route_search) {
}

void RouteStore::SaveMovingRoute(Circle* goal_circle) {
  Circle* curr_circle = bingo_area_->robot_.circle;
  Circle* back_circle = NULL;
  // char str[32] = {};
  //test_str.push_back(&goal_circle->id);

  char str[kRouteCharNum] = {};
  for (int i = 0; i < kRouteCharNum - 2; ++i) {
    str[i] = curr_circle->id;
    back_circle = curr_circle;
    curr_circle = curr_circle->prev;
    if (curr_circle->id == goal_circle->id) {
      str[i + 1] = curr_circle->id;
      str[i + 2] = '\0';
      break;
    }
  }
  char* route = new char(strlen(str) + 1);
  strcpy(route, str);
  routes_.push_back(route);
  syslog(LOG_NOTICE, route);

  if (goal_circle->id == '0')
    route_search_->reverse_circle_ = NULL;
  else
    route_search_->reverse_circle_ = back_circle;
}

void RouteStore::SaveCarryRoute(Circle* goal_circle) {
  Circle* curr_circle = bingo_area_->robot_.circle;
  Circle* back_circle = NULL;

  char str[kRouteCharNum] = {};
  for (int i = 0; i < kRouteCharNum - 3; ++i) {
    str[i] = curr_circle->id;
    back_circle = curr_circle;
    curr_circle = curr_circle->prev;
    if (curr_circle->id == goal_circle->id) {
      str[i + 1] = curr_circle->id;
      str[i + 2] = back_circle->id;
      str[i + 3] = '\0';
      break;
    }
  }
  char* route = new char(strlen(str) + 1);
  strcpy(route, str);
  routes_.push_back(route);
  syslog(LOG_NOTICE, route);


}

RouteSearch::RouteSearch(BingoArea* bingo_area)
    : reverse_circle_(NULL), bingo_area_(bingo_area) {
}

void RouteSearch::ResetRouteSearchInfo() {
  for (int i = 0; i < kCircleNum; ++i) {
    Circle* circle = &bingo_area_->circles_[i];
    circle->prev = NULL;
    circle->queue_added = false;

    if ('1' <= circle->id && circle->id <= '9')
      circle->cost = kMax;
    else if (circle->block != NULL)
      circle->cost = kMax;
    else
      circle->cost = -1;
  }

  if (reverse_circle_ != NULL)
    reverse_circle_->cost = kMax;
}

bool RouteSearch::CalcMovingRoute(Circle* goal_circle) {
  static bool is_entry = true;
  if (is_entry) {
    ResetRouteSearchInfo();
    queue_.push_back(goal_circle);
    is_entry = false;
  }

  Robot* robot = &bingo_area_->robot_;
  if (!queue_.empty()) {
    Circle* curr_circle = queue_.front();
    queue_.pop_front();
    if (curr_circle->id == robot->circle->id) {
      queue_.clear();
      is_entry = true;
      return true;
    }
    for (int i = 0; i < curr_circle->next_num; ++i) {
      Circle* next_circle = curr_circle->next[i];
      if (next_circle->queue_added)
        continue;

      if (next_circle->cost == -1 || next_circle->id == robot->circle->id) {
        next_circle->queue_added = true;
        next_circle->prev = curr_circle;
        queue_.push_back(next_circle);
      }
    }
  }
  return false;
}

void RouteSearch::MoveRobot(Circle* goal_circle, bool stepback) {
  Robot* robot = &bingo_area_->robot_;
  if (stepback) {
    Circle* curr_circle = robot->circle;
    Circle* back_circle = NULL;
    for (int i = 0; i < kCircleNum; ++i) {
      back_circle = curr_circle;
      curr_circle = curr_circle->prev;
      if (curr_circle->id == goal_circle->id) {
        break;
      }
    }
    robot->circle = back_circle;
  } else {
    robot->circle = goal_circle;
  }
}

void RouteSearch::CompleteCarryBlock(Block* block) {
  Circle* before_circle = block->circle;
  Circle* after_circle = block->target;

  before_circle->block = NULL;
  after_circle->block = block;

  block->circle = block->target;
  block->carrying_completed = true;
}

BlockDecision::BlockDecision(BingoArea* bingo_area, BingoState* bingo_state)
    : bingo_area_(bingo_area), bingo_state_(bingo_state), carried_blocks_num_(0) {
}

Block* BlockDecision::NextCarryBlock() {
  Block* next_carry_block;

  if (carried_blocks_num_ == 0)
    next_carry_block = Select1stBlock();
  else if (carried_blocks_num_ == 1)
    next_carry_block = Select2ndBlock();
  else if (2 <= carried_blocks_num_ && carried_blocks_num_ <= 7)
    next_carry_block = Select3rdTo8thBlock();
  else if (carried_blocks_num_ == 8)
    next_carry_block = SelectBlackBlock();
  else
    next_carry_block = NULL;

  ++carried_blocks_num_;

  return next_carry_block;
}

Block* BlockDecision::Select1stBlock() {
  Robot* robot = &bingo_area_->robot_;
  Block* first_block = NULL;

  double min = std::numeric_limits<double>::infinity();
  const double kDecisionCoeff = 1.5;

  for (int i = 0; i < kBlockNum; ++i) {
    Block* block = &bingo_area_->blocks_[i];
    if (block->carrying_completed)
      continue;

    double d = bingo_area_->DistanceBtwCircles(robot->circle, block->circle);
    if (robot->circle->x != block->circle->x &&
        robot->circle->y != block->circle->y) {
      d *= kDecisionCoeff;
    }

    if (min > d) {
      first_block = block;
      min = d;
    }
  }
  return first_block;
}

Block* BlockDecision::Select2ndBlock() {
  Block* second_block = NULL;

  double min = std::numeric_limits<double>::infinity();

  for (int i = 0; i < kBlockNum; ++i) {
    Block* block = &bingo_area_->blocks_[i];
    if (block->carrying_completed)
      continue;

    double d = bingo_area_->DistanceFromCenter(block->circle);
    if (min > d) {
      second_block = block;
      min = d;
    }
  }

  return second_block;
}

Block* BlockDecision::Select3rdTo8thBlock() {
  Robot* robot = &bingo_area_->robot_;
  Block* third_to_eighth_block = NULL;

  for (int i = 0; i < kBlockNum; ++i) {
    Block* block = &bingo_area_->blocks_[i];
    if (block->carrying_completed)
      continue;

    double d = bingo_area_->DistanceFromCenter(block->circle);
    if (3 > d) {
      third_to_eighth_block = block;
      return third_to_eighth_block;
    }
  }

  double min = std::numeric_limits<double>::infinity();
  bingo_state_->Update();

  for (int i = 0; i < kBlockNum; ++i) {
    Block* block = &bingo_area_->blocks_[i];
    if (block->carrying_completed)
      continue;

    if (bingo_state_->IsGoodCandBlock(block)) {
      double d = bingo_area_->DistanceBtwCircles(robot->circle, block->circle)
               + bingo_area_->DistanceBtwCircles(block->circle, block->target);
      if (min > d) {
        third_to_eighth_block = block;
        min = d;
      }
    }
  }

  return third_to_eighth_block;
}

Block* BlockDecision::SelectBlackBlock() {
  Block* black_block = NULL;

  for (int i = 0; i < kBlockNum; ++i) {
    Block* block = &bingo_area_->blocks_[i];
    if (block->color == 'K')
      black_block = block;
  }

  return black_block;
}

BingoAgent::BingoAgent(bool is_Rcourse)
    : calc_completed_(false), is_Rcourse_(is_Rcourse),
      curr_step_(kDecideCarryBlock), carry_block_(NULL) {
  bingo_area_ = new BingoArea(is_Rcourse_);
  bingo_state_ = new BingoState(bingo_area_);
  block_decision_ = new BlockDecision(bingo_area_, bingo_state_);
  route_search_ = new RouteSearch(bingo_area_);
  route_store_ = new RouteStore(bingo_area_, route_search_);
  param_maker_ = new ParamMaker(is_Rcourse);
  param_store_ = new ParamStore(bingo_area_, route_store_, param_maker_);
}

BingoAgent::~BingoAgent() {
  delete param_store_;
  delete param_maker_;
  delete route_store_;
  delete route_search_;
  delete block_decision_;
  delete bingo_state_;
  delete bingo_area_;
}

void BingoAgent::SetBlockPos(BlockId block_id, char circle_id) {
  bingo_area_->SetBlockPos(block_id, circle_id);
}

void BingoAgent::UpdateBlockTarget() {
  bingo_area_->UpdateBlockTarget();
}

void BingoAgent::TakeOneStep() {
  // char str[32];
  switch (curr_step_) {
    case kDecideCarryBlock:
      DecideCarryBlock();
      break;

    case kSearchMovingRoute:
      SearchMovingRoute();
      break;

    case kSearchCarryRoute:
      SearchCarryRoute();
      break;

    case kGenerateDrivingParam:
      GenerateDrivingParam();
      break;

    default:
      calc_completed_ = true;
      break;
  }
}

void BingoAgent::DecideCarryBlock() {
  carry_block_ = block_decision_->NextCarryBlock();

  if (carry_block_ == NULL)
    curr_step_ = kGenerateDrivingParam;
  else
    curr_step_ = kSearchMovingRoute;
}

void BingoAgent::SearchMovingRoute() {
  bool is_found = route_search_->CalcMovingRoute(carry_block_->circle);

  if (is_found) {
    route_store_->SaveMovingRoute(carry_block_->circle);
    route_search_->MoveRobot(carry_block_->circle, false);
    curr_step_ = kSearchCarryRoute;
  }
}

void BingoAgent::SearchCarryRoute() {
  bool is_found = route_search_->CalcMovingRoute(carry_block_->target);

  if (is_found) {
    route_store_->SaveCarryRoute(carry_block_->target);
    route_search_->MoveRobot(carry_block_->target, true);
    route_search_->CompleteCarryBlock(carry_block_);
    curr_step_ = kDecideCarryBlock;
  }
}

void BingoAgent::GenerateDrivingParam() {
  bool generate_completed = param_store_->GenerateParam();
  
  if (generate_completed){
    curr_step_ = kBingoCompleted;

  }
}
