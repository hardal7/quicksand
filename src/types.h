#pragma once

#include <cstdint>
#include <map>

class Board {
public:
  using bitboard = uint64_t;

  static const int Pieces = 6;
  static const int Colors = 2;

  static const int TotalSquares = 64;
  static const int FileSquares = 8;
  static const int RankSquares = 8;

  using Piece = int;
  enum Indexes : Piece {
    Pawn,
    Knight,
    Bishop,
    Rook,
    Queen,
    King,
    White,
    Black
  };
  bitboard Bitboards[Pieces + Colors] = {0};

  inline static const std::map<char, Piece> PieceFromChars = {
      {'p', Pawn}, {'n', Knight}, {'b', Bishop},
      {'r', Rook}, {'q', Queen},  {'k', King},
  };
  inline static const std::map<Piece, char> PieceToChars = {
      {Pawn, 'p'}, {Knight, 'n'}, {Bishop, 'b'},
      {Rook, 'r'}, {Queen, 'q'},  {King, 'k'},
  };
};

struct GameState : Board {
  bool WhiteToPlay = true;

  bool WhiteShortCastle = true;
  bool WhiteLongCastle = true;
  bool BlackShortCastle = true;
  bool BlackLongCastle = true;
};
