#include "board/fen.h"
#include "board/print.h"
#include "move/generate.h"
#include "move/move.h"
#include "types.h"
#include <iostream>

int main() {
  GameState state = loadFEN();
  printBitboards(state);
  printBoard(state);

  Move::List movesList = generateMoves(state);
  for (int i = 0; i < movesList.length; i++) {
    std::cerr << Move::decode(movesList.list[i]) << std::endl;
  }

  return 0;
}
