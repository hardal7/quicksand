#include "../interface/polyglot.h"
#include "../move/move.h"
#include "../search/zobrist.h"
#include <fstream>
#include <vector>

__uint128_t byteSwap128(__uint128_t x) {
  uint64_t low = static_cast<uint64_t>(x);
  uint64_t high = static_cast<uint64_t>(x >> 64);

  low = __builtin_bswap64(low);
  high = __builtin_bswap64(high);

  return (static_cast<__uint128_t>(low) << 64) | high;
}

std::vector<Move::Encoded> searchOpening(const GameState &state) {
  std::vector<Move::Encoded> openings;

  Key positionKey = GetZobristKey(state);

  std::ifstream file("./assets/book.bin", std::ios::binary);
  if (!file) {
    return {};
  }

  __uint128_t entry;
  char *entryAddress = reinterpret_cast<char *>(&entry);

  int keyOffset = 64;
  __uint128_t keyOffsetMask = 0xFFFFFFFFFFFFFFFF;
  int moveOffset = 48;
  __uint128_t moveOffsetMask = 0xFFFF;

  while (file.read(entryAddress, sizeof(entry))) {
    entry = byteSwap128(entry);
    Move::Encoded move = (entry >> moveOffset) & moveOffsetMask;
    Key key = (entry >> keyOffset) & keyOffsetMask;

    if (key == positionKey) {
      openings.push_back(convertPolyglotMove(move));
    }
  }

  return openings;
}
