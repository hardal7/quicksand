#pragma once

#include <cstdint>
#include <map>
#include <string>

namespace Board {
using bitboard = uint64_t;

const int TotalPieces = 6;
const int TotalColors = 2;
const int TotalSquares = 64;
const int FileSquares = 8;
const int RankSquares = 8;

using Piece = int;
enum Index : Piece { Pawn, Knight, Bishop, Rook, Queen, King, White, Black, None };

const std::map<char, Piece> PieceFromChars = {
    {'p', Pawn}, {'n', Knight}, {'b', Bishop}, {'r', Rook}, {'q', Queen}, {'k', King},
};
const std::map<Piece, char> PieceToChars = {
    {Pawn, 'p'}, {Knight, 'n'}, {Bishop, 'b'}, {Rook, 'r'}, {Queen, 'q'}, {King, 'k'},
};

int squareFromPosition(std::string position);
std::string squareToPosition(int square);

enum Masks : bitboard {
  FileA = 0x0101010101010101,
  FileB = 0x0202020202020202,
  FileC = 0x0404040404040404,
  FileD = 0x0808080808080808,
  FileE = 0x1010101010101010,
  FileF = 0x2020202020202020,
  FileG = 0x4040404040404040,
  FileH = 0x8080808080808080,

  // clang-format off
  RankOne =   0xFF00000000000000,
  RankTwo =   0x00FF000000000000,
  RankThree = 0x0000FF0000000000,
  RankFour =  0x000000FF00000000,
  RankFive =  0x00000000FF000000,
  RankSix =   0x0000000000FF0000,
  RankSeven = 0x000000000000FF00,
  RankEight = 0x00000000000000FF,
  // clang-format on
};
}; // namespace Board
