#pragma once

#include "encode.h"
#include "enums.h"
#include <array>

struct ScoredMove {
  move value;
  int score;
};

void orderMoves(GameState &state, std::array<ScoredMove, MAX_MOVES> moves);
