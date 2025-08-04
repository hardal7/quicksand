#include <cmath>
#include <cstdint>

#include "../utils/enums.hpp"

// TODO: Check if move is legal before evaluating, if not: set eval to infinite zero so that it can never be picked as bestMove
bool checkLegality(uint64_t pieceBitboard[8], uint16_t move, int color) {
  int kingSquare = log2(pieceBitboard[King] & pieceBitboard[color]);

  return true;
}
