#include "board.h"
#include <string>

using namespace Board;

int Board::squareFromPosition(std::string position) {
  const std::map<char, int> FileToSquares = {
      {'a', 0}, {'b', 1}, {'c', 2}, {'d', 3}, {'e', 4}, {'f', 5}, {'g', 6}, {'h', 7},
  };

  const std::map<char, int> RankToSquares = {
      {'8', 0}, {'7', 1}, {'6', 2}, {'5', 3}, {'4', 4}, {'3', 5}, {'2', 6}, {'1', 7},
  };

  int square = 0;
  auto it = FileToSquares.find(position[0]);
  square += it->second;
  it = RankToSquares.find(position[1]);
  square += it->second * RankSquares;

  return square;
}

std::string Board::squareToPosition(int square) {
  const std::map<int, char> FileFromSquares = {
      {0, 'a'}, {1, 'b'}, {2, 'c'}, {3, 'd'}, {4, 'e'}, {5, 'f'}, {6, 'g'}, {7, 'h'},

  };
  const std::map<int, char> RankFromSquares = {
      {0, '8'}, {1, '7'}, {2, '6'}, {3, '5'}, {4, '4'}, {5, '3'}, {6, '2'}, {7, '1'},
  };

  std::string position = "";

  auto it = FileFromSquares.find(square % Board::FileSquares);
  position += it->second;
  it = RankFromSquares.find(square / Board::RankSquares);
  position += it->second;

  return position;
}
