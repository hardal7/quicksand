#include "board/fen.h"
#include "board/print.h"
#include "move/move.h"
#include "types.h"

int main() {
  GameState state = loadFEN();
  printBoard(state);

  Move::List movesList = Move::generate(state);
}
