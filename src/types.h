#pragma once

#include <cstdint>
#include <map>

class Board {
public:
  using bitboard = uint64_t;

  static const int Pieces = 6;
  static const int Colors = 2;

  enum Indexes { Pawn, Knight, Bishop, Rook, Queen, King, White, Black };
  bitboard Bitboards[Pieces + Colors] = {0};
};

class Piece {
public:
  enum Type { Pawn, Knight, Bishop, Rook, Queen, King, None };

  inline static const std::map<char, Type> Chars = {
      {'p', Pawn}, {'n', Knight}, {'b', Bishop},
      {'r', Rook}, {'q', Queen},  {'k', King},
  };
};

struct GameState : Board {
  bool WhiteToPlay = true;

  bool WhiteShortCastle = true;
  bool WhiteLongCastle = true;
  bool BlackShortCastle = true;
  bool BlackLongCastle = true;
};
