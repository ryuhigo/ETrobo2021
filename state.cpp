#include "state.h"

const int kLcourseParamsNum = 45;
const DrivingParam kLcourseParams[kLcourseParamsNum] = {
  { kTraceRightEdge, 58, { 0.30, 0, 0.08 }, kDistanceEnd, kInvalidColor, 30 },
  { kTraceRightEdge, 68, { 0.30, 0, 0.08 }, kDistanceEnd, kInvalidColor, 50 },
  { kTraceRightEdge, 78, { 0.30, 0, 0.08 }, kDistanceEnd, kInvalidColor, 50 },
  { kTraceRightEdge, 88, { 0.30, 0, 0.08 }, kDistanceEnd, kInvalidColor, 50 },
  { kTraceRightEdge, 93, { 0.30, 0, 0.08 }, kDistanceEnd, kInvalidColor, 1100 },
  { kTraceRightEdge, 88, { 0.30, 0, 0.08 }, kDistanceEnd, kInvalidColor, 80 },
  { kTraceRightEdge, 78, { 0.32, 0, 0.08 }, kDistanceEnd, kInvalidColor, 80 },
  { kTraceRightEdge, 68, { 0.34, 0, 0.08 }, kDistanceEnd, kInvalidColor, 50 },
  { kTraceRightEdge, 60, { 0.36, 0, 0.08 }, kDistanceEnd, kInvalidColor, 50 },
  { kTraceRightEdge, 58, { 0.38, 0, 0.08 }, kDistanceEnd, kInvalidColor, 30 },
  { kTraceRightEdge, 55, { 0.40, 0, 0.08 }, kDistanceEnd, kInvalidColor, 30 },

  { kTraceRightEdge, 55, { 0.48, 0, 0.08 }, kDistanceEnd, kInvalidColor, 750 },

  { kTraceRightEdge, 55, { 0.40, 0, 0.08 }, kDistanceEnd, kInvalidColor, 30 },
  { kTraceRightEdge, 58, { 0.38, 0, 0.08 }, kDistanceEnd, kInvalidColor, 30 },
  { kTraceRightEdge, 73, { 0.36, 0, 0.08 }, kDistanceEnd, kInvalidColor, 50 },
  { kTraceRightEdge, 88, { 0.34, 0, 0.08 }, kDistanceEnd, kInvalidColor, 370 },
  { kTraceRightEdge, 73, { 0.36, 0, 0.08 }, kDistanceEnd, kInvalidColor, 50 },
  { kTraceRightEdge, 60, { 0.38, 0, 0.08 }, kDistanceEnd, kInvalidColor, 30 },
  { kTraceRightEdge, 58, { 0.40, 0, 0.08 }, kDistanceEnd, kInvalidColor, 20 },

  { kTraceRightEdge, 58, { 0.48, 0, 0.08 }, kDistanceEnd, kInvalidColor, 630 },

  { kTraceRightEdge, 58, { 0.40, 0, 0.08 }, kDistanceEnd, kInvalidColor, 30 },
  { kTraceRightEdge, 58, { 0.38, 0, 0.08 }, kDistanceEnd, kInvalidColor, 50 },
  { kTraceRightEdge, 68, { 0.36, 0, 0.08 }, kDistanceEnd, kInvalidColor, 50 },
  { kTraceRightEdge, 80, { 0.34, 0, 0.08 }, kDistanceEnd, kInvalidColor, 50 },
  { kTraceRightEdge, 88, { 0.30, 0, 0.08 }, kDistanceEnd, kInvalidColor, 50 },
  { kTraceRightEdge, 93, { 0.30, 0, 0.08 }, kDistanceEnd, kInvalidColor, 1080 },
  { kTraceRightEdge, 88, { 0.30, 0, 0.08 }, kDistanceEnd, kInvalidColor, 50 },
  { kTraceRightEdge, 80, { 0.30, 0, 0.08 }, kDistanceEnd, kInvalidColor, 50 },
  { kTraceRightEdge, 75, { 0.35, 0, 0.08 }, kDistanceEnd, kInvalidColor, 50 },
  { kTraceRightEdge, 65, { 0.38, 0, 0.08 }, kDistanceEnd, kInvalidColor, 30 },
  { kTraceRightEdge, 60, { 0.40, 0, 0.08 }, kDistanceEnd, kInvalidColor, 30 },
  { kTraceRightEdge, 55, { 0.43, 0, 0.08 }, kDistanceEnd, kInvalidColor, 30 },
  { kTraceRightEdge, 50, { 0.45, 0, 0.08 }, kDistanceEnd, kInvalidColor, 30 },

  { kTraceRightEdge, 50, { 0.58, 0, 0.08 }, kDistanceEnd, kInvalidColor, 370 },

  { kTraceRightEdge, 50, { 0.43, 0, 0.08 }, kDistanceEnd, kInvalidColor, 30 },
  { kTraceRightEdge, 60, { 0.38, 0, 0.08 }, kDistanceEnd, kInvalidColor, 50 },
  { kTraceRightEdge, 75, { 0.33, 0, 0.08 }, kDistanceEnd, kInvalidColor, 50 },
  { kTraceRightEdge, 88, { 0.30, 0, 0.08 }, kDistanceEnd, kInvalidColor, 50 },
  { kTraceRightEdge, 88, { 0.28, 0, 0.08 }, kDistanceEnd, kInvalidColor, 70 },
  { kTraceRightEdge, 90, { 0.28, 0, 0.08 }, kDistanceEnd, kInvalidColor, 430 },
  { kGoForward, 80, { }, kDistanceEnd, kInvalidColor, 250 },
  { kTraceRightEdge, 60, { 0.31, 0, 0.06 }, kDistanceEnd, kInvalidColor, 790 },
  { kGoForward, 70, { }, kDistanceEnd, kInvalidColor, 230 },
  { kTraceRightEdge, 45, { 0.15, 0, 0.06 }, kColorEnd, kYellow, 0},
  { kGoForward, 30, { }, kDistanceEnd, kInvalidColor, 6 },
};

