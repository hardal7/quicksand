#include "board.h"
#include <string>

using namespace Board;

int Board::squareFromPosition(std::string position) {
  int square = 0;
  auto it = CharToSquares.find(position[0]);
  square += it->second;
  it = CharToSquares.find(position[1]);
  square += it->second * RankSquares;

  return square;
}

std::string Board::squareToPosition(int square) {
  std::string position = "";

  auto it = CharFromSquares.find(square % Board::FileSquares);
  position += it->second;
  it = CharFromSquares.find(square / Board::RankSquares);
  position += it->second;

  return position;
}
