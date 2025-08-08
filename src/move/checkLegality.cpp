#include <cmath>
#include <cstdint>

#include "../utils/enums.hpp"

bool checkLegality(uint64_t pieceBitboard[8], uint16_t move, int color) {
  int kingSquare = log2(pieceBitboard[King] & pieceBitboard[color]);

  return true;
}
