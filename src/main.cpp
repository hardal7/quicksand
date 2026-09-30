#include "../include/types.h"
#include "move/generateMoves.h"
#include "utils/loadFEN.h"
#include "utils/printBoard.h"

int main() {
  GameState state;
  loadFEN(state);
  printBoard(state);
  generateMoves(state);

  return 0;
}
