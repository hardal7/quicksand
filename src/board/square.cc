#include "../types.h"
#include <map>
#include <string>

const std::map<char, int> CharToSquares = {
    {'a', 0},
    {'b', 1},
    {'c', 2},
    {'d', 3},
    {'e', 4},
    {'f', 5},
    {'g', 6},
    {'h', 7},

    {'8', 0 * Board::RankSquares},
    {'7', 1 * Board::RankSquares},
    {'6', 2 * Board::RankSquares},
    {'5', 3 * Board::RankSquares},
    {'4', 4 * Board::RankSquares},
    {'3', 5 * Board::RankSquares},
    {'2', 6 * Board::RankSquares},
    {'1', 7 * Board::RankSquares},
};

const std::map<int, char> CharFromSquares = {
    {0, 'a'},
    {1, 'b'},
    {2, 'c'},
    {3, 'd'},
    {4, 'e'},
    {5, 'f'},
    {6, 'g'},
    {7, 'h'},

    {0 * Board::RankSquares, '8'},
    {1 * Board::RankSquares, '7'},
    {2 * Board::RankSquares, '6'},
    {3 * Board::RankSquares, '5'},
    {4 * Board::RankSquares, '4'},
    {5 * Board::RankSquares, '3'},
    {6 * Board::RankSquares, '2'},
    {7 * Board::RankSquares, '1'},
};

int squareFromPosition(std::string position) {
  int square = 0;
  auto it = CharToSquares.find(position[0]);
  square += it->second;
  it = CharToSquares.find(position[1]);
  square += it->second;

  return square;
}

std::string squareToPosition(int square) {
  std::string position = "";

  auto it = CharFromSquares.find(square % Board::FileSquares);
  position += it->second;
  it = CharFromSquares.find(square / Board::RankSquares);
  position += it->second;

  return position;
}
