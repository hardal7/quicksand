#pragma once

#include "../game.h"
#include "../move/move.h"
#include "zobrist.h"
#include <optional>
#include <unordered_map>

struct Transposition {
  int depth;
  int eval;
  Move::Encoded bestMove;
};

using TTable = std::unordered_map<Key, Transposition>;

std::optional<Transposition> searchTranspositions(const GameState &state, const TTable &ttable, int depth);
void saveTransposition(const GameState &state, TTable &ttable, int depth, int eval, Move::Encoded move);
