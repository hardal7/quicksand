#pragma once

#include "../board/board.h"
#include "../move/move.h"
#include "../game.h"

struct ScoredMove {
  Move::Encoded Move;
  int Score;
};

struct OrderedList {
  std::array<ScoredMove, Move::MaxMoves> list = {Move::NoMove, 0};
  int length = 0;
};

const int MinCaptureScore = 10;

OrderedList orderMoves(const GameState &state, Move::List moves);