const int kRcourseParamsNum = 45;
const DrivingParam kRcourseParams[kRcourseParamsNum] = {
  { kTraceLeftEdge, 58, { 0.30, 0, 0.08 }, kDistanceEnd, kInvalidColor, 30 },
  { kTraceLeftEdge, 68, { 0.30, 0, 0.08 }, kDistanceEnd, kInvalidColor, 50 },
  { kTraceLeftEdge, 78, { 0.30, 0, 0.08 }, kDistanceEnd, kInvalidColor, 50 },
  { kTraceLeftEdge, 88, { 0.30, 0, 0.08 }, kDistanceEnd, kInvalidColor, 50 },
  { kTraceLeftEdge, 93, { 0.30, 0, 0.08 }, kDistanceEnd, kInvalidColor, 1100 },
  { kTraceLeftEdge, 88, { 0.30, 0, 0.08 }, kDistanceEnd, kInvalidColor, 80 },
  { kTraceLeftEdge, 78, { 0.32, 0, 0.08 }, kDistanceEnd, kInvalidColor, 80 },
  { kTraceLeftEdge, 68, { 0.34, 0, 0.08 }, kDistanceEnd, kInvalidColor, 50 },
  { kTraceLeftEdge, 60, { 0.36, 0, 0.08 }, kDistanceEnd, kInvalidColor, 50 },
  { kTraceLeftEdge, 58, { 0.38, 0, 0.08 }, kDistanceEnd, kInvalidColor, 30 },
  { kTraceLeftEdge, 55, { 0.40, 0, 0.08 }, kDistanceEnd, kInvalidColor, 30 },

  { kTraceLeftEdge, 55, { 0.48, 0, 0.08 }, kDistanceEnd, kInvalidColor, 750 },

  { kTraceLeftEdge, 55, { 0.40, 0, 0.08 }, kDistanceEnd, kInvalidColor, 30 },
  { kTraceLeftEdge, 58, { 0.38, 0, 0.08 }, kDistanceEnd, kInvalidColor, 30 },
  { kTraceLeftEdge, 73, { 0.36, 0, 0.08 }, kDistanceEnd, kInvalidColor, 50 },
  { kTraceLeftEdge, 88, { 0.34, 0, 0.08 }, kDistanceEnd, kInvalidColor, 370 },
  { kTraceLeftEdge, 73, { 0.36, 0, 0.08 }, kDistanceEnd, kInvalidColor, 50 },
  { kTraceLeftEdge, 60, { 0.38, 0, 0.08 }, kDistanceEnd, kInvalidColor, 30 },
  { kTraceLeftEdge, 58, { 0.40, 0, 0.08 }, kDistanceEnd, kInvalidColor, 20 },

  { kTraceLeftEdge, 58, { 0.48, 0, 0.08 }, kDistanceEnd, kInvalidColor, 630 },

  { kTraceLeftEdge, 58, { 0.40, 0, 0.08 }, kDistanceEnd, kInvalidColor, 30 },
  { kTraceLeftEdge, 58, { 0.38, 0, 0.08 }, kDistanceEnd, kInvalidColor, 50 },
  { kTraceLeftEdge, 68, { 0.36, 0, 0.08 }, kDistanceEnd, kInvalidColor, 50 },
  { kTraceLeftEdge, 80, { 0.34, 0, 0.08 }, kDistanceEnd, kInvalidColor, 50 },
  { kTraceLeftEdge, 88, { 0.30, 0, 0.08 }, kDistanceEnd, kInvalidColor, 50 },
  { kTraceLeftEdge, 93, { 0.30, 0, 0.08 }, kDistanceEnd, kInvalidColor, 1080 },
  { kTraceLeftEdge, 88, { 0.30, 0, 0.08 }, kDistanceEnd, kInvalidColor, 50 },
  { kTraceLeftEdge, 80, { 0.30, 0, 0.08 }, kDistanceEnd, kInvalidColor, 50 },
  { kTraceLeftEdge, 75, { 0.35, 0, 0.08 }, kDistanceEnd, kInvalidColor, 50 },
  { kTraceLeftEdge, 65, { 0.38, 0, 0.08 }, kDistanceEnd, kInvalidColor, 30 },
  { kTraceLeftEdge, 60, { 0.40, 0, 0.08 }, kDistanceEnd, kInvalidColor, 30 },
  { kTraceLeftEdge, 55, { 0.43, 0, 0.08 }, kDistanceEnd, kInvalidColor, 30 },
  { kTraceLeftEdge, 50, { 0.45, 0, 0.08 }, kDistanceEnd, kInvalidColor, 30 },

  { kTraceLeftEdge, 50, { 0.58, 0, 0.08 }, kDistanceEnd, kInvalidColor, 370 },

  { kTraceLeftEdge, 50, { 0.43, 0, 0.08 }, kDistanceEnd, kInvalidColor, 30 },
  { kTraceLeftEdge, 60, { 0.38, 0, 0.08 }, kDistanceEnd, kInvalidColor, 50 },
  { kTraceLeftEdge, 75, { 0.33, 0, 0.08 }, kDistanceEnd, kInvalidColor, 50 },
  { kTraceLeftEdge, 88, { 0.30, 0, 0.08 }, kDistanceEnd, kInvalidColor, 50 },
  { kTraceLeftEdge, 88, { 0.28, 0, 0.08 }, kDistanceEnd, kInvalidColor, 70 },
  { kTraceLeftEdge, 90, { 0.28, 0, 0.08 }, kDistanceEnd, kInvalidColor, 430 },
  { kGoForward, 80, { }, kDistanceEnd, kInvalidColor, 250 },
  { kTraceLeftEdge, 60, { 0.31, 0, 0.06 }, kDistanceEnd, kInvalidColor, 790 },
  { kGoForward, 70, { }, kDistanceEnd, kInvalidColor, 230 },
  { kTraceLeftEdge, 45, { 0.15, 0, 0.06 }, kColorEnd, kYellow, 0},
  { kGoForward, 30, { }, kDistanceEnd, kInvalidColor, 6 },
};

