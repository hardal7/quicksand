#include "board/fen.h"
#include "board/print.h"
#include "move/generate.h"
#include "move/move.h"
#include "types.h"

int main() {
  GameState state;
  loadFEN(state);
  printBitboards(state);
  printBoard(state);
  Move::List movesList;
  generateMoves(state, movesList);

  return 0;
}
