#include "../include/types.h"
#include "move/create.h"
#include "move/generate.h"
#include "utils/loadFEN.h"
#include "utils/printBoard.h"
#include <bitset>
#include <iostream>

int main() {
  GameState state;
  loadFEN(state);
  printBoard(state);
  for (auto bitboard : state.bitboards) {
    std::cout << std::bitset<64>(bitboard) << std::endl;
  }

  std::array<move, MAX_MOVES> moves = {0};
  generateMoves(state, moves);
  Board::Piece capturedPiece = makeMove(state, moves[0]);
  std::cout << "Captured Piece: " << capturedPiece << std::endl;
  printBoard(state);
  for (auto bitboard : state.bitboards) {
    std::cout << std::bitset<64>(bitboard) << std::endl;
  }

  unmakeMove(state, moves[0], capturedPiece);
  printBoard(state);
  for (auto bitboard : state.bitboards) {
    std::cout << std::bitset<64>(bitboard) << std::endl;
  }

  return 0;
}
