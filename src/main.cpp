#include "../include/types.h"
#include "move/create.h"
#include "move/generate.h"
#include "utils/loadFEN.h"
#include "utils/printBoard.h"
#include <iostream>

int search(GameState &state, int depth, int nodes = 0) {
  if (depth != 0) {
    std::array<move, MAX_MOVES> moves = {0};
    generateMoves(state, moves);
    for (move m : moves) {
      if (m == 0) {
        break;
      }
      Board::Piece capturedPiece = makeMove(state, m);
      nodes++;
      nodes = search(state, depth - 1, nodes);
      unmakeMove(state, moves[0], capturedPiece);
    }
  }
  return nodes;
}

int main() {
  GameState state;
  loadFEN(state, "r1bk3r/p2pBpNp/n4n2/1p1NP2P/6P1/3P4/P1P1K3/q5b1");
  printBoard(state);
  std::cout << search(state, 1) << std::endl;

  return 0;
}
