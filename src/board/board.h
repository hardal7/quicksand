#pragma once

#include <cstdint>
#include <map>
#include <string>

class Board {
  static inline const std::map<char, int> CharToSquares = {
      {'a', 0}, {'b', 1}, {'c', 2}, {'d', 3}, {'e', 4}, {'f', 5}, {'g', 6}, {'h', 7},

      {'8', 0}, {'7', 1}, {'6', 2}, {'5', 3}, {'4', 4}, {'3', 5}, {'2', 6}, {'1', 7},
  };

  static inline const std::map<int, char> CharFromSquares = {
      {0, 'a'}, {1, 'b'}, {2, 'c'}, {3, 'd'}, {4, 'e'}, {5, 'f'}, {6, 'g'}, {7, 'h'},

      {0, '8'}, {1, '7'}, {2, '6'}, {3, '5'}, {4, '4'}, {5, '3'}, {6, '2'}, {7, '1'},
  };

public:
  using bitboard = uint64_t;

  static const int Pieces = 6;
  static const int Colors = 2;

  static const int TotalSquares = 64;
  static const int FileSquares = 8;
  static const int RankSquares = 8;

  using Piece = int;
  enum Indexes : Piece { Pawn, Knight, Bishop, Rook, Queen, King, White, Black };
  bitboard Bitboards[Pieces + Colors] = {0};

  inline static const std::map<char, Piece> PieceFromChars = {
      {'p', Pawn}, {'n', Knight}, {'b', Bishop}, {'r', Rook}, {'q', Queen}, {'k', King},
  };
  inline static const std::map<Piece, char> PieceToChars = {
      {Pawn, 'p'}, {Knight, 'n'}, {Bishop, 'b'}, {Rook, 'r'}, {Queen, 'q'}, {King, 'k'},
  };

  static int squareFromPosition(std::string position) {
    int square = 0;
    auto it = CharToSquares.find(position[0]);
    square += it->second;
    it = CharToSquares.find(position[1]);
    square += it->second * RankSquares;

    return square;
  }

  static std::string squareToPosition(int square) {
    std::string position = "";

    auto it = CharFromSquares.find(square % Board::FileSquares);
    position += it->second;
    it = CharFromSquares.find(square / Board::RankSquares);
    position += it->second;

    return position;
  }

  enum Directions { Up = -8, Down = 8, Left = -1, Right = 1 };
};