StateManager::StateManager(DrivingManager* driving_manager, BingoAgent* bingo_agent, bool is_Rcourse)
    : driving_manager_(driving_manager), bingo_agent_(bingo_agent), is_Rcourse_(is_Rcourse),
      state_(kTimeAttack) {
  if (is_Rcourse_) {
    for (int i = 0; i < kRcourseParamsNum; ++i) {
      driving_manager_->AddDrivingParam(kRcourseParams[i]);
    }
  } else {
    for (int i = 0; i < kLcourseParamsNum; ++i) {
      driving_manager_->AddDrivingParam(kLcourseParams[i]);
    }
  }
}

void StateManager::Update() {
  switch (state_) {
    case kTimeAttack:
      ActTimeAttack();
      break;

    case kBlockBingo:
      ActBlockBingo();
      break;

    default:
      break;
  }
}

void StateManager::ActTimeAttack() {
  driving_manager_->Update();
  if (driving_manager_->DrivingParamsEmpty()) {
    state_ = kBlockBingo;
  }
}

void StateManager::ActBlockBingo() {
  if (bingo_agent_->calc_completed_) {
    ParamStore* param_store = bingo_agent_->param_store_;
    std::list<DrivingParam>& driving_params = param_store->driving_params_;
    if (!driving_params.empty()) {
      DrivingParam driving_param = driving_params.front();
      driving_manager_->AddDrivingParam(driving_param);
      driving_params.pop_front();
    }
  }
  driving_manager_->Update();
}
