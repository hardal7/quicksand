#include "../include/types.h"
#include "utils/loadFEN.h"
#include "utils/printBoard.h"
#include <bitset>
#include <iostream>

int main() {
  GameState state;
  loadFEN(state);
  for (auto bitboard : state.bitboards) {
    std::cout << std::bitset<64>(bitboard) << '\n';
  }
  printBoard(state);

  return 0;
}
