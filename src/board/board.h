#pragma once

#include <array>
#include <cstdint>

const int BOARD_SQUARES = 64, RANK_SQUARES = 8, PIECES = 8, COLORS = 2;

class Board {
public:
  using Piece = int;
  enum Indexes : Piece {
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
  using Bitboard = uint64_t;
  using Bitboards = std::array<Bitboard, PIECES>;
};

struct GameState {
  Board::Bitboards bitboards = {0};
  bool whiteToPlay = true;

  int enPassantSquare = 0;
  int enPassantSquarePrev = 0;
  bool canShortCastle = true;
  bool canLongCastle = true;
};
