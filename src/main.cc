#include "board/fen.h"
#include "board/print.h"
#include "types.h"

int main() {
  GameState state;
  loadFEN(state);
  printBitboards(state);
  printBoard(state);
  return 0;
}
