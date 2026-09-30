#pragma once

#include <array>
#include <cstdint>

const int BOARD_SQUARES = 64;
const int ROW_SQUARES = 8;

class Bitboard {
public:
  static const int BITBOARDS_COUNT = 8;
  enum Indexes { Pawn, Knight, Bishop, Rook, Queen, King, White, Black };
  using Bitboards = std::array<uint64_t, BITBOARDS_COUNT>;
};

struct GameState {
  Bitboard::Bitboards bitboards = {0};
};
