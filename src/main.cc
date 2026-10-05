#include "board/fen.h"
#include "board/print.h"
#include "move/move.h"
#include "search/search.h"
#include "types.h"
#include <iostream>

int main() {
  GameState state = loadFEN();
  printBoard(state);

  int depth = 3;
  Move::Encoded bestMove = searchBestMove(state, depth);
  std::cerr << "Best Move: " << Move::annotation(bestMove) << std::endl;
}
