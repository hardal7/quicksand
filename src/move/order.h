#pragma once

#include "encode.h"
#include "enums.h"
#include <array>

struct ScoredMove {
  move value;
  int score;
};

const int MinCaptureScore = 1;

void orderMoves(GameState &state, std::array<ScoredMove, MAX_MOVES> &moves);
