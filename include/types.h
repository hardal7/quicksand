#pragma once

#include <array>
#include <cstdint>

const int BOARD_SQUARES = 64;
const int RANK_SQUARES = 8;

class Bitboard {
public:
  static const int BITBOARDS_COUNT = 8;
  enum Indexes {
    None = -1,
    Pawn,
    Knight,
    Bishop,
    Rook,
    Queen,
    King,
    White,
    Black
  };
  using Bitboards = std::array<uint64_t, BITBOARDS_COUNT>;
};

struct GameState {
  Bitboard::Bitboards bitboards = {0};
  bool whiteToPlay = true;

  int enPassantSquare = 0;
  bool canShortCastle = false;
  bool canLongCastle = false;
};
