#include "transposition.h"
#include "../move/move.h"
#include "../game.h"
#include "zobrist.h"
#include <optional>
#include <unordered_map>

std::optional<Transposition> searchTranspositions(const GameState &state, const TTable &ttable, int depth) {
  Key key = getZobristKey(state);
  auto it = ttable.find(key);
  if (it != ttable.end()) {
    if (it->second.depth >= depth) {
      return it->second;
    }
  }

  return std::nullopt;
}

void saveTransposition(const GameState &state, TTable &ttable, int depth, int eval, Move::Encoded move) {
  Key key = getZobristKey(state);

  Transposition pastTransposition;
  bool transpositionFound = false;

  auto it = ttable.find(key);
  if (it != ttable.end()) {
    if (it->second.depth >= depth) {
      pastTransposition = it->second;
      transpositionFound = true;
    }
  }

  if (transpositionFound) {
    return;
  }

  ttable[key] = Transposition{depth, eval, move};
}
